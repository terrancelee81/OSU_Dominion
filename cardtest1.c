#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "rngs.h"

#define DEBUG 0
#define NOISY_TEST 1

int main(int argc, char const *argv[]){
	
	struct gameState game;

	int i, j;
	int testValue;
	int testSwitch = 0;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	
	int myCard = outpost; //Set the flag for the current card
	int myChoice1 = -1;
	int myChoice2 = -1;
	int myChoice3 = -1;
	int myHandPos = -1;
	int* myBonus = 0; //tracks coins gained from actions

	
	for(i = MIN_PLAYERS; i < (MAX_PLAYERS + 1); i++){
		for (j = 1; j < 10; j++){ //initializes 9 game states for each possible number of players
			initializeGame(i, k, j, &game); //initialize a game with 2 to 4 and  
										//	seeds between 1-9 as specified in rngs.c.
			

			/* ---- Card Specific Tests ---- */
			
			/* ---- outpostCard ---- */
			int testOutpostCardPlayed = game.outpostPlayed; //Get baseline outpostcard flag

			//choices1-3, and handPos currently reflect the values hard coded from playDom
			testValue = cardEffect(myCard, myChoice1, myChoice2, myChoice3, &game, myHandPos, myBonus);
			
			if (testValue == -1){ //Ensure cardEffect routes the card correctly.
				printf("Error: cardTest1 [cardEffect()] returned a -1.\n");
				testSwitch = 1;
			} else if (testValue == 0){ //Successful card played
				if (game.outpostPlayed == (testOutpostCardPlayed + 1)){ //outpostCard works correctly.
					//printf("outpostcard successfully played.");
				}
			}
			/* ----------------------------------*/
		}
	}
	
	if (testSwitch == 0)
		printf("cardtest1 [outpostCard()] shows no errors.\n");


	return 0;
}