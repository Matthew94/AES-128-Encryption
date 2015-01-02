#include "aes_key.hpp"

std::array<std::array<unsigned char, 44>, 4> key_schedule(
    std::array<std::array<unsigned char, 4>, 4> cipher_key
)
{
    //Array to hold all permutations of the round key
	std::array<std::array<unsigned char, 44>, 4> round_key;

    //Used to hold the next generated column of the round key
	std::array<unsigned char, 4> temp;

    //Makes the first 4 columns of the round key the same as the cipher key
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			round_key[i][j] = cipher_key[i][j];
		}
	}

    // Loop goes through all columns of the round key
    // starts at 4 to ignore the cipher values
	for(int i = 4; i < 44; i++)
	{
		if(i % 4 == 0)
		{
			auto a_rot_word = rot_word(round_key, i - 4);

            //Performs the s box on it
			a_rot_word = sub_bytes_k(a_rot_word);

            //XORs rotated column with column 4 places
            //before in round key with part of the r_con
			for(int j = 0; j < 4; j++)
			{
				temp[j] = (round_key[j][i - 4] ^ a_rot_word[j] ^ aes_const::R_CON[j][(i / 4) - 1]);
			}

            //Assigns the XOR'd column to the round key
			for(int j = 0; j < 4; j++)
			{
				round_key[j][i] = temp[j];
			}
		}
		else
		{
			for(int j = 0; j < 4; j++)
			{
			    //XORs the round key 4 places back with the round key from 1 place back
				round_key[j][i] = round_key[j][i - 4] ^ round_key[j][i - 1];
			}
		}
	}

	return round_key;
}

std::array<unsigned char, 4> rot_word(
    std::array<std::array<unsigned char, 44>, 4> round_key,
    int i
)
{
    std::array<unsigned char, 4> rot_word = {
        {
            round_key[1][3 + i],
            round_key[2][3 + i],
            round_key[3][3 + i],
            round_key[0][3 + i]
        }
    };

    return rot_word;
}

std::array<unsigned char, 4> sub_bytes_k(
    std::array<unsigned char, 4> a_rot_word
)
{
	for(int i = 0; i < 4; i++)
	{
        //Character to hold a character from the array
        const unsigned char n = a_rot_word[i];

        //n is AND'd with 15 in hex to isolate the first digit
        // n is bit shifted 4 places across to get the second digit
		a_rot_word[i] = aes_const::S_BOX[n >> 4][n & 0xF];
	}

	return a_rot_word;
}

