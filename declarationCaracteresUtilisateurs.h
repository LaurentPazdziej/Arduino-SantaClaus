//Afficheur à 2 ligne et 16 colonne
//Branchements I2C du LCD
//GND, VCC=5V, SDA=A4, SCL=A5
//Déclaration caractères utilisateurs
byte flecheBas[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100
};
byte flecheHaut[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
};
byte flecheDroite[8] = {
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000,
  B00000,
};
byte flecheGauche[8] = {
  B00000,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00000,
  B00000,
};
byte personnageOK[8] = {
  B00110,
  B00110,
  B00100,
  B01110,
  B10101,
  B00100,
  B01010,
  B01010
};
byte personnageKO[8] = {
  B01010,
  B01010,
  B00100,
  B10101,
  B01110,
  B00100,
  B00110,
  B00110,
};
byte obstacle[8] = {
  B01110,
  B11111,
  B10101,
  B10101,
  B11111,
  B10001,
  B11111,
  B10101
};

byte cadeau[8] = {
  B01001,
  B11010,
  B00100,
  B11111,
  B10101,
  B11111,
  B10101,
  B11111
};

const unsigned int carFlecheHaut = 0, carFlecheBas = 1, carFlecheDroite = 2, carPersonnageOK = 3, carPersonnageKO = 4, carObstacle = 5,
                   carCadeau = 6, carFlecheGauche = 7;
void declareCaracteresUtilisateur(LiquidCrystal_I2C lcd)
{
  lcd.init();
  lcd.createChar(carFlecheHaut, flecheHaut);
  lcd.createChar(carFlecheBas, flecheBas);
  lcd.createChar(carFlecheDroite, flecheDroite);
  lcd.createChar(carFlecheGauche, flecheGauche);
  lcd.createChar(carPersonnageOK, personnageOK);
  lcd.createChar(carPersonnageKO, personnageKO);
  lcd.createChar(carObstacle, obstacle);
  lcd.createChar(carCadeau, cadeau);
}


