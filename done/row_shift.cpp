//Row shift function
#include "aes.h"

void row_shift(char state[4][4])
{
	char blank[4];													//Array to hold template
	
	for(int i = 0; i < 4; i++)										//Copy a template of a row into an array
	{
		blank[i] = state[0][i];
	}
	
	state[1][0] = blank[1];											//Shifts values in row 1, 1 byte over
	state[1][1] = blank[2];
	state[1][2] = blank[3];
	state[1][3] = blank[0];
	
	state[2][0] = blank[2];											//Shifts values in row 2, 2 bytes over
	state[2][1] = blank[3];
	state[2][2] = blank[0];
	state[2][3] = blank[1];
	
	state[3][0] = blank[3];											//Shifts values in row 3, 3 bytes over
	state[3][1] = blank[0];
	state[3][2] = blank[1];
	state[3][3] = blank[2];	
}	

	