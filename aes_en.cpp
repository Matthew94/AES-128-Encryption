//Performs AES encryption on a state array

#include "aes_en.hpp"

void encrypt_state(
    std::array<std::array<unsigned char, 44>, 4> &round_key,
    std::array<std::array<unsigned char, 4>, 4> &state
)
{
        int count = 0;

        //Adds the round key to state
		count = add_round_key(state, round_key, count);

        //Does 9 rounds of the encryption
		for(int i = 0; i < 10; i++)
		{
			state = s_box(state);
			row_shift(state);
            if (i != 9)
            {
                state = mix_columns(state);
            }
			count = add_round_key(state, round_key, count);
		}
}

//XORs the round key with state
int add_round_key(
    std::array<std::array<unsigned char, 4>, 4> &state,
    std::array<std::array<unsigned char, 44>, 4> round_key,
    int count
)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
		    //XORs state with the currently used part of the round key
			state[i][j] = (state[i][j] ^ round_key[i][j + count]);
		}
	}

	return count + 4;
}

//S box function to be used in the encryption loop
std::array<std::array<unsigned char, 4>, 4> s_box(
    std::array<std::array<unsigned char, 4>, 4> state
)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
		    //Character to hold a character from the array
			const unsigned char n = state[i][j];

            //n is AND'd with 15 in hex to isolate the first digit, i.e the 5 from 15
			const int t = 0xF & n;

			// n is bit shifted 4 places across
			// This is to isolate the second digit from the character
			// i.e the 1 from 15
			state[i][j] = aes_const::S_BOX[n >> 4][t];
		}
	}

	return state;
}

//Shifts the values of the state array
void row_shift(
    std::array<std::array<unsigned char, 4>, 4> &state
)
{
    std::vector<int> index = {1, 2, 3, 0};
	unsigned char temp[4];

    //Copy a template of a row into an array
	for(int i = 1; i < 4; i++)
	{
	    for(int j = 0; j < 4; j++){
    	    temp[j] = state[i][j];
	    }

        //Shifts values in row n, n bytes over
        state[i][0] = temp[index[0]];
        state[i][1] = temp[index[1]];
        state[i][2] = temp[index[2]];
        state[i][3] = temp[index[3]];

        std::rotate(index.begin(), index.begin() + 1, index.end());
	}
}

//Implements mix columns function
std::array<std::array<unsigned char, 4>, 4> mix_columns(
    std::array<std::array<unsigned char, 4>, 4> state
)
{
    //4x4 array for holding result of mix columns function
	std::array<std::array<unsigned char, 4>, 4> output;

    // Loops through 4 values of the state array
    // the 4 numbers are XOD'd after the matrix multiplication is done
	for(int i = 0; i < 4; i++)
	{
		output[0][i] = ( (aes_const::TABLE_2[static_cast<int>(state[0][i])]) ^ (aes_const::TABLE_3[static_cast<int>(state[1][i])]) ^ state[2][i] ^ state[3][i] );
	}

	for(int i = 0; i < 4; i++)
	{
		output[1][i] = ( state[0][i] ^ (aes_const::TABLE_2[static_cast<int>(state[1][i])]) ^ (aes_const::TABLE_3[static_cast<int>(state[2][i])]) ^ state[3][i] );
	}

	for(int i = 0; i < 4; i++)
	{
		output[2][i] = ( state[0][i] ^ state[1][i] ^ (aes_const::TABLE_2[static_cast<int>(state[2][i])]) ^ (aes_const::TABLE_3[static_cast<int>(state[3][i])]) );
	}

	for(int i = 0; i < 4; i++)
	{
		output[3][i] = ( (aes_const::TABLE_3[static_cast<int>(state[0][i])]) ^ state[1][i] ^ state[2][i] ^ (aes_const::TABLE_2[static_cast<int>(state[3][i])]) );
	}

    return output;
}

//Implements the key schedule and expands the whole round key
std::array<std::array<unsigned char, 44>, 4> key_schedule(
    std::array<std::array<unsigned char, 4>, 4> cipher_key
)
{
    //Array to hold all permutations of the round key
	std::array<std::array<unsigned char, 44>, 4> round_key;

    //Used to hold the next generated column of the round key
	unsigned char temp[4];

    //Makes the first 4 columns of the round key the same as the cipher key
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			round_key[i][j] = cipher_key[i][j];
		}
	}

    std::array<unsigned char, 4> a_rot_word;

    // Loop goes through all columns of the round key
    // starts at 4 to ignore the cipher values
	for(int i = 4; i < 44; i++)
	{

		if(i % 4 == 0)
		{
			a_rot_word = rot_word(round_key, a_rot_word, i - 4);

            //Performs the s box on it
			a_rot_word = s_box_k(a_rot_word);

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
        //Checks if the value of the round key is not divisible evenly by 4
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

//Used in key expansion, rotates a column by 1 byte "upwards"
std::array<unsigned char, 4> rot_word(
    std::array<std::array<unsigned char, 44>, 4> round_key,
    std::array<unsigned char, 4> rot_word,
    int i
)
{
	rot_word[0] = round_key[1][3 + i];
	rot_word[1] = round_key[2][3 + i];
	rot_word[2] = round_key[3][3 + i];
	rot_word[3] = round_key[0][3 + i];

	return rot_word;
}

//S Box function to be used with the round key expansion
std::array<unsigned char, 4> s_box_k(
    std::array<unsigned char, 4> a_rot_word
)
{
	for(int i = 0; i < 4; i++)
	{
	    //Character to hold a character from the array
		unsigned char n = a_rot_word[i];

        //n is AND'd with 15 in hex to isloate the first digit, i.e the 5 from 15
		unsigned char t = 0xF & n;

        //The 2 isloated numbers are inserted into the s_boxs_box array and this is assigned to the state array
		a_rot_word[i] = aes_const::S_BOX[n >> 4][t];
	}

	return a_rot_word;
}

//Function to get user to enter cipher
std::array<std::array<unsigned char, 4>, 4> cipher()
{
	//Used to split up each char in the key
	unsigned int cipher_count = 0;

	std::cout << aes_const::ENTER_KEY;

    std::array<std::array<unsigned char, 4>, 4> cipher_key;

    //Writes input to cipher
    std::string cipher;
	std::cin >> cipher;

    //Splits string up into cipher_key array
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
		    //Checks if the key is less than 16 characters
			if(cipher.length() <= cipher_count)
			{
			    //If it is it makes the remaining characters blank
				cipher_key[j][i] = ' ';
			}
			else
			{
			    //Reads char from input, writes to cipher_key array
				cipher_key[j][i] = cipher.at(cipher_count);
			}

			//Increments count to read the next char from the input
			cipher_count++;
		}
	}

	return cipher_key;
}
