#define _CRT_SECURE_NO_DEPRECATE
#include "dominion.h"
#include "dominion_helpers.h"
#include "testdominion.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "rngs.h"


#define DEBUG 0
#define NOISY_TEST 1

int random_number(int min_num, int max_num);
void checkCard(int* randomCard, int* deckHasCard, int* deckCount, int k[]);
void getRandomDeck(int k[]);
char* getCardName(int* card);
void resetCardState(struct cardState* cards);
int checkDeck(int k[], int* myCard);
void getDeckContents(struct cardState* cards, struct gameState* game, int* money);
void performBuys(struct cardState* cards, struct gameState* game, int* money, int* playerTurn, FILE *fp);
void performActions(struct cardState* cards, struct gameState* game, int* money, int* playerTurn, FILE *fp);
void printGame(struct gameState *game, FILE *fp);



int main(int argc, char const *argv[])
{
	//initialize gamestart variables
  int i, l, q;
	int money;

	struct gameState game;
    struct cardState cards;
	int k[10];
	int gameNum;
	int playerTurn;
	 
	//Use file io to write gameResults.out
	FILE *fp;
	fp = fopen("gameResults.out", "w");
	gameNum = 0;
  srand(time(NULL)); //seed random generator


	//randomize the number of players (2-4) and the starting cards with the random seed
	for (q = 0; q < random_number(0, 5); q ++){ //Loops number of games a random number of times < 5
    
    //randomizer functions for cards chosen for game and initializer
    printf("Getting Random Cards for game [%d]\n", gameNum);
    getRandomDeck(k);
    printf("Initializing game [%d]\n", gameNum);
		initializeGame(random_number(2,4), k, random_number(1, 9), &game);

    //Number of player randomly chosen written to console
    printf("[%d] players randomly assigned\n", game.numPlayers);
    fprintf(fp, "Game %d has %d players.\n", gameNum, game.numPlayers);
    //Cards chosen randomly written to console
    printf("Cards Randomly Chosen[");
    fprintf(fp, "Using Cards: ");
    for (l = 0; l < 10; l++){
      printf("%s, ", getCardName(&k[l]));
      fprintf(fp, "%s ", getCardName(&k[l]));
    }
    printf("]\n");
		//Code to play dominion contained within this block.
		printf("Starting game [%d]\n", gameNum);
    fprintf(fp,"Begin game %d\n\n", gameNum);

		//initialize game starting values
		money = 0;
    resetCardState(&cards);
    

		//The loop that the main game runs under
		while (!isGameOver(&game)){
			//reset turn dependant variables
      resetCardState(&cards);
      
      //Play Dominion 
      playerTurn = whoseTurn(&game);
      printGame(&game, fp);
      fprintf(fp,"Player %d turn.\n", playerTurn +1);
      
      //Play any action cards the player has
      fprintf(fp, "Beginning Player %d Action Phase\n", playerTurn + 1);
      performActions(&cards, &game, &money, &playerTurn, fp);
      fprintf(fp, "End Player %d Actions\n", playerTurn + 1);

      /*
      //Count up money and use cards
      money = 0;
      j = 0;
      while(j < numHandCards(&game)){
        if (handCard(j, &game) == copper){
          playCard(j, -1, -1, -1, &game);
          money++;
        }
        else if(handCard(j, &game) == silver){
          playCard(j, -1, -1, -1, &game);
          money = money + 2;
        }
        else if(handCard(j, &game) == gold){
          playCard(j, -1, -1, -1, &game);
          money = money + 3;
        }
        j++;
      }
      */

      fprintf(fp, "Player %d has %d money.\n", playerTurn +1, game.coins);
      // Buys Phase
      fprintf(fp, "Entering player %d Buy Phase.\n", playerTurn + 1);
      performBuys(&cards, &game, &money, &playerTurn, fp);
      fprintf(fp, "End Player %d Buys.\n", playerTurn +1);

      printf("Player[%d]: end turn.\n", playerTurn +1);
      fprintf(fp, "End Player %d turn.\n\n", playerTurn +1);
      //printf("Player[%d]: end turn. Money[$%d], Actions[%d], Buys[%d]\n", playerTurn + 1, money, game.numActions, game.numBuys);
      endTurn(&game); 
  			
    }

      // Print Game Score  
      printf ("Finished game [%d].\n", gameNum);

      for (i = 0; i < game.numPlayers; i++){
        printf ("Player [%d]: Score [%d]\n", (i + 1), scoreFor(i, &game));
        fprintf(fp, "Player [%d]: Score [%d]\n", (i + 1), scoreFor(i, &game));
      }
      printGame(&game, fp);
      printf("\n\n\n\n");
      fprintf(fp, "\n\n\n\n");
      gameNum = gameNum + 1;
	} 

  fclose(fp);
	return 0;
}

//Helper functions

void performBuys(struct cardState* cards, struct gameState* game, int* money, int* playerTurn, FILE *fp){
    
  int randNum;
  int buyTest;
  int iteratorBreak;
  *money = game->coins;
  buyTest = 1;
  iteratorBreak = 0;

  while (game->numBuys){
	  if (game->coins == 0) break;
  //if money is greater than 8, always purchase a province.
  if (*money >= 8){
    buyTest = buyCard(province, game);
    if (buyTest == 0){
		*money = game->coins;
      printf("\tPlayer[%d]: province purchased. Money[$%d]\n", *playerTurn + 1, *money);
      fprintf(fp, "\tPurchased province. Money after purchase: [%d]", *money);
      buyTest = 1;
    }
  }
 
  //Gold and Adventurer both cost 6
  if (*money >= 6){
    randNum = random_number(1, 2);
    if ((randNum == 1) && (game->supplyCount[adventurer] > 0)){
      buyTest = buyCard(adventurer, game);
      if (buyTest == 0){
		*money = game->coins;
		printf("Player[%d]: Adventurer purchased.\n", *playerTurn +1);
        fprintf(fp, "\tAdventurer purchased. Money after purchase: [%d]\n", *money);
        buyTest = 1;
      }
	}
    else {
		buyTest = buyCard(gold, game);
		if (buyTest == 0){
			*money = game->coins;
			printf("Player[%d]: Gold card purchased.\n", *playerTurn +1);
			fprintf(fp, "\tGold Card purchased. Money after purchase: [%d]\n", *money);
			buyTest = 1;
		}
	}
   }
  
  
  // There are 6 cards that cost 5 gold (Minion, Mine, Duchy, Council Room, Tribute, Outpost)
  if (*money == 5){
    randNum = random_number(1, 6);
    if((randNum == 1) && (game->supplyCount[duchy]) > 0){
        buyTest = buyCard(duchy, game);
		*money = game->coins;
        printf("Player[%d]: Duchy card purchased.\n", *playerTurn +1);
        fprintf(fp, "\tDuchy purchased. Money after purchase: [%d]\n", *money);
        buyTest = 1;
    }
    else if((randNum == 2) && (game->supplyCount[minion]) > 0){
      buyTest = buyCard(minion, game);
	  *money = game->coins;
      printf("Player[%d]: Minion card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tMinion purchased. Money after purchase: [%d]\n", *money);
      buyTest = 1;
    }
    else if((randNum == 3) && (game->supplyCount[council_room]) > 0){
      buyTest = buyCard(council_room, game);
	  *money = game->coins;
      printf("Player[%d]: Council Room card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tCouncil Room purchased. Money after purchase: [%d]\n", *money);
      buyTest = 1;
    }
    else if((randNum == 4) && (game->supplyCount[tribute]) > 0){
      buyTest = buyCard(tribute, game);
	  *money = game->coins;
      printf("Player[%d]: Tribute card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tTribute purchased. Money after purchase: [%d]\n", *money);
      buyTest = 1;
    }
    else if((randNum == 5) && (game->supplyCount[mine]) > 0){
      buyTest = buyCard(mine, game);
	  *money = game->coins;
      printf("Player[%d]: Mine card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tMine purchased. Money after purchase: [%d]\n", *money);
      buyTest = 1;
    }
    else if((randNum == 6) && (game->supplyCount[council_room]) > 0){
      buyTest = buyCard(outpost, game);
	  *money = game->coins;
      printf("Player[%d]: Outpost card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tOutpost purchased. Money after purchase: [%d]\n", *money);
      buyTest = 1;
    }
    else {
      //No card purchased.
    }
  }

  // There are nine cards that cost 4 coins
  // (Feast, Gardens, Remodel, Smithy, Baron, Cutpurse, Salvager, Sea Hag, Treasure Map)
  if (*money == 4) {
        randNum = random_number(1, 9);
    if((randNum == 1) && (game->supplyCount[feast]) > 0){
        buyTest = buyCard(feast, game);
		*money = game->coins;
        printf("Player[%d]: Feast card purchased.\n", *playerTurn +1);
        fprintf(fp, "\tFeast purchased. Money after purchase: [%d]\n", *money);
        buyTest = 1;
    }
    else if((randNum == 2) && (game->supplyCount[gardens]) > 0){
      buyTest = buyCard(gardens, game);
	  *money = game->coins;
      printf("Player[%d]: Gardens card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tGardens purchased. Money after purchase: [%d]\n", *money);
      buyTest = 1;
    }
    else if((randNum == 3) && (game->supplyCount[remodel]) > 0){
      buyTest = buyCard(remodel, game);
	  *money = game->coins;
      printf("Player[%d]: Remodel card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tRemodel purchased. Money after purchase: [%d]\n", *money);
      buyTest = 1;
    }
    else if((randNum == 4) && (game->supplyCount[smithy]) > 0){
      buyTest = buyCard(smithy, game);
	  *money = game->coins;
      printf("Player[%d]: Smithy card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tSmithy purchased. Money after purchase: [%d]\n", *money);
      buyTest = 1;
    }
    else if((randNum == 5) && (game->supplyCount[baron]) > 0){
      buyTest = buyCard(baron, game);
	  *money = game->coins;
      printf("Player[%d]: Baron card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tBaron purchased. Money after purchase: [%d]\n", *money);
      buyTest = 1;
    }
    else if((randNum == 6) && (game->supplyCount[cutpurse]) > 0){
      buyTest = buyCard(cutpurse, game);
	  *money = game->coins;
      printf("Player[%d]: Cutpurse card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tCutpurse purchased. Money after purchase: [%d]\n", *money);
      buyTest = 1;
    }
    else if((randNum == 7) && (game->supplyCount[salvager]) > 0){
      buyTest = buyCard(salvager, game);
	  *money = game->coins;
      printf("Player[%d]: Salvager card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tSalvager purchased. Money after purchase: [%d]\n", *money);
      buyTest = 1;
    }
    else if((randNum == 8) && (game->supplyCount[sea_hag]) > 0){
      buyTest = buyCard(sea_hag, game);
	  *money = game->coins;
      printf("Player[%d]: Sea Hag card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tSea Hag purchased. Money after purchase: [%d]\n", *money);
      buyTest = 1;
    }
    else if((randNum == 9) && (game->supplyCount[treasure_map]) > 0){
      buyTest = buyCard(treasure_map, game);
	  *money = game->coins;
      printf("Player[%d]: Treasure Map card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tTreasure Map purchased. Money after purchase: [%d]\n", *money);
      buyTest = 1;
    }
    else {
      //No purchase at this level
    }
  }


  //There are 5 cards that cost 3 gold
  //(Silver, Village, Great Hall, Steward, Ambassador)
  if (*money == 3) {
    randNum = random_number(1, 5);
    if((randNum == 1) && (game->supplyCount[silver]) > 0){
        buyTest = buyCard(silver, game);
		*money = game->coins;
        printf("Player[%d]: Silver card purchased.\n", *playerTurn +1);
        fprintf(fp, "\tSilver purchased. Money after purchase: [%d]\n", *money);
        buyTest = 1;
    }
    else if((randNum == 2) && (game->supplyCount[village]) > 0){
      buyTest = buyCard(village, game);
	  *money = game->coins;
      printf("Player[%d]: Village card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tVillage purchased. Money after purchase: [%d]\n", *money);
      buyTest = 1;
    }
    else if((randNum == 3) && (game->supplyCount[great_hall]) > 0){
      buyTest = buyCard(great_hall, game);
	  *money = game->coins;
      printf("Player[%d]: Great Hall card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tGreat Hall purchased. Money after purchase: [%d]\n", *money);
      buyTest = 1;
    }
    else if((randNum == 4) && (game->supplyCount[steward]) > 0){
      buyTest = buyCard(steward, game);
	  *money = game->coins;
      printf("Player[%d]: Steward card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tSteward purchased. Money after purchase: [%d]\n", *money);
      buyTest = 1;
    }    
    else if((randNum == 5) && (game->supplyCount[ambassador]) > 0){
      buyTest = buyCard(ambassador, game);
	  *money = game->coins;
      printf("Player[%d]: Ambassador card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tAmbassador purchased. Money after purchase: [%d]\n", *money);
      buyTest = 1;
    }
    else {
      // No purchases at this level
    }
  }

  //There are 2 cards that cost 3 gold
  //(Embargo, Estate)
  if (*money == 2) {
    randNum = random_number(1, 2);
    if((randNum == 1) && (game->supplyCount[embargo]) > 0){
        buyTest = buyCard(embargo, game);
		*money = game->coins;
        printf("Player[%d]: Embargo card purchased.\n", *playerTurn +1);
        fprintf(fp, "\tEmbargo purchased. Money after purchase: [%d]\n", *money);
        buyTest = 1;
    }
    else if((randNum == 2) && (game->supplyCount[estate]) > 0){
      buyTest = buyCard(estate, game);
	  *money = game->coins;
      printf("Player[%d]: Estate card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tEstate purchased. Money after purchase: [%d]\n", *money);
      buyTest = 1;
    }
    else {
      //No purchases at this level
    }
  }

  //If money is 1 or 0, but the player still has buys...  
  if (*money < 2){
    buyTest = buyCard(copper, game);
    if (buyTest == 0){
		*money = game->coins;
      printf("Player[%d]: Copper card purchased.\n", *playerTurn +1);
      fprintf(fp, "\tCopper purchased. Money after purchase: [%d]\n", *money);
    }
  }

  if (iteratorBreak > 10) return; //Break out of endless loops
  iteratorBreak++;

 }
}


void performActions(struct cardState* cards, struct gameState* game, int* money, int* playerTurn, FILE *fp){
  //look at every card in the current players deck and up their money
  //set any action card in hand equal to an integer value  indicating 
  //the position of the card in the current hand
  int actionTest;
  getDeckContents(cards, game, money);
  actionTest = 1;
  //Currently, I only implement the cards that don't require choices. This may change.
  while (game->numActions){
	
  //Smithy Card
  //If the player has the smithy then play it... always
  if (cards->smithyPos != -1){
	  if (playCard(cards->smithyPos, -1, -1, -1, game) != -1){
		// -1 Action, +3 Cards
		fprintf(fp, "\tSmithy Played\n");
		actionTest = 0;
		//Look at the deck again
		cards->smithyPos = -1;
		getDeckContents(cards, game, money);
	} 
	else {
		fprintf(fp, "\tError: playCard(Smithy) returned -1.\n");
		printf("\tError: playCard(Smithy) returned -1.\n");
		return;
	}
  } // End Smithy

  //Adventurer Card
  if (cards->adventurerPos != -1){
    printf("Player[%d]: Adventurer played from position %d\n", *playerTurn + 1, cards->adventurerPos);
	if (playCard(cards->adventurerPos, -1, -1, -1, game) != -1){
		// -1 Action, +3 Cards
		fprintf(fp, "\tAdventurer Played\n");
		actionTest = 0;
		//Look at the deck again
		cards->adventurerPos = -1;
		getDeckContents(cards, game, money);
	} 
	else {
		fprintf(fp, "\tError: playCard(Adventurer) returned -1.\n");
		printf("\tError: playCard(Adventurer) returned -1.\n");
		return;
	}
  } // End Adventurer

  //End Council Room
  if (cards->council_roomPos != -1){
    printf("Player[%d]: Council Room played from position %d\n", *playerTurn + 1, cards->council_roomPos);
	if (playCard(cards->council_roomPos, -1, -1, -1, game) != -1){
		// -1 Action, +3 Cards
		fprintf(fp, "\tCouncil Room Played\n");
		actionTest = 0;
		//Look at the deck again
		cards->council_roomPos = -1;
		getDeckContents(cards, game, money);
	} 
	else {
		fprintf(fp, "\tError: playCard(Council Room) returned -1.\n");
		printf("\tError: playCard(Council Room) returned -1.\n");
		return;
	}
  } // End Council Room

  //Village Card
  if (cards->villagePos != -1){
    printf("Player[%d]: Village played from position %d\n", *playerTurn + 1, cards->villagePos);
	if (playCard(cards->villagePos, -1, -1, -1, game) != -1){
		// -1 Action, +3 Cards
		fprintf(fp, "\tVillage Played\n");
		actionTest = 0;
		//Look at the deck again
		cards->villagePos = -1;
		getDeckContents(cards, game, money);
	} 
	else {
		fprintf(fp, "\tError: playCard(Village) returned -1.\n");
		printf("\tError: playCard(Village) returned -1.\n");
		return;
	}
  } // End Village 

  //Great Hall
  if (cards->great_hallPos != -1){
    printf("Player[%d]: Great Hall played from position %d\n", *playerTurn + 1, cards->great_hallPos);
	if (playCard(cards->great_hallPos, -1, -1, -1, game) != -1){
		// -1 Action, +3 Cards
		fprintf(fp, "\tGreat Hall Played\n");
		actionTest = 0;
		//Look at the deck again
		cards->great_hallPos = -1;
		getDeckContents(cards, game, money);
	} 
	else {
		fprintf(fp, "\tError: playCard(Great Hall) returned -1.\n");
		printf("\tError: playCard(Great Hall) returned -1.\n");
		return;
	}
  } // End Great Hall

    //Tribute
  if (cards->tributePos != -1){
    printf("Player[%d]: Tribute played from position %d\n", *playerTurn + 1, cards->tributePos);
	if (playCard(cards->tributePos, -1, -1, -1, game) != -1){
		// -1 Action, +3 Cards
		fprintf(fp, "\tTribute Played\n");
		actionTest = 0;
		//Look at the deck again
		cards->tributePos = -1;
		getDeckContents(cards, game, money);
	} 
	else {
		fprintf(fp, "\tError: playCard(Tribute) returned -1.\n");
		printf("\tError: playCard(Tribute) returned -1.\n");
		return;
	}
  } // End Tribute

    //Cut Purse
  if (cards->cutpursePos != -1){
    printf("Player[%d]: Cutpurse played from position %d\n", *playerTurn + 1, cards->cutpursePos);
    if (playCard(cards->cutpursePos, -1, -1, -1, game) != -1){
		// -1 Action, +3 Cards
		fprintf(fp, "\tCutpurse Played\n");
		actionTest = 0;
		//Look at the deck again
		cards->cutpursePos = -1;
		getDeckContents(cards, game, money);
	} 
	else {
		fprintf(fp, "\tError: playCard(Cutpurse) returned -1.\n");
		printf("\tError: playCard(Cutpurse) returned -1.\n");
		return;
	}
  } // End Cutpurse

    //Village Card
  if (cards->outpostPos != -1){
    printf("Player[%d]: Outpost played from position %d\n", *playerTurn + 1, cards->outpostPos);
    if (playCard(cards->outpostPos, -1, -1, -1, game) != -1){
		// -1 Action, +3 Cards
		fprintf(fp, "\tOutpost Played\n");
		actionTest = 0;
		//Look at the deck again
		cards->outpostPos = -1;
		getDeckContents(cards, game, money);
	} 
	else {
		fprintf(fp, "\tError: playCard(Outpost) returned -1.\n");
		printf("\tError: playCard(Outpost) returned -1.\n");
		return;
	}
  } // End Outpost

    //Sea Hag
  if (cards->sea_hagPos != -1){
    printf("Player[%d]: Sea Hag played from position %d\n", *playerTurn + 1, cards->sea_hagPos);
    if (playCard(cards->sea_hagPos, -1, -1, -1, game) != -1){
		// -1 Action, +3 Cards
		fprintf(fp, "\tSea Hag Played\n");
		actionTest = 0;
		//Look at the deck again
		cards->sea_hagPos = -1;
		getDeckContents(cards, game, money);
	} 
	else {
		fprintf(fp, "\tError: playCard(Sea Hag) returned -1.\n");
		printf("\tError: playCard(Sea Hag) returned -1.\n");
		return;
	}
  } // End Sea Hag

    //Treasure Map
  if (cards->treasure_mapPos != -1){
    printf("Player[%d]: Treasure Map played from position %d\n", *playerTurn + 1, cards->treasure_mapPos);
	if (playCard(cards->treasure_mapPos, -1, -1, -1, game) != -1){
		// -1 Action, +3 Cards
		fprintf(fp, "\tTreasure Map Played\n");
		actionTest = 0;
		//Look at the deck again
		cards->treasure_mapPos = -1;
		getDeckContents(cards, game, money);
	} 
	else {
		fprintf(fp, "\tError: playCard(Treasure Map) returned -1.\n");
		printf("\tError: playCard(Treasure Map) returned -1.\n");
		return;
	}
  } // End Treasure Map

  //If no action is taken...
  if (actionTest == 1){
    printf("Player[%d] No Action taken this turn.\n", *playerTurn +1);
    fprintf(fp, "\tPlayer %d No Action taken this turn.\n", *playerTurn +1);
	return;
  }
  else if (actionTest <= 1){
	  if (actionTest <= -1){
		  fprintf(fp, "Player[%d] No More Action Cards to play this turn.\n", *playerTurn +1);
		  printf("Player[%d] No More Cards to play this turn.\n", *playerTurn +1);
		  return;
	  }
	  actionTest--;
  }
  else if (actionTest == 0){
	  if (game->numActions > 10){
		printf("\t\tAbnormally High Actions for this turn [%d], examine last card played.\n", game->numActions);
		fprintf(fp, "\t\tAbnormally High Actions for this turn [%d], examine last card played.\n", game->numActions);
		return;
	  }
  }
 }
}


int random_number(int min_num, int max_num){
    int result=0,low_num=0,hi_num=0;
    if(min_num<max_num)
    {
        low_num=min_num;
        hi_num=max_num+1; // this is done to include max_num in output.
    }else{
        low_num=max_num+1;// this is done to include max_num in output.
        hi_num=min_num;
    }
    
    result = (rand()%(hi_num-low_num))+low_num;
    return result;
}

void getDeckContents(struct cardState* cards, struct gameState* game, int* money){
    int i;
    *money = 0;

    for (i = 0; i < numHandCards(game); i++){
      if (handCard(i, game) == copper)
        *money = *money + 1;
      if (handCard(i, game) == silver)
        *money = *money + 2;
      if (handCard(i, game) == gold)
        *money = *money + 3;
      if (handCard(i, game) == smithy)
        cards->smithyPos = i;
      if (handCard(i, game) == adventurer)
        cards->adventurerPos = i;
      if (handCard(i, game) == council_room)
        cards->council_roomPos = i;  
      if (handCard(i, game) == feast)
        cards->feastPos = i;   
      if (handCard(i, game) == gardens)
        cards->gardensPos = i;   
      if (handCard(i, game) == mine)
        cards->minePos = i;  
      if (handCard(i, game) == remodel)
        cards->remodelPos = i;
      if (handCard(i, game) == village)
        cards->villagePos = i;  
      if (handCard(i, game) == baron)
        cards->baronPos = i; 
      if (handCard(i, game) == great_hall)
        cards->great_hallPos = i; 
      if (handCard(i, game) == minion)
        cards->minionPos = i;  
      if (handCard(i, game) == steward)
        cards->stewardPos = i; 
      if (handCard(i, game) == tribute)
        cards->tributePos = i; 
      if (handCard(i, game) == ambassador)
        cards->ambassadorPos = i; 
      if (handCard(i, game) == cutpurse)
        cards->cutpursePos = i; 
      if (handCard(i, game) == embargo)
        cards->embargoPos = i; 
      if (handCard(i, game) == outpost)
        cards->outpostPos = i; 
      if (handCard(i, game) == salvager)
        cards->salvagerPos = i; 
      if (handCard(i, game) == sea_hag)
        cards->sea_hagPos = i;
      if (handCard(i, game) == treasure_map)
        cards->treasure_mapPos = i;  
    }
}

//Returns 1 if card is in current deck, 0 if it is not.
int checkDeck(int k[], int* myCard){
  int i;
  for (i = 0; i < 10; i++){
    if (k[i] == *myCard){
      return 1;
    }
  }
  return 0;
}

char* getCardName(int* card){
  int cardNumber = *card;
  switch( cardNumber ) 
    {
    case curse:
      return "Curse";
    case estate:
      return "Estate";
    case duchy:
      return "Duchy";
    case province:
      return "Province";
    case copper:
      return "Copper";
    case silver:
      return "Silver";
    case gold:
      return "Gold";
    case adventurer:
      return "Adventurer";
    case council_room:
      return "Council Room";
    case feast:
      return "Feast";
    case gardens:
      return "Gardens";
    case mine:
      return "Mine";
    case remodel:
      return "Remodel";
    case smithy:
      return "Smithy";
    case village:
      return "Village";
    case baron:
      return "Baron";
    case great_hall:
      return "Great Hall";
    case minion:
      return "Minion";
    case steward:
      return "Steward";
    case tribute:
      return "Tribute";
    case ambassador:
      return "Ambassador";
    case cutpurse:
      return "Cutpurse";
    case embargo: 
      return "Embargo";
    case outpost:
      return "Outpost";
    case salvager:
      return "Salvager";
    case sea_hag:
      return "Sea Hag";
    case treasure_map:
      return "Treasure Map";
    default:
      return 0;
    }
}

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

void resetCardState(struct cardState* cards){
    cards->smithyPos = -1;
    cards->adventurerPos = -1;
    cards->embargoPos = -1;
    cards->villagePos = -1;
    cards->great_hallPos = -1;
    cards->stewardPos = -1;
    cards->ambassadorPos = -1;
    cards->feastPos = -1;
    cards->gardensPos = -1;
    cards->remodelPos = -1;
    cards->baronPos = -1;
    cards->cutpursePos = -1;
    cards->salvagerPos = -1;
    cards->sea_hagPos = -1;
    cards->treasure_mapPos = -1;
    cards->council_roomPos = -1;
    cards->minePos = -1;
    cards->minionPos = -1;
    cards->tributePos = -1;
    cards->outpostPos = -1;

    cards->numSmithies = 0;
    cards->numAdventurers = 0;
    cards->numEmbargo = 0;
    cards->numVillage = 0;
    cards->numGreat_Hall = 0;
    cards->numSteward = 0;
    cards->numAmbassador = 0;
    cards->numFeast = 0;
    cards->numGarden = 0;
    cards->numRemodel = 0;
    cards->numBaron = 0;
    cards->numCutpurse = 0;
    cards->numSalvager = 0;
    cards->numSea_Hag = 0;
    cards->numTreasure_Map = 0;
    cards->numCouncil_Room = 0;
    cards->numMine = 0;
    cards->numMinion = 0; 
    cards->numTribute = 0; 
    cards->numOutpost = 0; 
}

void printGame(struct gameState *game, FILE *fp){
  int i, j;
  int gameHand;
  int gameDeck;
  int gameDiscard;
  fprintf(fp,"\n\n\t****Printing Game State****\n");
  for (i = 0; i < game->numPlayers; i++){
    fprintf(fp, "\tPlayer %d Hand: [", i + 1);
    for (j = 0; j < game->handCount[i]; j++){
		gameHand = game->hand[i][j];
      fprintf(fp, " %s", getCardName(&gameHand));
    }
    fprintf(fp, "]\n");
    fprintf(fp, "\t\tDeck: [");
    for (j = 0; j < game->deckCount[i]; j++){
		gameDeck = game->deck[i][j];
      fprintf(fp, " %s", getCardName(&gameDeck));
    }
    fprintf(fp, "]\n");
    fprintf(fp, "\t\tDiscard: [");
    for (j = 0; j < game->discardCount[i]; j++){
		gameDiscard = game->discard[i][j];
		fprintf(fp, " %s", getCardName(&gameDiscard));
    }
    fprintf(fp, "]\n\n");
  }
  fprintf(fp, "\t***************************\n\n");
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
      break;

      case 10:
        randomCard = gardens;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      case 11:
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      case 12:
        randomCard = remodel;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      case 13:
        randomCard = smithy;
         checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      case 14:
        randomCard = village;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      case 15:
        randomCard = baron;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      case 16:
        randomCard = great_hall;
         checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      case 17:
        randomCard = minion;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      case 18:
        randomCard = steward;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      case 19:
        randomCard = tribute;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      case 20:
        randomCard = ambassador;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      case 21:
        randomCard = cutpurse;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      case 22:
        randomCard = embargo;
         checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      case 23:
        randomCard = outpost;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      case 24:
        randomCard = salvager;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      case 25:
        randomCard = sea_hag;
          checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      case 26:
        randomCard = treasure_map;
         checkCard(&randomCard, &deckHasCard, &deckCount, k);
      break;

      default:
        break;
    }
  }
}
