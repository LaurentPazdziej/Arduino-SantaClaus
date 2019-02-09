//Défintion des cartes par rapport à l'action
#define START1 "1481854252" //Badge Rouge
#define START2 "1481854252" //Badge Rouge
#define START3 "1481854252" //Badge Rouge
#define DROITE1 "9502014" //Carte Blanche
#define DROITE2 "9502014" //Carte Blanche
#define DROITE3 "9502014" //Carte Blanche
#define HAUT1 "520721211" //Badge bleu
#define HAUT2 "520721211" //Badge bleu
#define HAUT3 "520721211" //Badge bleu
#define BAS1 "2113618836" //Carte Blanche
#define BAS2 "2113618836"
#define BAS3 "2113618836"
#define DEUX1 "20927119137" //Carte Blanche 2
#define DEUX2 "20927119137" 
#define DEUX3 "20927119137" 
#define TROIS1 "16016819737"
#define TROIS2 "16016819737"
#define TROIS3 "16016819737"
#define QUATRE1 "211621636"
#define QUATRE2 "211621636"
#define QUATRE3 "211621636"
#define CINQ1 "21113413936" 
#define CINQ2 "21113413936" 
#define CINQ3 "21113413936" 
#define FINITERATION1 "23023382195" //Badge bleu
#define FINITERATION2 "23023382195"
#define FINITERATION3 "23023382195"

//Déclaration du module RFID
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

//Carte lue ou saisie au clavier ?
boolean isCardRead()
{
  // Look for new cards
  if ( rfid.PICC_IsNewCardPresent() == true)
    // Verify if the NUID has been readed
    if (rfid.PICC_ReadCardSerial() == true) {
      while (Serial.read() != -1); //Vidage du buffer série
      return true;
    }
  //Saisie au clavier
  return Serial.available();
}

//Lecture du contenu de la carte ou clavier
char getCodeCarte()
{
  String  codeCarte = "";
  char c = ' ';

  c = Serial.read();
  //Si saisie clavier alors c!=-1
  if (c == -1)
  {
    //Carte lue
    // Store NUID into string
    for (byte i = 0; i < 4; i++)
      codeCarte += rfid.uid.uidByte[i];
    // Halt PICC
    rfid.PICC_HaltA();
    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();

    Serial.println(codeCarte);
    if (codeCarte == START1 || codeCarte == START2 || codeCarte == START3)
      c = 's';
    else if (codeCarte == DROITE1 || codeCarte == DROITE2 || codeCarte == DROITE3)
      c = 'd';
    else if (codeCarte == HAUT1 || codeCarte == HAUT2 || codeCarte == HAUT3)
      c = 'h';
    else if (codeCarte == BAS1 || codeCarte == BAS2 || codeCarte == BAS3)
      c = 'b';
    else if (codeCarte == DEUX1 || codeCarte == DEUX2 || codeCarte == DEUX3)
      c = '2';
    else if (codeCarte == DEUX1 || codeCarte == DEUX2 || codeCarte == DEUX3)
      c = '2';
    else if (codeCarte == TROIS1 || codeCarte == TROIS2 || codeCarte == TROIS3)
      c = '3';
    else if (codeCarte == QUATRE1 || codeCarte == QUATRE2 || codeCarte == QUATRE3)
      c = '4';
    else if (codeCarte == CINQ1 || codeCarte == CINQ2 || codeCarte == CINQ3)
      c = '5';
    else if (codeCarte == FINITERATION1 || codeCarte == FINITERATION2 || codeCarte == FINITERATION3)
      c = 'f';
    else
      Serial.println("Carte Inconnue");
  }

  return c;
}
