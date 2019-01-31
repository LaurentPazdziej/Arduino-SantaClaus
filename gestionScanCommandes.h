//Fonction qui extrait l'itération de premier niveau
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
  //On lit caractère par caractère
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
      
      //Si la longueur de chaine est supérieure à 63=nombre de déplacements maximum => on arrête
      //SINON On recommence avec la nouvelle chaine construite
      longueurChaine = chaine.length();
      Serial.println(longueurChaine);
      if (longueurChaine > 63)
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
  //Lecture de la chaine caractères par caractères
  int i = 0, nombreDeplacements = deplacements.length();
  Serial.print("nombreDeplacements="); Serial.println(nombreDeplacements);
  colonneCurseur[0] = 0; ligneCurseur[0] = 0;
  for (i = 0; i <= nombreDeplacements - 1; i++)
  {
    //Serial.println(deplacements.charAt(i)); delay(10);
    switch (deplacements.charAt(i)) {
      case 'h':
        if (i > 0)
        {
          colonneCurseur[i] = colonneCurseur[i - 1]; //On reste sur la même colonne
          ligneCurseur[i] = ligneCurseur[i - 1] - 1; //On passe sur la ligne précédente
        }
        else
        {
          //Gestion du premier déplacement
          colonneCurseur[i] = 0;
          ligneCurseur[i] = - 1;
        }
        break;
      case 'b':
        if (i > 0)
        {
          colonneCurseur[i] = colonneCurseur[i - 1]; //On reste sur la même colonne
          ligneCurseur[i] = ligneCurseur[i - 1] + 1; //On passe sur la ligne 1
        }
        else
        {
          //Gestion du premier déplacement
          colonneCurseur[i] = 0;
          ligneCurseur[i] = 1;
        }
        break;
      case 'd':
        if (i > 0)
        {
          colonneCurseur[i] = colonneCurseur[i - 1] + 1; //On passe sur la colonne  suivante
          ligneCurseur[i] = ligneCurseur[i - 1]; //On reste sur la même ligne
        }
        else
        {
          //Gestion du premier déplacement
          colonneCurseur[i] = 1;
          ligneCurseur[i] = 0;
        }
        break;
    }
  }
  colonneCurseur[i] = 999; //Fin du parcours
  ligneCurseur[i] = 999;

}
