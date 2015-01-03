//Performs AES encryption on a state array

#include "aes_en.hpp"

std::vector<std::vector<unsigned char>> encrypt_state(
    std::array<std::array<unsigned char, 44>, 4> round_key,
    std::vector<std::vector<unsigned char>> state
)
{
        //Does 10 rounds of the encryption
		for(int i = 0; i < 11; i++)
		{
            if(i > 0){
                state = sub_bytes(state);
                state = shift_rows(state);
            }

            if (i != 10 && i > 0)
                state = mix_columns(state);

            state = add_round_key(state, round_key, i * 4);
		}

		return state;
}

//XORs the round key with state
std::vector<std::vector<unsigned char>> add_round_key(
    std::vector<std::vector<unsigned char>> state,
    std::array<std::array<unsigned char, 44>, 4> round_key,
    int index
)
{
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
		    //XORs state with the currently used part of the round key
			state[i][j] = state[i][j] ^ round_key[i][j + index];
		}
	}

	return state;
}

//S box function to be used in the encryption loop
std::vector<std::vector<unsigned char>> sub_bytes(
    std::vector<std::vector<unsigned char>> state
)
{
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			const unsigned char n = state[i][j];

            //n is AND'd with 15 in hex to isolate the first digit
			// n is bit shifted 4 places across to get the second digit
			state[i][j] = aes_const::S_BOX[n >> 4][n & 0xF];
		}
	}

	return state;
}

//Shifts the values of the state array
std::vector<std::vector<unsigned char>> shift_rows(
    std::vector<std::vector<unsigned char>> state
)
{
    //Copy a template of a row into an array
	for(int i = 0; i < 4; i++)
        std::rotate(state[i].begin(), state[i].begin() + i, state[i].end());

	return state;
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

    // Loops through each column of vector
    // Uses Rijndael mix columns look up tables
    // Then XORS all values together
	for(int i = 0; i < 4; i++)
		output[0][i] =
            aes_const::TABLE_2[state[0][i]] ^
            aes_const::TABLE_3[state[1][i]] ^
            state[2][i] ^
            state[3][i];

	for(int i = 0; i < 4; i++)
		output[1][i] =
            state[0][i] ^
            aes_const::TABLE_2[state[1][i]] ^
            aes_const::TABLE_3[state[2][i]] ^
            state[3][i];

	for(int i = 0; i < 4; i++)
		output[2][i] =
            state[0][i] ^
            state[1][i] ^
            aes_const::TABLE_2[state[2][i]] ^
            aes_const::TABLE_3[state[3][i]];

	for(int i = 0; i < 4; i++)
		output[3][i] =
            aes_const::TABLE_3[state[0][i]] ^
            state[1][i] ^
            state[2][i] ^
            aes_const::TABLE_2[state[3][i]];

    return output;
}
