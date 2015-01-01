// Program refactor

#include <iostream>
#include <fstream>
#include <string>

void main_menu();

void test();

void encrypt_file(bool output_key);

void add_round_key(unsigned char state[4][4],
                   unsigned char round_key[4][44],
                   int &count);

void write_to_array(std::ifstream &infile,
                    unsigned char state[4][4]);

void s_box_k(unsigned char state[4],
             const unsigned char s[16][16]);

void s_box(unsigned char state[4][4],
           const unsigned char s[16][16]);

void rot_word(unsigned char round_key[4][44],
              unsigned char rot_word[4],
              int &count);

void write_to_file(unsigned char state[4][4],
                   std::ofstream &outfile);

void row_shift(unsigned char state[4][4]);

void mix_columns(unsigned char state[4][4]);

void cipher(unsigned char cipher_key[4][4]);

void encrypt_state(int &count,
                   const unsigned char s[16][16],
                   unsigned char round_key[4][44],
                   unsigned char state[4][4]);

void open_file(std::ifstream &infile,
               std::ofstream &outfile,
               bool output_key,
               std::ofstream &outkey,
               unsigned char cipher_key[4][4]);

void key_schedule(unsigned char cipher_key[4][4],
				  unsigned char round_key[4][44],
				  unsigned char a_rot_word[4],
				  const unsigned char s[16][16],
				  int &count);

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
					     "[1]: User is asked to enter the name of the input and output files.\n"
					     "[1]: The program automatically adds the .txt extension.\n\n"
                         "[2]: Same as [1] but outputs the key used in a .txt file for the user in the same directory.\n\n"
                         "[3]: Test Run: Encrypts a preset 4x4 array and outputs the results.\n\n"
                         "State is:" 	      << "\t\tThe cipher key is:" << "\tThe results should be:\n"
                         "[32, 88, 31, e0]" << "\t[2b, 28, ab, 09]"     << "\t[39, 02, dc, 19]\n"
                         "[43, 5a, 31, 37]" << "\t[7e, ae, f7, cf]"     << "\t[25, dc, 11, 6a]\n"
                         "[f6, 30, 98, 07]" << "\t[15, d2, 15, 4f]"     << "\t[84, 09, 85, 0b]\n"
                         "[a8, 8d, a2, 34]" << "\t[16, a6, 88, 3c]"     << "\t[1d, fb, 97, 32]\n\n"
                         "Values and expected results sourced from the following website:"
                         "http://www.cs.bc.edu/~straubin/cs381-05/blockciphers/rijndael_ingles2004.swf\n\n"
                         "It proves that this program works correctly.\n";
            main_menu();
            break;
		default:
			std::cout << "\nClosing program...";
	}
}


void test()
{
    //S box look up table
	const unsigned char s[16][16] =
	{
	   {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5,
	    0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
	   {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,
	    0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
	   {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC,
	    0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
	   {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A,
	    0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
	   {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,
	    0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
	   {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B,
	    0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
	   {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85,
	    0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
	   {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5,
	    0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
	   {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17,
	    0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
	   {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88,
	    0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
	   {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,
	    0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
	   {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9,
	    0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
	   {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6,
	    0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
	   {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,
	    0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
	   {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94,
	    0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
	   {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68,
	    0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}
	};

	unsigned char cipher_key[4][4] = {
		{0x2b, 0x28, 0xab, 0x09},
		{0x7e, 0xae, 0xf7, 0xcf},
		{0x15, 0xd2, 0x15, 0x4f},
		{0x16, 0xa6, 0x88, 0x3c}
    };

	unsigned char state[4][4] = {
	    {0x32, 0x88, 0x31, 0xe0},
		{0x43, 0x5a, 0x31, 0x37},
		{0xf6, 0x30, 0x98, 0x07},
		{0xa8, 0x8d, 0xa2, 0x34}
    };

	static int count = 0;

	//Array to hold all permutations of the round key
	unsigned char round_key[4][44];

	//Array to hold the rotated column used in the key schedule
	unsigned char a_rot_word[4];

    //Expands the entire round key
	key_schedule(cipher_key, round_key, a_rot_word, s, count);

	//Resets count for the next loop
	encrypt_state(count, s, round_key, state);

	std::cout << "\nThe results are:\n";

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			std::cout << std::hex << static_cast<int>(state[i][j]) << "\t";
		}
		std::cout << std::endl;
	}
}

void open_file(std::ifstream &infile,
               std::ofstream &outfile,
               bool output_key,
               std::ofstream &outkey,
               unsigned char cipher_key[4][4])
{
	std::string file_name;
	std::string key_name;
	char character;

	bool read_success = false;

	while(read_success == false)
	{
		std::cout << "\n\nEnter the name of a file to be read: ";
		std::cin >> file_name;

		file_name = (file_name + ".txt");

		infile.open(file_name.c_str());

		if (infile.is_open())
		{
			std::cout << "\n***The file has been opened.***\n\n";
			read_success = true;
		}
		else
		{
			std::cout << "\nThat is not a valid file name.\n\n";
		}
	}


	std::cout << "Enter the name of a file to be written to (if it already exists it will be overwritten): ";
	std::cin >> file_name;

	key_name  = (file_name + "_key" + ".txt");
	file_name = (file_name + ".txt");

	outfile.open(file_name.c_str());

	if(output_key == true)
	{
		outkey.open(key_name.c_str());

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

//Implements the key schedule and expands the whole round key
void key_schedule(unsigned char cipher_key[4][4],
				  unsigned char round_key[4][44],
				  unsigned char a_rot_word[4],
				  const unsigned char s[16][16],
				  int &count)
{
    //Used to hold the next generated column of the round key
	unsigned char temp[4];

    //Array holding the relevant values of the r_con for 128 bit AES
	const unsigned char r_con[4][10] = {
	    {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
    };

    //Makes the first 4 columns of the round key the same as the cipher key
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			round_key[i][j] = cipher_key[i][j];
		}
	}

    //Loop goes through all columns of the round key
    // starts at 4 to ignore the cipher values
	for(int i = 4; i < 44; i++)
	{

		if(i % 4 == 0)
		{

			rot_word(round_key, a_rot_word, count);

            //Performs the s box on it
			s_box_k(a_rot_word, s);

            //XORs rotated column with column 4 places
            //before in round key with part of the r_con
			for(int j = 0; j < 4; j++)
			{
				temp[j] = (round_key[j][i - 4] ^ a_rot_word[j] ^ r_con[j][(i / 4) - 1]);
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

	count = 0;
}

//S Box function to be used with the round key expansion
void s_box_k(unsigned char a_rot_word[4], const unsigned char s[16][16])
{

	for(int i = 0; i < 4; i++)
	{
	    //Character to hold a character from the array
		unsigned char n = a_rot_word[i];

        //n is AND'd with 15 in hex to isloate the first digit, i.e the 5 from 15
		unsigned char t = 0xF & n;

		//n is bit shifted 4 places accross to isolate the second digit from the character, i.e the 1 from 15
		n = n >> 4;

        //The 2 isloated numbers are inserted into the s_box array and this is assigned to the state array
		a_rot_word[i] = s[n][t];
	}
}

//S box function to be used in the encryption loop
void s_box(unsigned char state[4][4],
           const unsigned char s[16][16])
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
		    //Character to hold a character from the array
			unsigned char n = state[i][j];

            //n is AND'd with 15 in hex to isloate the first digit, i.e the 5 from 15
			char t = 0xF & n;
			n = n >> 4;																									//n is bit shifted 4 places accross to isolate the second digit from the character, i.e the 1 from 15

            //The 2 isloated numbers are inserted into the s_box array and this is assigned to the state array
			state[i][j] = s[n][t];
		}
	}
}

//Used in key expansion, rotates a column by 1 byte "upwards"
void rot_word(unsigned char round_key[4][44],
              unsigned char rot_word[4],
              int &count)
{
	rot_word[0] = round_key[1][3+count];
	rot_word[1] = round_key[2][3+count];
	rot_word[2] = round_key[3][3+count];
	rot_word[3] = round_key[0][3+count];

	count += 4;
}

//XORs the round key with state
void add_round_key(unsigned char state[4][4],
                   unsigned char round_key[4][44],
                   int &count)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
		    //XORs state with the currently used part of the round key
			state[i][j] = (state[i][j] ^ round_key[i][j + count]);
		}
	}

	count += 4;
}

//Shifts the values of the state array
void row_shift(unsigned char state[4][4])
{
    //Array to hold template
	unsigned char blank[4];

    //Copy a template of a row into an array
	for(int i = 0; i < 4; i++)
	{
		blank[i] = state[1][i];
	}

    //Shifts values in row 1, 1 byte over
	state[1][0] = blank[1];
	state[1][1] = blank[2];
	state[1][2] = blank[3];
	state[1][3] = blank[0];

    //Copy a template of a row into an array
	for(int i = 0; i < 4; i++)
	{
		blank[i] = state[2][i];
	}

    //Shifts values in row 2, 2 bytes over
	state[2][0] = blank[2];
	state[2][1] = blank[3];
	state[2][2] = blank[0];
	state[2][3] = blank[1];

    //Copy a template of a row into an array
	for(int i = 0; i < 4; i++)
	{
		blank[i] = state[3][i];
	}

    //Shifts values in row 3, 3 bytes over
	state[3][0] = blank[3];
	state[3][1] = blank[0];
	state[3][2] = blank[1];
	state[3][3] = blank[2];
}
//Implements mix columns function
void mix_columns(unsigned char state[4][4])
{
    //4x4 array for holding result of mix colmns function
	unsigned char output[4][4];

    //Lookup table for doing the "multiply by 02" calculation
	const unsigned char table_2[256] =   {
	    0x00,0x02,0x04,0x06,0x08,0x0a,0x0c,0x0e,
	    0x10,0x12,0x14,0x16,0x18,0x1a,0x1c,0x1e,
        0x20,0x22,0x24,0x26,0x28,0x2a,0x2c,0x2e,
        0x30,0x32,0x34,0x36,0x38,0x3a,0x3c,0x3e,
		0x40,0x42,0x44,0x46,0x48,0x4a,0x4c,0x4e,
		0x50,0x52,0x54,0x56,0x58,0x5a,0x5c,0x5e,
		0x60,0x62,0x64,0x66,0x68,0x6a,0x6c,0x6e,
		0x70,0x72,0x74,0x76,0x78,0x7a,0x7c,0x7e,
		0x80,0x82,0x84,0x86,0x88,0x8a,0x8c,0x8e,
		0x90,0x92,0x94,0x96,0x98,0x9a,0x9c,0x9e,
		0xa0,0xa2,0xa4,0xa6,0xa8,0xaa,0xac,0xae,
		0xb0,0xb2,0xb4,0xb6,0xb8,0xba,0xbc,0xbe,
		0xc0,0xc2,0xc4,0xc6,0xc8,0xca,0xcc,0xce,
		0xd0,0xd2,0xd4,0xd6,0xd8,0xda,0xdc,0xde,
		0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xec,0xee,
		0xf0,0xf2,0xf4,0xf6,0xf8,0xfa,0xfc,0xfe,
		0x1b,0x19,0x1f,0x1d,0x13,0x11,0x17,0x15,
		0x0b,0x09,0x0f,0x0d,0x03,0x01,0x07,0x05,
		0x3b,0x39,0x3f,0x3d,0x33,0x31,0x37,0x35,
		0x2b,0x29,0x2f,0x2d,0x23,0x21,0x27,0x25,
		0x5b,0x59,0x5f,0x5d,0x53,0x51,0x57,0x55,
		0x4b,0x49,0x4f,0x4d,0x43,0x41,0x47,0x45,
		0x7b,0x79,0x7f,0x7d,0x73,0x71,0x77,0x75,
		0x6b,0x69,0x6f,0x6d,0x63,0x61,0x67,0x65,
		0x9b,0x99,0x9f,0x9d,0x93,0x91,0x97,0x95,
		0x8b,0x89,0x8f,0x8d,0x83,0x81,0x87,0x85,
		0xbb,0xb9,0xbf,0xbd,0xb3,0xb1,0xb7,0xb5,
		0xab,0xa9,0xaf,0xad,0xa3,0xa1,0xa7,0xa5,
		0xdb,0xd9,0xdf,0xdd,0xd3,0xd1,0xd7,0xd5,
		0xcb,0xc9,0xcf,0xcd,0xc3,0xc1,0xc7,0xc5,
		0xfb,0xf9,0xff,0xfd,0xf3,0xf1,0xf7,0xf5,
		0xeb,0xe9,0xef,0xed,0xe3,0xe1,0xe7,0xe5,
    };

    //Lookup table for doing the "multiply by 03" calculation
	const unsigned char table_3[256] =	{
	    0x00,0x03,0x06,0x05,0x0c,0x0f,0x0a,0x09,
	    0x18,0x1b,0x1e,0x1d,0x14,0x17,0x12,0x11,
		0x30,0x33,0x36,0x35,0x3c,0x3f,0x3a,0x39,
		0x28,0x2b,0x2e,0x2d,0x24,0x27,0x22,0x21,
		0x60,0x63,0x66,0x65,0x6c,0x6f,0x6a,0x69,
		0x78,0x7b,0x7e,0x7d,0x74,0x77,0x72,0x71,
		0x50,0x53,0x56,0x55,0x5c,0x5f,0x5a,0x59,
		0x48,0x4b,0x4e,0x4d,0x44,0x47,0x42,0x41,
		0xc0,0xc3,0xc6,0xc5,0xcc,0xcf,0xca,0xc9,
		0xd8,0xdb,0xde,0xdd,0xd4,0xd7,0xd2,0xd1,
		0xf0,0xf3,0xf6,0xf5,0xfc,0xff,0xfa,0xf9,
		0xe8,0xeb,0xee,0xed,0xe4,0xe7,0xe2,0xe1,
		0xa0,0xa3,0xa6,0xa5,0xac,0xaf,0xaa,0xa9,
		0xb8,0xbb,0xbe,0xbd,0xb4,0xb7,0xb2,0xb1,
		0x90,0x93,0x96,0x95,0x9c,0x9f,0x9a,0x99,
		0x88,0x8b,0x8e,0x8d,0x84,0x87,0x82,0x81,
		0x9b,0x98,0x9d,0x9e,0x97,0x94,0x91,0x92,
		0x83,0x80,0x85,0x86,0x8f,0x8c,0x89,0x8a,
		0xab,0xa8,0xad,0xae,0xa7,0xa4,0xa1,0xa2,
		0xb3,0xb0,0xb5,0xb6,0xbf,0xbc,0xb9,0xba,
		0xfb,0xf8,0xfd,0xfe,0xf7,0xf4,0xf1,0xf2,
		0xe3,0xe0,0xe5,0xe6,0xef,0xec,0xe9,0xea,
		0xcb,0xc8,0xcd,0xce,0xc7,0xc4,0xc1,0xc2,
		0xd3,0xd0,0xd5,0xd6,0xdf,0xdc,0xd9,0xda,
		0x5b,0x58,0x5d,0x5e,0x57,0x54,0x51,0x52,
		0x43,0x40,0x45,0x46,0x4f,0x4c,0x49,0x4a,
		0x6b,0x68,0x6d,0x6e,0x67,0x64,0x61,0x62,
		0x73,0x70,0x75,0x76,0x7f,0x7c,0x79,0x7a,
		0x3b,0x38,0x3d,0x3e,0x37,0x34,0x31,0x32,
		0x23,0x20,0x25,0x26,0x2f,0x2c,0x29,0x2a,
		0x0b,0x08,0x0d,0x0e,0x07,0x04,0x01,0x02,
		0x13,0x10,0x15,0x16,0x1f,0x1c,0x19,0x1a
    };

    // Loops through 4 values of the state array
    // the 4 numbers are XOD'd after the matrix multiplication is done
	for(int i = 0; i < 4; i++)
	{
		output[0][i] = ( (table_2[static_cast<int>(state[0][i])]) ^ (table_3[static_cast<int>(state[1][i])]) ^ state[2][i] ^ state[3][i] );
	}

	for(int i = 0; i < 4; i++)
	{
		output[1][i] = ( state[0][i] ^ (table_2[static_cast<int>(state[1][i])]) ^ (table_3[static_cast<int>(state[2][i])]) ^ state[3][i] );
	}

	for(int i = 0; i < 4; i++)
	{
		output[2][i] = ( state[0][i] ^ state[1][i] ^ (table_2[static_cast<int>(state[2][i])]) ^ (table_3[static_cast<int>(state[3][i])]) );
	}

	for(int i = 0; i < 4; i++)
	{
		output[3][i] = ( (table_3[static_cast<int>(state[0][i])]) ^ state[1][i] ^ state[2][i] ^ (table_2[static_cast<int>(state[3][i])]) );
	}

    //Copies output array on to the state array
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			state[i][j] = output[i][j];
		}
	}

}

//Writes the state array to the output file
void write_to_file(unsigned char state[4][4], std::ofstream &outfile)
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
void cipher(unsigned char cipher_key[4][4])
{
	//Used to split up each char in the key
	int cipher_count = 0;

	std::cout << "\nEnter 16 characters for the key.\n"
                 "Less than 16 will result in blanks being used "
			     "which will make the file easier to crack.\n\n"
			     "Enter the key here: ";

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
}

//Performs AES encryption on a state array
void encrypt_state(int &count,
                   const unsigned char s[16][16],
				   unsigned char round_key[4][44],
                   unsigned char state[4][4])
{
        //Adds the round key to state
		add_round_key(state, round_key, count);

        //Does 9 rounds of the encryption
		for(int i = 0; i < 10; i++)
		{
			s_box(state, s);
			row_shift(state);
            if (i != 9)
            {
                mix_columns(state);
            }
			add_round_key(state, round_key, count);
		}

		count = 0;
}

//Performs AES 128 Bit Encryption on a file
void encrypt_file(bool output_key)
{
    //S box look up table
	const unsigned char s[16][16] =
	{
	   0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	   0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	   0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	   0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	   0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	   0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	   0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	   0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	   0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	   0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	   0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	   0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	   0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	   0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	   0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	   0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
	};

	unsigned char state[4][4] = {
					{' ', ' ', ' ', ' '},
					{' ', ' ', ' ', ' '},
					{' ', ' ', ' ', ' '},
					{' ', ' ', ' ', ' '}
				};


	unsigned char cipher_key[4][4] = {
					{' ', ' ', ' ', ' '},
					{' ', ' ', ' ', ' '},
					{' ', ' ', ' ', ' '},
					{' ', ' ', ' ', ' '}
				};

    //Array to hold all permutations of the round key
	unsigned char round_key[4][44];

	//Array to hold the rotated column used in the key schedule
	unsigned char a_rot_word[4];

	static int count = 0;

	cipher(cipher_key);

	//Expands the entire round key
	key_schedule(cipher_key, round_key, a_rot_word, s, count);

	std::ifstream infile;
	std::ofstream outfile;
	std::ofstream outkey;

	open_file(infile, outfile, output_key, outkey, cipher_key);

	while(infile)
	{
	    //Writes 16 characters to the state array
		write_to_array(infile, state);

		//Resets count for the next loop
		encrypt_state(count, s, round_key, state);

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
