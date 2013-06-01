#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <time.h>


int random_number(int min_num, int max_num);
void testme();
char inputChar();
char* inputString();


int main(int argc, char const *argv[])
{
	
	testme();
	return 0;
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
    srand(time(NULL));
    result = (rand()%(hi_num-low_num))+low_num;
    return result;
}


char inputChar(){
	int a = random_number(32, 126); //Ascii numbers
	char c;


	while (1){
		if (a >=48 && a<=57){
			a = random_number(32, 126);
		} else {
			break;
		}
	}
	c = (char) a;
	return c;
}

char * inputString(){
	int i;
	int strLength = random_number(1, 6);
	static char ch[10];
	
	int a;
	for (i = 0; i < strLength; i++){
		a = random_number(97, 122);
		ch[i] = (char) a;
	}	
	ch[i+1] = '\0';
	return ch;
}


void testme(){

	char *s;
	char c;
	int state = 0;

	while (1){

		c = inputChar();
		s = inputString();
		if (c == '[' && state == 0) state = 1;
		if (c == '(' && state == 1) state = 2;
		if (c == '{' && state == 2) state = 3;
		if (c == ' ' && state == 3) state = 4;
		if (c == 'a' && state == 4) state = 5;
		if (c == 'x' && state == 5) state = 6;
		if (c == '}' && state == 6) state = 7;
		if (c == ')' && state == 7) state = 8;
		if (c == ']' && state == 8) state = 9;

		if (s[0] == 'r' && s[1] == 'e' && s[2] == 's' && s[3] == 'e' && s[4] == 't' && s[5] == 0 && state == 9){
			printf("error ");
			exit(200);
		}
	}
}
