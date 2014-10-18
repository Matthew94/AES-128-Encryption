//Outputs an array to a test file

#include <iostream>
#include <fstream>
#include <string>
#include "aes.h"

void write_to_file(unsigned char state[4][4])
{
	using namespace std;
	
	ofstream outfile;
	string file_name;
					
	cout << "Enter the name of a file to be written to: ";		//Prompt to get file name
	cin >> file_name;
		
	file_name = (file_name + ".txt");							//Adds .txt extension so file can be read
			
	outfile.open(file_name.c_str());							//Opens file
	
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			outfile << state[j][i];
		}
	}
	
}
