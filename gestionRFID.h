//Défintion des cartes par rapport à l'action
#define START1 "428511026" 
#define START2 "2501488625" 
#define START3 "101517126" 
#define START4 "102013826" 
#define DROITE1 "102541126" 
#define DROITE2 "2501635925" 
#define DROITE3 "25021814325" 
#define DROITE4 "107110926" 
#define HAUT1 "2501181825" 
#define HAUT2 "2502139225" 
#define HAUT3 "1096026" 
#define HAUT4 "2502084825" 
#define BAS1 "25023824725" 
#define BAS2 "2501634225"
#define BAS3 "1021316426"
#define BAS4 "250153525"
#define DEUX1 "25013219725" 
#define DEUX2 "25020110425" 
#define DEUX3 "25013219325"
#define DEUX4 "10389326" 
#define TROIS1 "2501484125"
#define TROIS2 "103319026"
#define TROIS3 "10469326"
#define TROIS4 "25018123325"
#define QUATRE1 "2502473225"
#define QUATRE2 "25022320225"
#define QUATRE3 "2501862225"
#define QUATRE4 "101021726"
#define CINQ1 "2501488725" 
#define CINQ2 "2502422325" 
#define CINQ3 "25019215825"
#define CINQ4 "1022224926" 
#define FINITERATION1 "101777026"
#define FINITERATION2 "10538626"
#define FINITERATION3 "2501488925"
#define FINITERATION4 "10575126"

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
    if (codeCarte == START1 || codeCarte == START2 || codeCarte == START3 || codeCarte == START4)
      c = 's';
    else if (codeCarte == DROITE1 || codeCarte == DROITE2 || codeCarte == DROITE3 || codeCarte == DROITE4)
      c = 'd';
    else if (codeCarte == HAUT1 || codeCarte == HAUT2 || codeCarte == HAUT3 || codeCarte == HAUT4)
      c = 'h';
    else if (codeCarte == BAS1 || codeCarte == BAS2 || codeCarte == BAS3 || codeCarte == BAS4)
      c = 'b';
    else if (codeCarte == DEUX1 || codeCarte == DEUX2 || codeCarte == DEUX3 || codeCarte == DEUX4)
      c = '2';
    else if (codeCarte == TROIS1 || codeCarte == TROIS2 || codeCarte == TROIS3 || codeCarte == TROIS4)
      c = '3';
    else if (codeCarte == QUATRE1 || codeCarte == QUATRE2 || codeCarte == QUATRE3 || codeCarte == QUATRE4)
      c = '4';
    else if (codeCarte == CINQ1 || codeCarte == CINQ2 || codeCarte == CINQ3 || codeCarte == CINQ4)
      c = '5';
    else if (codeCarte == FINITERATION1 || codeCarte == FINITERATION2 || codeCarte == FINITERATION3 || codeCarte == FINITERATION4)
      c = 'f';
    else
      Serial.println("Carte Inconnue");
  }

  return c;
}
