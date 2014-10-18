//Round key function
#include "aes.h"

void add_round_key(unsigned char state[4][4], unsigned char r_key[4][4])
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			state[i][j] = (state[i][j] ^ r_key[i][j]);
		}
	}
}	
