//Déclaration du parcours; Monstre=1, cadeau=2
//Solution sdb4dfh25dffb3dfhds
unsigned int afficheurLCD[2][33] = {
  {  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  2,  0,  1,  0,  0,  0,  0,  1,  1,  1,  0,  0,  1,  999 },
  { 1,  0,  0,  0,  0,  0,  0,  1,  1,  0,  1,  0,  1,  1,  1,  0,  0,  0,  0,  1,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  999 }
}  ;

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

void affichageParcours(LiquidCrystal_I2C lcd)
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


