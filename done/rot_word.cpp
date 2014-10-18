//Rot word function
#include "aes.h"

void rot_word(unsigned char round_key[4][44], unsigned char rot_word[4])
{
	unsigned char temp;
		
	temp = round_key[0][3];
	
	rot_word[0] = round_key[1][3];
	rot_word[1] = round_key[2][3];
	rot_word[2] = round_key[3][3];
	rot_word[3] = temp;
}
