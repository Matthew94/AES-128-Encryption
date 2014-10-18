//Write from file to array

#include <iostream>
#include <fstream>
#include "aes.h"

void write_to_array(std::ifstream &infile, unsigned char state[4][4])
{
	
	for(int i = 0; i < 4; i++)
	{
		for( int j = 0; j < 4; j++)
		{
			state[j][i] = infile.get();
			
		}
	}
}
