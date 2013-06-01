#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "rngs.h"



#define DEBUG 0
#define NOISY_TEST 1

int adventurerCard(int *drawntreasure, struct gameState *state, int *cardDrawn, int *currentPlayer, int *temphand, int *z);


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
            srand(time(NULL));
            result = (rand()%(hi_num-low_num))+low_num;
            return result;
}

int main(int argc, char const *argv[]){
	
	struct gameState game;

	int i, j;
	int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy};
	
	//adventurecard specific variables
	int drawntreasure;
	int currentplayer;
	int temphand[MAX_HAND]; //array of 500
	int z;
	int cardDrawn;
	int myCard;
	int tempcoins;


	for (i = 0; i < random_number(0, 100); i ++){ //Loops a random number of times <100
		initializeGame(random_number(2,4), k, random_number(1, 9), &game); //initializes a game with a random seed
															//between 1-9
			
		currentplayer = game.whoseTurn; //initialize the player

		for (j = 0; j < 10000; j++){ // run 10000 times with random variables in this game state
			drawntreasure = random_number(0, 10); //What happens if you start with drawntreasure
			//drawCard(currentplayer, &game); //What happens if you draw a card prematurely
			//z = random_number(0, 500); //If z is higher than 0, it produces a significant access error
			tempcoins = game.coins;

			myCard = adventurerCard(&drawntreasure, &game, &cardDrawn, &currentplayer, temphand, &z);
			if (tempcoins > game.coins)
				printf("Error: tempcoins [%d] game.coins[%d]\n", tempcoins, game.coins);
		}
	}

	return 0;
}