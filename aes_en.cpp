//Performs AES encryption on a state array

#include "aes_en.hpp"

void encrypt_state(
    std::array<std::array<unsigned char, 44>, 4> &round_key,
    std::vector<std::vector<unsigned char>> &state
)
{
        int count = 0;

        //Adds the round key to state
		count = add_round_key(state, round_key, count);

        //Does 9 rounds of the encryption
		for(int i = 0; i < 10; i++)
		{
			state = sub_bytes(state);
			shift_rows(state);
            if (i != 9)
            {
                state = mix_columns(state);
            }
			count = add_round_key(state, round_key, count);
		}
}

//XORs the round key with state
int add_round_key(
    std::vector<std::vector<unsigned char>> &state,
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
std::vector<std::vector<unsigned char>> sub_bytes(
    std::vector<std::vector<unsigned char>> state
)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
		    //Character to hold a character from the array
			const unsigned char n = state[i][j];

            //n is AND'd with 15 in hex to isolate the first digit
			// n is bit shifted 4 places across to get the second digit
			state[i][j] = aes_const::S_BOX[n >> 4][n & 0xF];
		}
	}

	return state;
}

//Shifts the values of the state array
void shift_rows(
    std::vector<std::vector<unsigned char>> &state
)
{
    //Copy a template of a row into an array
	for(int i = 0; i < 4; i++)
	{
        std::rotate(state[i].begin(), state[i].begin() + i, state[i].end());
	}
}

//Implements mix columns function
std::vector<std::vector<unsigned char>> mix_columns(
    std::vector<std::vector<unsigned char>> state
)
{
    //4x4 array for holding result of mix columns function
	std::vector<std::vector<unsigned char>> output {
	    {0x00, 0x00, 0x00, 0x00},
	    {0x00, 0x00, 0x00, 0x00},
	    {0x00, 0x00, 0x00, 0x00},
	    {0x00, 0x00, 0x00, 0x00},
	};

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
