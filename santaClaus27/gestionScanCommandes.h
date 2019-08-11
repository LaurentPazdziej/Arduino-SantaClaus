//Fonction qui extrait l'itÃ©ration de premier niveau
//Exemple : 2bd3dbff => bd3dbf
static int colonneCurseur[99], ligneCurseur[99];
String fextraction(String chaine)
{
  int longueur = chaine.length();
  String extraction = "";
  char c;
  int i = 1, profondeur = 0;
  while (i <= longueur  && profondeur >= 0)
  {
    c = chaine.charAt(i);
    if (c == '2' || c == '3' || c == '4' || c == '5')
      profondeur++;
    if (c == 'f')
      profondeur--;
    extraction += c;
    i++;
  }
  return extraction.substring(0, i - 2);
}

//On transforme la chaine de type 3df en ddd
String denormalisation(String chaine)
{
  int i = 0, j, longueurChaine = chaine.length(), longueurExtraction = 0, cInt = 0;
  char c;
  String remplaceExtraction;
  String extraction = "";
  //On lit caractÃ¨re par caractÃ¨re
  while (i < longueurChaine)
  {
    c = chaine.charAt(i);

    if (c == '2' || c == '3' || c == '4' || c == '5')
    {
      //Char to Int
      cInt = c - '0';
      //On extrait la sous iteration entre c et 'f'
      extraction = fextraction(chaine.substring(i, longueurChaine));
      longueurExtraction = extraction.length();
      remplaceExtraction = "";
      for (j = 1; j <= cInt; j++)
        remplaceExtraction += extraction;
      chaine = chaine.substring(0, i) + remplaceExtraction + chaine.substring(i + longueurExtraction + 2);
      
      //Si la longueur de chaine est supÃ©rieure Ã  63=nombre de dÃ©placements maximum => on arrÃªte
      //SINON On recommence avec la nouvelle chaine construite
      longueurChaine = chaine.length();
      Serial.println(longueurChaine);
      Serial.println(chaine);
      if (longueurChaine > 60)
        i = longueurChaine;
      else
        i = -1;
    }
    i++;
  }
  
  return chaine;
}

//On transforme la chaine de deplacement en tableau de deplacements pour le LCD
void scan2Deplacements(String deplacements)
{
  //Lecture de la chaine caractÃ¨res par caractÃ¨res
  int i = 0, nombreDeplacements = deplacements.length();
  //Serial.print("nombreDeplacements="); Serial.println(nombreDeplacements);
  colonneCurseur[0] = 0; ligneCurseur[0] = 1;
  for (i = 0; i <= nombreDeplacements - 1; i++)
  {
    //Serial.println(deplacements.charAt(i)); delay(10);
    switch (deplacements.charAt(i)) {
      case 'h':
        if (i > 0)
        {
          colonneCurseur[i] = colonneCurseur[i - 1]; //On reste sur la mÃªme colonne
          ligneCurseur[i] = ligneCurseur[i - 1] - 1; //On passe sur la ligne prÃ©cÃ©dente
        }
        else
        {
          //Gestion du premier dÃ©placement
          colonneCurseur[i] = 0;
          ligneCurseur[i] = 0;
        }
        break;
      case 'b':
        if (i > 0)
        {
          colonneCurseur[i] = colonneCurseur[i - 1]; //On reste sur la mÃªme colonne
          ligneCurseur[i] = ligneCurseur[i - 1] + 1; //On passe sur la ligne 1
        }
        else
        {
          //Gestion du premier dÃ©placement
          colonneCurseur[i] = 0;
          ligneCurseur[i] = -1;
        }
        break;
      case 'd':
        if (i > 0)
        {
          colonneCurseur[i] = colonneCurseur[i - 1] + 1; //On passe sur la colonne  suivante
          ligneCurseur[i] = ligneCurseur[i - 1]; //On reste sur la mÃªme ligne
        }
        else
        {
          //Gestion du premier dÃ©placement
          colonneCurseur[i] = 1;
          ligneCurseur[i] = 1;
        }
        break;
    }
  }
  colonneCurseur[i] = 999; //Fin du parcours
  ligneCurseur[i] = 999;

}

