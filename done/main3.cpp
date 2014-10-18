//The encryption program
//Second revision


#include <iostream>																										//Used to prompt the user to input values and to display text
#include <fstream>																										//For the reading of the text file and outputting the cipher text
#include <string>																										//Used to open files by name

void add_round_key		(unsigned char state[4][4], 	unsigned char round_key[4][44], int &count);
void open_file			(std::ifstream &infile, 		std::ofstream &outfile);										//Forward declarations for all of the functions
void write_to_array		(std::ifstream &infile, 		unsigned char state[4][4]);
void s_box_k			(unsigned char state[4], 		const unsigned char s[16][16]);
void s_box				(unsigned char state[4][4], 	const unsigned char s[16][16]);
void rot_word			(unsigned char round_key[4][44],unsigned char rot_word[4], int &count);
void write_to_file		(unsigned char state[4][4], 	std::ofstream &outfile);
void row_shift			(unsigned char state[4][4]);
void mix_columns		(unsigned char state[4][4]);
void key_schedule		(unsigned char cipher_key[4][4],
						  unsigned char round_key[4][44],
						  unsigned char a_rot_word[4],
						  const unsigned char s[16][16],
						  int &count);
void encrypt_file		();

void open_file			(std::ifstream &infile, 		std::ofstream &outfile)											//Opens input and output text files
{
	std::string file_name;																								//String to hold file name
	
	char character;																										//To hold character read from file
	
	bool read_success = false;
	
	while(read_success == false)
	{
		std::cout << "Enter the name of a file to be read: ";															//Prompt to get file name
		std::cin >> file_name;
		
		file_name = (file_name + ".txt");																				//Adds .txt extension so file can be read
			
		infile.open(file_name.c_str());																					//Opens file
		
		if (infile.is_open())																							//Checks if file is open
		{
			std::cout << "\n***The file has been opened.***\n\n";
			read_success = true;
		}
		else																											//Outputs if file is not vaild
		{
			std::cout << "\nThat is not a valid file name.\n\n";
		}
	}
	
					
	std::cout << "Enter the name of a file to be written to (if it already exists it will be overwritten): ";			//Prompt to get file name for output of cipher text
	std::cin >> file_name;
		
	file_name = (file_name + ".txt");																					//Adds .txt extension so file can be read
			
	outfile.open(file_name.c_str());																					//Opens file for output
}

void write_to_array		(std::ifstream &infile, 		unsigned char state[4][4])										//Reads 16 characters from text file, writes to state array
{
	
	for(int i = 0; i < 4; i++)													//Loop to go through all values of the state array
	{
		for( int j = 0; j < 4; j++)
		{
			state[j][i] = infile.get();											//Reads a character from file, writes to the array
			
		}
	}
}

void key_schedule		(unsigned char cipher_key[4][4],																//Implements the key schedule and expands the whole round key
						  unsigned char round_key[4][44],
						  unsigned char a_rot_word[4],
						  const unsigned char s[16][16],
						  int &count)				  
{
	unsigned char temp[4];																								//Used to hold the next generated column of the round key

	const unsigned char r_con[4][10] = {																				//Array holding the relevant values of the r_con for 128 bit AES
									{0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36},
									{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
									{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
									{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
								};	
	
	for(int i = 0; i < 4; i++)																							//Makes the first 4 columns of the round key the same as the cipher key
	{
		for(int j = 0; j < 4; j++)
		{
			round_key[i][j] = cipher_key[i][j];
		}
	}
		
	for(int i = 4; i < 44; i++)																							//Loop goes through all columns of the round key, starts at 4 to ignore the cipher values
	{	
		
		if(i % 4 == 0)																									//Checks if the current value is a multiple of 4
		{
			
			rot_word(round_key, a_rot_word, count);																		//Rotates the column by 1 byte
			s_box_k(a_rot_word, s);																						//Performs the s box on it
			
			for(int j = 0; j < 4; j++)																					//XORs rotated column with column 4 places before in round key with part of the r_con
			{
				temp[j] = ( round_key[j][i-4] ^ a_rot_word[j] ^ r_con[j][(i/4)-1] );
			}
			
			for(int j = 0; j < 4; j++)																					//Assigns the XOR'd column to the round key
			{
				round_key[j][i] = temp[j];
			}
		}
			
		else																											//Checks if the value of the round key is not divisible evenly by 4
		{
			for(int j = 0; j < 4; j++)
			{
				round_key[j][i] = round_key[j][i - 4] ^ round_key[j][i - 1];											//XORs the round key 4 places back with the round key from 1 place back
			}
		}
	}
	
	count = 0;																											//Resets count to 0 to be used in encryption function
}

void s_box_k			(unsigned char state[4], 		const unsigned char s[16][16])									//S Box function to be used with the round key expansion
{
	
	for(int i = 0; i < 4; i++)																							//Loops through entire column of state array
	{
		unsigned char 	n = state[i];																					//Character to hold a character from the array
		unsigned char	t = 0xF & n;																					//n is AND'd with 15 in hex to isloate the first digit, i.e the 5 from 15
		n = n >> 4;																										//n is bit shifted 4 places accross to isolate the second digit from the character, i.e the 1 from 15
			
		state[i] = s[n][t];																								//The 2 isloated numbers are inserted into the s_box array and this is assigned to the state array
	}
}

void s_box				(unsigned char state[4][4], 	const unsigned char s[16][16])									//S box function to be used in the encryption loop
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			unsigned char n = state[i][j];																				//Character to hold a character from the array
			char t = 0xF & n;																							//n is AND'd with 15 in hex to isloate the first digit, i.e the 5 from 15
			n = n >> 4;																									//n is bit shifted 4 places accross to isolate the second digit from the character, i.e the 1 from 15
			
			state[i][j] = s[n][t];																						//The 2 isloated numbers are inserted into the s_box array and this is assigned to the state array
		}
	}
}

void rot_word			(unsigned char round_key[4][44],unsigned char rot_word[4], int &count)							//Used in key expansion, rotates a column by 1 byte "upwards"
{
	unsigned char temp;																									//Holds the first value in the array so it can be put in the last value at the end
		
	temp = round_key[0][3+count];																								//Temp stores first value of the column in round key
	
	rot_word[0] = round_key[1][3+count];																				//Assigns the second value of the round key to the first in the rot_word array
	rot_word[1] = round_key[2][3+count];																				//Assigns the third value of the round key to the first in the rot_word array
	rot_word[2] = round_key[3][3+count];																				//Assigns the second value of the round key to the first in the rot_word array
	rot_word[3] = temp;																									//Assigned the temp variable to the fourth part of the rot_word array

	count+=4;
}

void add_round_key		(unsigned char state[4][4], 	unsigned char round_key[4][44], int &count)						//XORs the round key with state
{
	for(int i = 0; i < 4; i++)																							//Loops though all parts of an array
	{
		for(int j = 0; j < 4; j++)
		{
			state[i][j] = (state[i][j] ^ round_key[i][j + count]);														//XORs state with the currently used part of the round key
		}
	}
	
	count+=4;																											//Increases the value for count
}	

void row_shift			(unsigned char state[4][4])																		//Shifts the values of the state array
{
	unsigned char blank[4];																								//Array to hold template
	
	for(int i = 0; i < 4; i++)																							//Copy a template of a row into an array
	{
		blank[i] = state[1][i];
	}
	
	state[1][0] = blank[1];																								//Shifts values in row 1, 1 byte over
	state[1][1] = blank[2];
	state[1][2] = blank[3];
	state[1][3] = blank[0];
	
	for(int i = 0; i < 4; i++)																							//Copy a template of a row into an array
	{
		blank[i] = state[2][i];
	}
	
	state[2][0] = blank[2];																								//Shifts values in row 2, 2 bytes over
	state[2][1] = blank[3];
	state[2][2] = blank[0];
	state[2][3] = blank[1];
	
	for(int i = 0; i < 4; i++)																							//Copy a template of a row into an array
	{
		blank[i] = state[3][i];
	}
	
	state[3][0] = blank[3];																								//Shifts values in row 3, 3 bytes over
	state[3][1] = blank[0];
	state[3][2] = blank[1];
	state[3][3] = blank[2];	
}

void mix_columns		(unsigned char state[4][4])																		//Implements mix columns function
{
	unsigned char output[4][4];																							//4x4 array for holding result of mix colmns function
	
	const unsigned char table_2[256] =   {0x00,0x02,0x04,0x06,0x08,0x0a,0x0c,0x0e,0x10,0x12,0x14,0x16,0x18,0x1a,0x1c,0x1e,	//Lookup table for doing the "multiply by 02" calculation
										0x20,0x22,0x24,0x26,0x28,0x2a,0x2c,0x2e,0x30,0x32,0x34,0x36,0x38,0x3a,0x3c,0x3e,
										0x40,0x42,0x44,0x46,0x48,0x4a,0x4c,0x4e,0x50,0x52,0x54,0x56,0x58,0x5a,0x5c,0x5e,
										0x60,0x62,0x64,0x66,0x68,0x6a,0x6c,0x6e,0x70,0x72,0x74,0x76,0x78,0x7a,0x7c,0x7e,
										0x80,0x82,0x84,0x86,0x88,0x8a,0x8c,0x8e,0x90,0x92,0x94,0x96,0x98,0x9a,0x9c,0x9e,
										0xa0,0xa2,0xa4,0xa6,0xa8,0xaa,0xac,0xae,0xb0,0xb2,0xb4,0xb6,0xb8,0xba,0xbc,0xbe,
										0xc0,0xc2,0xc4,0xc6,0xc8,0xca,0xcc,0xce,0xd0,0xd2,0xd4,0xd6,0xd8,0xda,0xdc,0xde,
										0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xec,0xee,0xf0,0xf2,0xf4,0xf6,0xf8,0xfa,0xfc,0xfe,
										0x1b,0x19,0x1f,0x1d,0x13,0x11,0x17,0x15,0x0b,0x09,0x0f,0x0d,0x03,0x01,0x07,0x05,
										0x3b,0x39,0x3f,0x3d,0x33,0x31,0x37,0x35,0x2b,0x29,0x2f,0x2d,0x23,0x21,0x27,0x25,
										0x5b,0x59,0x5f,0x5d,0x53,0x51,0x57,0x55,0x4b,0x49,0x4f,0x4d,0x43,0x41,0x47,0x45,
										0x7b,0x79,0x7f,0x7d,0x73,0x71,0x77,0x75,0x6b,0x69,0x6f,0x6d,0x63,0x61,0x67,0x65,
										0x9b,0x99,0x9f,0x9d,0x93,0x91,0x97,0x95,0x8b,0x89,0x8f,0x8d,0x83,0x81,0x87,0x85,
										0xbb,0xb9,0xbf,0xbd,0xb3,0xb1,0xb7,0xb5,0xab,0xa9,0xaf,0xad,0xa3,0xa1,0xa7,0xa5,
										0xdb,0xd9,0xdf,0xdd,0xd3,0xd1,0xd7,0xd5,0xcb,0xc9,0xcf,0xcd,0xc3,0xc1,0xc7,0xc5,
										0xfb,0xf9,0xff,0xfd,0xf3,0xf1,0xf7,0xf5,0xeb,0xe9,0xef,0xed,0xe3,0xe1,0xe7,0xe5};
								
	const unsigned char table_3[256] =	{0x00,0x03,0x06,0x05,0x0c,0x0f,0x0a,0x09,0x18,0x1b,0x1e,0x1d,0x14,0x17,0x12,0x11, //Lookup table for doing the "multiply by 03" calculation
										0x30,0x33,0x36,0x35,0x3c,0x3f,0x3a,0x39,0x28,0x2b,0x2e,0x2d,0x24,0x27,0x22,0x21,
										0x60,0x63,0x66,0x65,0x6c,0x6f,0x6a,0x69,0x78,0x7b,0x7e,0x7d,0x74,0x77,0x72,0x71,
										0x50,0x53,0x56,0x55,0x5c,0x5f,0x5a,0x59,0x48,0x4b,0x4e,0x4d,0x44,0x47,0x42,0x41,
										0xc0,0xc3,0xc6,0xc5,0xcc,0xcf,0xca,0xc9,0xd8,0xdb,0xde,0xdd,0xd4,0xd7,0xd2,0xd1,
										0xf0,0xf3,0xf6,0xf5,0xfc,0xff,0xfa,0xf9,0xe8,0xeb,0xee,0xed,0xe4,0xe7,0xe2,0xe1,
										0xa0,0xa3,0xa6,0xa5,0xac,0xaf,0xaa,0xa9,0xb8,0xbb,0xbe,0xbd,0xb4,0xb7,0xb2,0xb1,
										0x90,0x93,0x96,0x95,0x9c,0x9f,0x9a,0x99,0x88,0x8b,0x8e,0x8d,0x84,0x87,0x82,0x81,
										0x9b,0x98,0x9d,0x9e,0x97,0x94,0x91,0x92,0x83,0x80,0x85,0x86,0x8f,0x8c,0x89,0x8a,
										0xab,0xa8,0xad,0xae,0xa7,0xa4,0xa1,0xa2,0xb3,0xb0,0xb5,0xb6,0xbf,0xbc,0xb9,0xba,
										0xfb,0xf8,0xfd,0xfe,0xf7,0xf4,0xf1,0xf2,0xe3,0xe0,0xe5,0xe6,0xef,0xec,0xe9,0xea,
										0xcb,0xc8,0xcd,0xce,0xc7,0xc4,0xc1,0xc2,0xd3,0xd0,0xd5,0xd6,0xdf,0xdc,0xd9,0xda,
										0x5b,0x58,0x5d,0x5e,0x57,0x54,0x51,0x52,0x43,0x40,0x45,0x46,0x4f,0x4c,0x49,0x4a,
										0x6b,0x68,0x6d,0x6e,0x67,0x64,0x61,0x62,0x73,0x70,0x75,0x76,0x7f,0x7c,0x79,0x7a,
										0x3b,0x38,0x3d,0x3e,0x37,0x34,0x31,0x32,0x23,0x20,0x25,0x26,0x2f,0x2c,0x29,0x2a,
										0x0b,0x08,0x0d,0x0e,0x07,0x04,0x01,0x02,0x13,0x10,0x15,0x16,0x1f,0x1c,0x19,0x1a};
	
	for(int i = 0; i < 4; i++)																							//Loops through 4 values of the state array, the 4 numbers are XOD'd after the maxtrix mulitplication is done
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
	
	for(int i = 0; i < 4; i++)																						//Copies output array on to the state array
	{
		for(int j = 0; j < 4; j++)
		{
			state[i][j] = output[i][j];
		}
	}
				
}

void write_to_file		(unsigned char state[4][4], 	std::ofstream &outfile)											//Writes the state array to the output file
{
	for(int i = 0; i < 4; i++)																							//Goes through all elements of the array
	{
		for(int j = 0; j < 4; j++)
		{
			outfile << state[j][i];																						//Writes character in array to file
		}
	}
	
}

void encrypt_file		()																								//Performs AES 128 Bit Encryption
{
	const unsigned char s[16][16] = 																					//S box look up table
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
	
	unsigned char state[4][4] = {																						//The array for state
					{' ', ' ', ' ', ' '},
					{' ', ' ', ' ', ' '},
					{' ', ' ', ' ', ' '},
					{' ', ' ', ' ', ' '}
				};
				
				
	unsigned char cipher_key[4][4] = {																					//Array to hold the cipher key
										{0x2b, 0x28, 0xab, 0x09},
										{0x7e, 0xae, 0xf7, 0xcf},
										{0x15, 0xd2, 0x15, 0x4f},
										{0x16, 0xa6, 0x88, 0x3c}
									}; 
								
	unsigned char round_key[4][44];																						//Array to hold all permutations of the round key
	unsigned char a_rot_word[4];																						//Array to hold the rotated column used in the key schedule
	
	static int count = 0;
	
	key_schedule(cipher_key, round_key, a_rot_word, s, count);															//Expands the entire round key
	
	std::ifstream infile;
	std::ofstream outfile;
	
	open_file(infile, outfile);																							//Opens the file
	
	while(infile)
	{
		write_to_array(infile, state);																					//Writes 16 characters to the state array
		
		add_round_key(state, round_key, count);																			//Adds the round key to state
		
		for(int i = 0; i < 9; i++)																						//Does 9 rounds of the encryption
		{
			s_box(state, s);																							//Does the s box function
			row_shift(state);																							//Shift rows function
			mix_columns(state);																							//Mix columns function
			add_round_key(state, round_key, count);																		//Adds the round key to state
		}
		
		s_box(state, s);																								//Does the s box function
		row_shift(state);																								//Shift rows function
		add_round_key(state, round_key, count);																			//Adds the round key to state
		
		count = 0;																										//Resets count for the next loop		

		write_to_file(state, outfile);																					//Outputs the final version of state to the file 													
	}
}

int main()
{
	encrypt_file();																										//Performs AES 128 Bit Encryption
	
	return 0;
}
