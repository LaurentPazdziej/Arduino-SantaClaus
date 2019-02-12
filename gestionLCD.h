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
