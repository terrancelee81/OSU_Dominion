void checkCard(int* randomCard, int* deckHasCard, int* deckCount, int k[]){

  int y;
  for(y = 0; y < 10; y++){        
    if (k[y] == *randomCard)
        *deckHasCard = 1;
  }
  if (*deckHasCard == 1)
    return;
  else {
    k[*deckCount] = *randomCard;
      *deckCount = *deckCount + 1;
      return;
  }
}

void getRandomDeck(int k[]){
  int randomCard;
  int deckHasCard;
  int deckCount;

  deckCount = 0;
  //draw 10 random cards
  while(deckCount < 10){
    deckHasCard = 0;
    randomCard = random_number(7, 26);
    switch(randomCard)
    {
      case 7:
        randomCard = adventurer;
        checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      case 8:
        randomCard = council_room;
        checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      case 9:
        randomCard = feast;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break

      case 10:
        randomCard = gardens;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break

      case 11:
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break

      case 12:
        randomCard = remodel;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break

      case 13:
        randomCard = smithy;
         checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break

      case 14:
        randomCard = village;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break

      case 15:
        randomCard = baron;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break

      case 16:
        randomCard = great_hall;
         checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break

      case 17:
        randomCard = minion;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break

      case 18:
        randomCard = steward;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break

      case 19:
        randomCard = tribute;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break

      case 20:
        randomCard = ambassador;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break

      case 21:
        randomCard = cutpurse;
        for(y = 0; y < 10; y++){
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break

      case 22:
        randomCard = embargo;
         checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break

      case 23:
        randomCard = outpost;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break

      case 24:
        randomCard = salvager;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break

      case 25:
        randomCard = sea_hag;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break

      case 26:
        randomCard = treasure_map;
         checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break

      default:
        break;
    }
  }
}