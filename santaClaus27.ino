#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include "declarationCaracteresUtilisateurs.h"
#include "gestionScanCommandes.h"
#include "gestionRFID.h"

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
//Déclaration du LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);
//Monstre=1, cadeau=2
unsigned int afficheurLCD[2][33] = {
  {  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  999 },
  { 1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  999 }

};
//Vitesse du personnage
const unsigned int vitesse = 400;
//Si le personnage arrive à cette position alors on scrolle à gauche
const unsigned int positionDeScroll = 16;
//Delai d'affichage du parcours
const unsigned int dureeAffichageParcours = 10000;

static int colCurseur, ligCurseur;

static int sens = 1, scrolling = 0;

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
  Serial.println("s=start; d=droite; b=bas; h=haut; itération=2,3,4,5; fin itération=f; fin de saisie=s");
}

void loop() {
  char c = ' ';
  boolean collision = false;
  animationMessage();
  if (isCardRead())
  {
    c = getCodeCarte();
    if (c == 's')
    {
      lcd.clear();
      c = ' ';
      afficheLCDChaine("Scan en cours...", 0, 0);
      //Scan des cartes
      colCurseur = 0;
      String deplacements = scanCartes();
      //On denormalise
      deplacements = denormalisation(deplacements);
      if (deplacements.length() > 63)
      {
        afficheLCDChaine("Parcours trop long", 0, 0);

      }
      else
      {
        //On transforme la chaine en tableau de déplacements
        scan2Deplacements(deplacements);
        affichageParcours();
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


//h,b,d=haut,bas,droite
//f=fin itération
//e=fin de scan
String scanCartes()
{
  char c = ' ';
  String listeCommandes = "";
  afficheLCDChaine("Scan en cours...", 0, 0);
  while ( c != 's') {
    if (isCardRead())
    {
      c = getCodeCarte(); // on lit le charactère
      if (listeCommandes.length() == 0 && c == 's')
      {
        afficheLCDChaine("START-Non valide", 0, 0);
        c = ' ';
      }
      else
      {
        afficheLCDChaine("Scan en cours...", 0, 0);
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
  afficheLCDChaine("Scan en cours...", 0, 0);
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
        afficheLCDChaine("Deja entre.", 0, 0);
    }
    else if (commande == 's' && iteration == false)
      afficheLCDChaine("Fin de scan", 0, 0);
    else if (commande == 's' && iteration == true)
      afficheLCDChaine("START invalide", 0, 0);
    else if (commande == 'f' && iteration == false)
      afficheLCDChaine("FIN ] invalide", 0, 0);
  }
  else if (commande != 'f')
    afficheLCDChaine("Deja entre.", 0, 0);
  else if (commande == 'f' && iteration == false)
    afficheLCDChaine("FIN ] invalide", 0, 0);
  return listeCommandes;
}

//animation en cas d'incident de parcours, duree en s
void animationKO(int colonne, int ligne, int duree)
{
  if (colonne < 0)
    colonne = 0;
  if (ligne < 0)
    ligne = 0;
  for (int i = 1; i <= duree; i++)
  {
    lcd.setCursor(colonne, ligne);
    lcd.write(carPersonnageKO);
    delay(500);
    lcd.setCursor(colonne, ligne);
    lcd.write(carPersonnageOK);
    delay(500);
  }
}


boolean deplacementsLCD() {
  int nb = 0;
  boolean collision = false, finDeParcours = false;
  lcd.setCursor(0, 0);
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
      Serial.println(colonneCurseur[nb - 1]);
      Serial.println(ligneCurseur[nb - 1]);
      animationKO(colonneCurseur[nb - 1], ligneCurseur[nb - 1], dureeAffichageParcours / 1000 );
      collision = true;
      afficheLCDChaine("Collision !", 0, 0);
      afficheLCDChaine("Scanner START...", 0, 1);
    }
    else {
      lcd.setCursor(colonneCurseur[nb], ligneCurseur[nb]);
      lcd.write(carPersonnageOK);
      if (nb > 0)
        lcd.setCursor(colonneCurseur[nb - 1], ligneCurseur[nb - 1]);
      else
        lcd.setCursor(0, 0);
      lcd.print(" ");
    }
    //Si on arrive à la colonne positionDeScroll => scroll à gauche
    if (colonneCurseur[nb] > 0 && colonneCurseur[nb] % positionDeScroll == 0 && collision == false)
    {
      for (int positionCounter = 0; positionCounter < positionDeScroll; positionCounter++) {
        lcd.scrollDisplayLeft();
        delay(100);
      }
    }
    //Si on arrive au bout du parcours
    if (afficheurLCD[ligneCurseur[nb]][colonneCurseur[nb]] == 2)
    {
      lcd.clear();
      afficheLCDChaine("INDICE : ", 0, 0);
      afficheLCDChaine("Votre indice", 0, 1);
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
    afficheLCDChaine("Etes-vous perdu?", 0, 0);
    afficheLCDChaine("Scanner START...", 0, 1);
    collision = true;
  }
  return collision;
}


void afficheLCD(char c, int col, int lig)
{
  String chaine = "";
  lcd.setCursor(col, lig);
  if (col > 15)
    lcd.scrollDisplayLeft();
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
void afficheLCDChaine(String chaine, int col, int lig)
{
  lcd.home();
  lcd.setCursor(col, lig);
  lcd.print("                                ");
  lcd.setCursor(col, lig);
  lcd.print(chaine);
  Serial.println(chaine);

}

void affichageParcours()
{
  int lig = 0, col = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  while (lig < 2)
  {
    while (afficheurLCD[lig][col] != 999)
    {
      lcd.setCursor(col, lig);
      if (afficheurLCD[lig][col] == 1)
        lcd.write(carObstacle);
      else if (afficheurLCD[lig][col] == 2)
        lcd.write(carCadeau);
      else
        lcd.print(" ");
      col++;
    }
    lig++;
    col = 0;
  }
}

void animationMessage()
{
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
    delay(1000);
  }
}
