#include "aes.h"

void key_schedule(unsigned char cipher_key[4][4],
				  unsigned char round_key[4][44],
				  unsigned char a_rot_word[4])				  
{
	unsigned char temp[4];

	const unsigned char r_con[4][10] = {
									{0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x30, 0x80, 0x1b, 0x36},
									{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
									{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
									{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
								};	
	
	for(int i = 0; i < 4; i++)															//Makes the first 4 columns of the round key the same as the cipher key
	{
		for(int j = 0; j < 4; j++)
		{
			round_key[i][j] = cipher_key[i][j];
		}
	}
		
	for(int i = 4; i < 44; i++)															//Loop goes through all columns of the round key, starts at 4 to ignore the cipher values
	{	
		
		if(i % 4 == 0)																	//Checks if the current value is a multiple of 4
		{
			
			rot_word(round_key, a_rot_word);											//Rotates the column by 1 byte
			s_box(a_rot_word);															//Performs the s box on it
			
			for(int j = 0; j < 4; j++)													//XORs the rotated column with a column 4 places before in the round key and with the correstpoding column on the r_con (i.e round 1, r_con 1)
			{
				temp[j] = ( round_key[j][i-4] ^ a_rot_word[j] ^ r_con[j][(i/4)-1] );
			}
			
			for(int j = 0; j < 4; j++)													//Assigns the XOR's column to the round key
			{
				round_key[j][i] = temp[j];
			}
		}
			
		else																			//Checks if the value of the round key is not divisible evenly by 4
		{
			for(int j = 0; j < 4; j++)
			{
				round_key[j][i] = round_key[j][i - 4] ^ round_key[j][i - 1];			//XORs The round key 4 places ago with the round key from 1 place ago
			}
		}
	}
}
