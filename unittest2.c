#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "rngs.h"

#define DEBUG 0
#define NOISY_TEST 1

int main(int argc, char const *argv[])
{
	int k1 =1;
	int k2 =2;
	int k3 =3;
	int k4 =4;
	int k5 =5;
	int k6 =6;
	int k7 =7;
	int k8 =8;
	int k9 =9;
	int k10 =10;

	int testSwitch = 0;
	int* testKingdomCards;

	testKingdomCards = kingdomCards(k1, k2, k3, k4, k5, k6, k7, k8, k9, k10);

	if (testKingdomCards[0] != k1){
		printf("Card One Error Mismatch\n");
		testSwitch = 1;
	}
	if (testKingdomCards[1] != k2){
		printf("Card Two Error Mismatch\n");
		testSwitch = 1;
	}
	if (testKingdomCards[2] != k3){
		printf("Card Three Error Mismatch\n");
		testSwitch = 1;
	}
	if (testKingdomCards[3] != k4){
		printf("Card Four Error Mismatch\n");
		testSwitch = 1;
	}
	if (testKingdomCards[4] != k5){
		printf("Card Five Error Mismatch\n");
		testSwitch = 1;
	}
	if (testKingdomCards[5] != k6){
		printf("Card Six Error Mismatch\n");
		testSwitch = 1;
	}
	if (testKingdomCards[6] != k7){
		printf("Card Seven Error Mismatch\n");
		testSwitch = 1;
	}
	if (testKingdomCards[7] != k8){
		printf("Card Eight Error Mismatch\n");
		testSwitch = 1;
	}
	if (testKingdomCards[8] != k9){
		printf("Card Nine Error Mismatch\n");
		testSwitch = 1;
	}
	if (testKingdomCards[9] != k10){
		printf("Card Ten Error Mismatch\n");
		testSwitch = 1;
	}

	if(testSwitch == 1)
		printf("Error, at least one card failed.\n");
	else
		printf("Unittest2 [kingdomCards()] shows no errors.\n");

	return 0;
}