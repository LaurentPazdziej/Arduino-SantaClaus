//LiquidCrystal I2C, Frank de Brahander, 1.1.2
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
//SPI 1.0.0
#include <SPI.h>
//MFRC522, GitHub Community 1.4.3
#include <MFRC522.h>
#include "declarationCaracteresUtilisateurs.h"
#include "gestionScanCommandes.h"
#include "gestionRFID.h"
#include "gestionLCD.h"

/*Branchements RFID
  SDA = blanc = D10
  SCK = Vert = D13
  MOSI = Jaune = D11
  MISO = Orange = D12
  IRQ = None
  GND = Noir
  RST = Bleu = D9
  3.3V = Rouge
*/
/*Branchements LCD
  GND = Noir =
  VCC 5V = Rouge =
  SDA = Gris = A4
  SCL = Marron = A5
*/

//DÃ©claration du LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);

//Vitesse du personnage
const unsigned int vitesse = 400;
//Si le personnage arrive Ã  cette position alors on scrolle Ã  gauche
const unsigned int positionDeScroll = 16;
//Delai d'affichage du parcours
const unsigned int dureeAffichageParcours = 10000;
static int colCurseur, ligCurseur;

static int sens = 1, scrolling = 0;
boolean scroll = true; //Le scroll doit se dÃ©clencher une seule fois.

void setup()
{
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  declareCaracteresUtilisateur(lcd);
  initLCD();
  delay(1000);
  sens = 1;
  ligCurseur = 1;
  colCurseur = 0;
  scrolling = 0;
  //Serial.println("s=start; d=droite; b=bas; h=haut; itÃ©ration=2,3,4,5; fin iteration=f; fin de saisie=s");
}

void loop() {
  char c = ' ';
  boolean collision = false;
  scroll = true; //Le scroll doit se declencher une seule fois.
  animationMessage();
  if (isCardRead())
  {
    c = getCodeCarte();
    if (c == 's')
    {
      lcd.clear();
      c = ' ';
      afficheLCDChaine(lcd, "Scan en cours...", 0, 0);
      //Scan des cartes
      colCurseur = 0;
      String deplacements = scanCartes();
      //On denormalise
      deplacements = denormalisation(deplacements);
      if (deplacements.length() > 63)
      {
        afficheLCDChaine(lcd, "Chemin trop long", 0, 0);
      }
      else
      {
        //On transforme la chaine en tableau de deplacements
        scan2Deplacements(deplacements);
        affichageParcours(lcd);
        collision = deplacementsLCD();
      }
    }
  }
}
//Init du LCD
void initLCD()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.print("Bienvenue !");
  lcd.setCursor(0, 1);
  lcd.print("Scanner START...");

}


String scanCartes()
{
  char c = ' ';
  String listeCommandes = "";
  afficheLCDChaine(lcd, "Scan en cours", 0, 0);
  while ( c != 's') {

    if (isCardRead())
    {
      c = getCodeCarte(); // on lit le charactere
      if (listeCommandes.length() == 0 && c == 's')
      {
        afficheLCDChaine(lcd, "START invalide", 0, 0);
        c = ' ';
      }
      else
      {
        afficheLCDChaine(lcd, "Scan en cours", 0, 0);
        listeCommandes = ajoutCommande(c, listeCommandes );
        
      }
      delay(10); // petit temps de pause
    }
  }
  Serial.print("Liste Commandes:");
  Serial.println(listeCommandes);
  return listeCommandes;
}

static boolean iteration = false, iteration2Entree = false;
String ajoutCommande(char commande, String listeCommandes )
{
  //afficheLCDChaine(lcd, "Scan en cours", 0, 0);
  if (listeCommandes.endsWith((String)commande) == false)
  {
    if (commande == 'h' || commande == 'b' || commande == 'd' )  {
      listeCommandes = listeCommandes + commande;
      afficheLCD(commande, colCurseur, ligCurseur);
    }
    else if (commande == '2' || commande == '3' || commande == '4' || commande == '5')
    {
      if (listeCommandes.indexOf(commande) < 0) {
        listeCommandes += commande;
        afficheLCD(commande, colCurseur, ligCurseur);
        while (commande != 'f') {
          if (isCardRead())
          {
            commande = getCodeCarte();
            iteration = true;
            listeCommandes = ajoutCommande(commande, listeCommandes );
            
          }
          if (commande == 'f')
          {
            afficheLCD(commande, colCurseur, ligCurseur);
            iteration = false;
          }
        }
        listeCommandes += commande;
      }
      else
        afficheLCDChaine(lcd, "Deja entre.", 0, 0);
    }
    else if (commande == 's' && iteration == false)
      afficheLCDChaine(lcd, "Fin de scan", 0, 0);
    else if (commande == 's' && iteration == true)
      afficheLCDChaine(lcd, "START invalide", 0, 0);
    else if (commande == 'f' && iteration == false)
      afficheLCDChaine(lcd, "FIN ] invalide", 0, 0);
  }
  else if (commande != 'f')
    afficheLCDChaine(lcd, "Deja entre.", 0, 0);
  else if (commande == 'f' && iteration == false)
    afficheLCDChaine(lcd, "FIN ] invalide", 0, 0);
  return listeCommandes;
}

//animation en cas d'incident de parcours, duree en s
void animationKO(int colonne, int ligne, int duree)
{
  lcd.setCursor(0, 1);
  lcd.print(" ");
  if (colonne < 0)
    colonne = 0;
  if (ligne < 0)
    ligne = 0;
  int i = 1;
  while (i <= duree && isCardRead() == false)
  {
    lcd.setCursor(colonne, ligne);
    lcd.write(carPersonnageKO);
    delay(500);
    lcd.setCursor(colonne, ligne);
    lcd.write(carPersonnageOK);
    delay(500);
    i++;
  }
}


boolean deplacementsLCD() {
  int nb = 0;
  boolean collision = false, finDeParcours = false;
  lcd.setCursor(0, 1);
  lcd.write(carPersonnageOK);
  delay(vitesse);

  while (colonneCurseur[nb] != 999 && collision == false && finDeParcours == false)
  {
    Serial.print(colonneCurseur[nb]); Serial.print(","); Serial.print(ligneCurseur[nb]);
    Serial.print(","); Serial.println(afficheurLCD[ligneCurseur[nb]][colonneCurseur[nb]]);

    if (colonneCurseur[nb] < 0 || ligneCurseur[nb] < 0 || ligneCurseur[nb] > 1 || afficheurLCD[ligneCurseur[nb]][colonneCurseur[nb]] == 1 || afficheurLCD[ligneCurseur[nb]][colonneCurseur[nb]] == 999)
    {
      //lcd.setCursor(colonneCurseur[nb - 1], ligneCurseur[nb - 1]);
      //lcd.write(carPersonnageKO);
      //Serial.println(colonneCurseur[nb - 1]);
      //Serial.println(ligneCurseur[nb - 1]);
      animationKO(colonneCurseur[nb - 1], ligneCurseur[nb - 1], dureeAffichageParcours / 1000 );
      collision = true;
      afficheLCDChaine(lcd, "Collision !", 0, 0);
      afficheLCDChaine(lcd, "Scanner START...", 0, 1);
    }
    else {
      lcd.setCursor(colonneCurseur[nb], ligneCurseur[nb]);
      lcd.write(carPersonnageOK);
      if (nb > 0)
        lcd.setCursor(colonneCurseur[nb - 1], ligneCurseur[nb - 1]);
      else
        lcd.setCursor(0, 1);
      lcd.print(" ");
    }
    //Si on arrive Ã  la colonne positionDeScroll => scroll Ã  gauche
    if (colonneCurseur[nb] > 0 && colonneCurseur[nb] % positionDeScroll == 0 && collision == false && scroll == true)
    {
      for (int positionCounter = 0; positionCounter < positionDeScroll; positionCounter++) {
        lcd.scrollDisplayLeft();
        delay(100);
      }
      scroll = false; //Le scroll ne s'effectue qu'une fois
    }
    //Si on arrive au bout du parcours
    if (afficheurLCD[ligneCurseur[nb]][colonneCurseur[nb]] == 2)
    {
      lcd.clear();
      afficheLCDIndice(lcd);
      //afficheLCDChaine(lcd, "INDICE : ", 0, 0);
      //afficheLCDChaine(lcd, indice, 0, 1);
      finDeParcours = true;
    }

    delay(vitesse);
    nb++;
  }
  //Si personnage perdu
  if (collision == false && finDeParcours == false)
  {
    //lcd.setCursor(colonneCurseur[nb - 1], ligneCurseur[nb - 1]);
    //lcd.write(carPersonnageKO);
    animationKO(colonneCurseur[nb - 1], ligneCurseur[nb - 1], dureeAffichageParcours / 1000 );
    afficheLCDChaine(lcd, "Etes-vous perdu?", 0, 0);
    afficheLCDChaine(lcd, "Scanner START...", 0, 1);
    collision = true;
  }
  return collision;
}


void afficheLCD(char c, int col, int lig)
{

  String chaine = "";
  if (colCurseur > 14)
  {
    lcd.setCursor(0, 1);
    lcd.print("                                ");
    colCurseur = 0;
    col = 0;
  }
  lcd.setCursor(col, lig);
  // if (col > 15)
  //  lcd.scrollDisplayLeft();
  switch (c) {
    case 'd':
      lcd.write(carFlecheDroite);
      colCurseur++;
      break;
    case 'h':
      lcd.write(carFlecheHaut);
      colCurseur++;
      break;
    case 'b':
      lcd.write(carFlecheBas);
      colCurseur++;
      break;
    case '2':
    case '3':
    case '4':
    case '5':
      chaine += c;
      chaine += '[';
      colCurseur += 2;
      lcd.print(chaine);
      break;
    case 'f':
      lcd.print("]");
      colCurseur++;
      break;
    default :
      lcd.print(c);
  }
}

void animationMessage()
{
  int duree = 200, i = 1;
  //Scrolling du parcours
  delay(50);
  if (sens > 0)
    lcd.scrollDisplayRight();
  else
    lcd.scrollDisplayLeft();
  scrolling = scrolling + sens;
  if (scrolling >= 5 || scrolling < -5)
    sens = -sens;
  if   (scrolling == 0 && sens > 0)
  {
    lcd.home();
    lcd.noBacklight();
    delay(200);
    lcd.backlight();
    i = 0;
    while (i <= duree && isCardRead() == false) {
      delay(1);
      i++;
    }
  }
}

