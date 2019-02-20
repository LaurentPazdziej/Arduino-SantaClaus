//Affichage d'une chaine sur le LCD en col et lig
void afficheLCDChaine(LiquidCrystal_I2C lcd, String chaine, int col, int lig)
{
  lcd.home();
  lcd.setCursor(col, lig);
  lcd.print("                                ");
  lcd.setCursor(col, lig);
  lcd.print(chaine);
  Serial.println(chaine);
}


//Affichage de l'indice
void afficheLCDIndice(LiquidCrystal_I2C lcd)
{
  afficheLCDChaine(lcd, "INDICE : ", 0, 0);
  lcd.setCursor(0, 1);
  lcd.write(carFlecheBas);
  lcd.write(carFlecheHaut);
  lcd.write(carFlecheHaut);
  lcd.write(carFlecheDroite);
  lcd.write(carFlecheGauche);
  lcd.write(carFlecheHaut);
}



