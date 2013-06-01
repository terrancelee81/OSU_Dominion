#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "rngs.h"

#define DEBUG 0
#define NOISY_TEST 1

int main(){

	struct gameState game;
	int i;
	int testSwitch = 0;

	for (i = 0; i < 4; i++){
		game.whoseTurn = i;
		if (whoseTurn(&game) != i){
			printf("Error: player %d turn produced an error.\n", i);
			testSwitch = 1;
		}
	}
	if (testSwitch == 0)
		printf("Unittest4 [whoseTurn()] shows no errors.\n");
}