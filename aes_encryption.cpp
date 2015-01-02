/*
AES 128 Bit Encryption Program.

Made for my first year final C++ project. Now being fixed up.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include "constants.hpp"

void main_menu();

void test();

void encrypt_file(bool output_key);

int add_round_key(
    std::array<std::array<unsigned char, 4>, 4> &state,
    std::array<std::array<unsigned char, 44>, 4> round_key,
    int count
);

void write_to_array(
    std::ifstream &infile,
    std::array<std::array<unsigned char, 4>, 4> &state
);

void s_box_k(unsigned char state[4]);

void s_box(std::array<std::array<unsigned char, 4>, 4> &state);

int rot_word(
    std::array<std::array<unsigned char, 44>, 4> round_key,
    unsigned char rot_word[4],
    int count
);

void write_to_file(
    std::array<std::array<unsigned char, 4>, 4> state,
    std::ofstream &outfile
);

void row_shift(
    std::array<std::array<unsigned char, 4>, 4> &state
);

std::array<std::array<unsigned char, 4>, 4> mix_columns(
    std::array<std::array<unsigned char, 4>, 4> state
);

std::array<std::array<unsigned char, 4>, 4> cipher();

void encrypt_state(
    std::array<std::array<unsigned char, 44>, 4> &round_key,
    std::array<std::array<unsigned char, 4>, 4> &state
);

void open_file(
    std::ifstream &infile,
    std::ofstream &outfile,
    bool output_key,
    std::ofstream &outkey,
    std::array<std::array<unsigned char, 4>, 4> cipher_key
);

void print_test_array(
    std::array<std::array<unsigned char, 4>, 4> &state
);

std::array<std::array<unsigned char, 44>, 4> key_schedule(
    std::array<std::array<unsigned char, 4>, 4> cipher_key,
	unsigned char a_rot_word[4]
);

void main_menu()
{
	std::cout << "\n***** AES 128 Bit Encryption Program *****\n\n"
                 "[1] Encrypt .txt file, no key output.\n"
                 "[2] Encrypt .txt file, outputs key to .txt file.\n"
                 "[3] Test Run.\n"
                 "[4] Description of options.\n"
                 "[ANY OTHER KEY] Quit the program\n\n"
			     "Enter your choice: ";

    int choice;
	std::cin >> choice;

	switch(choice)
	{
		case 1:
			encrypt_file(false);
			break;
		case 2:
			encrypt_file(true);
			break;
		case 3:
			test();
			main_menu();
			break;
		case 4:
			std::cout << "\n[1]: Encodes a .txt file in 128 Bit AES.\n"
					     "[1]: Enter the name of the input and output files.\n"
					     "[1]: .txt extension is automatically added.\n\n"

                         "[2]: Like [1] but key is also saved to a file.\n\n"

                         "[3]: Test to ensure correct operation.\n\n"

                         "State is:\t\tThe cipher key is:\t"
                         "The results should be:\n"

                         "[32, 88, 31, e0]\t[2b, 28, ab, 09]\t"
                         "[39, 02, dc, 19]\n"

                         "[43, 5a, 31, 37]\t[7e, ae, f7, cf]\t"
                         "[25, dc, 11, 6a]\n"

                         "[f6, 30, 98, 07]\t[15, d2, 15, 4f]\t"
                         "[84, 09, 85, 0b]\n"

                         "[a8, 8d, a2, 34]\t[16, a6, 88, 3c]\t"
                         "[1d, fb, 97, 32]\n\n"

                         "Values and expected results sourced"
                         " from the following website:\n"

                         "http://www.cs.bc.edu/~straubin/cs381-05/"
                         "blockciphers/rijndael_ingles2004.swf\n\n"
                         "It should prove that this program works correctly.\n";
            main_menu();
            break;
		default:
			std::cout << "\nClosing program...";
	}
}

void test()
{
	std::array<std::array<unsigned char, 4>, 4> cipher_key = {
	    {
            {0x2b, 0x28, 0xab, 0x09},
            {0x7e, 0xae, 0xf7, 0xcf},
            {0x15, 0xd2, 0x15, 0x4f},
            {0x16, 0xa6, 0x88, 0x3c}
        }
    };

	std::array<std::array<unsigned char, 4>, 4> state = {
	    {
            {0x32, 0x88, 0x31, 0xe0},
            {0x43, 0x5a, 0x31, 0x37},
            {0xf6, 0x30, 0x98, 0x07},
            {0xa8, 0x8d, 0xa2, 0x34}
	    }
    };

	//Array to hold the rotated column used in the key schedule
	unsigned char a_rot_word[4];

    //Expands the entire round key
	auto round_key = key_schedule(cipher_key, a_rot_word);

	//Resets count for the next loop
	encrypt_state(round_key, state);

    print_test_array(state);
}

void print_test_array(
    std::array<std::array<unsigned char, 4>, 4> &state
)
{
	std::cout << "\nThe results are:\n";

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			std::cout << std::hex << static_cast<int>(state[i][j]) << "\t";
		}
		std::cout << "\n";
	}
}

void open_file(
    std::ifstream &infile,
    std::ofstream &outfile,
    bool output_key,
    std::ofstream &outkey,
    std::array<std::array<unsigned char, 4>, 4> cipher_key
)
{
	std::string file_name;
	std::string key_name;

	while(1)
	{
		std::cout << "\n\nEnter the name of a file to be read: ";
		std::cin >> file_name;

		file_name = (file_name + ".txt");

		infile.open(file_name.c_str());

		if (infile.is_open())
		{
			std::cout << "\n***The file has been opened.***\n\n";
			break;
		}
		else
		{
			std::cout << "\nThat is not a valid file name.\n\n";
		}
	}

	std::cout << "Enter the name of a file to be written to (if it already exists it will be overwritten): ";
	std::cin >> file_name;

	key_name = file_name + "_key.txt";
	file_name += ".txt";

	outfile.open(file_name);

	if(output_key == true)
	{
		outkey.open(key_name);

		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				outkey << cipher_key[j][i];
			}
		}

		std::cout << "\nThe key has been written to a file. (KEEP IT HIDDEN)\n";
	}
}

//Reads 16 characters from text file, writes to state array
void write_to_array(
    std::ifstream &infile,
    std::array<std::array<unsigned char, 4>, 4> &state
)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			state[j][i] = infile.get();
		}
	}
}

//Implements the key schedule and expands the whole round key
std::array<std::array<unsigned char, 44>, 4> key_schedule(
    std::array<std::array<unsigned char, 4>, 4> cipher_key,
	unsigned char a_rot_word[4]
)
{
    int count = 0;

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

    // Loop goes through all columns of the round key
    // starts at 4 to ignore the cipher values
	for(int i = 4; i < 44; i++)
	{

		if(i % 4 == 0)
		{

			count = rot_word(round_key, a_rot_word, count);

            //Performs the s box on it
			s_box_k(a_rot_word);

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

//S Box function to be used with the round key expansion
void s_box_k(
    unsigned char a_rot_word[4]
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
}

//S box function to be used in the encryption loop
void s_box(
    std::array<std::array<unsigned char, 4>, 4> &state
)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
		    //Character to hold a character from the array
			const unsigned char n = state[i][j];

            //n is AND'd with 15 in hex to isolate the first digit, i.e the 5 from 15
			const char t = 0xF & n;

			// n is bit shifted 4 places across
			// This is to isolate the second digit from the character
			// i.e the 1 from 15
			state[i][j] = aes_const::S_BOX[n >> 4][t];
		}
	}
}

//Used in key expansion, rotates a column by 1 byte "upwards"
int rot_word(
    std::array<std::array<unsigned char, 44>, 4> round_key,
    unsigned char rot_word[4],
    int count
)
{
	rot_word[0] = round_key[1][3+count];
	rot_word[1] = round_key[2][3+count];
	rot_word[2] = round_key[3][3+count];
	rot_word[3] = round_key[0][3+count];

	return count + 4;
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

//Writes the state array to the output file
void write_to_file(
    std::array<std::array<unsigned char, 4>, 4> state,
    std::ofstream &outfile
)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			outfile << state[j][i];
		}
	}
}

//Function to get user to enter cipher
std::array<std::array<unsigned char, 4>, 4> cipher()
{
	//Used to split up each char in the key
	int cipher_count = 0;

	std::cout << "\nEnter 16 characters for the key.\n"
                 "Less than 16 will result in blanks being used "
			     "which will make the file easier to crack.\n\n"
			     "Enter the key here: ";

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

//Performs AES encryption on a state array
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
			s_box(state);
			row_shift(state);
            if (i != 9)
            {
                state = mix_columns(state);
            }
			count = add_round_key(state, round_key, count);
		}
}

//Performs AES 128 Bit Encryption on a file
void encrypt_file(
    bool output_key
)
{
	std::array<std::array<unsigned char, 4>, 4> state;

	//Array to hold the rotated column used in the key schedule
	unsigned char a_rot_word[4];

	auto cipher_key = cipher();

	//Expands the entire round key
	auto round_key = key_schedule(cipher_key, a_rot_word);

	std::ifstream infile;
	std::ofstream outfile;
	std::ofstream outkey;

	open_file(infile, outfile, output_key, outkey, cipher_key);

	while(infile)
	{
	    //Writes 16 characters to the state array
		write_to_array(infile, state);

		//Resets count for the next loop
		encrypt_state(round_key, state);

		//Outputs the final version of state to the file
		write_to_file(state, outfile);
	}

	std::cout << "\nThe encryption is complete.";
}

int main()
{
	main_menu();

	return 0;
}
