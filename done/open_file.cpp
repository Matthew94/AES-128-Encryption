//Function to open file

#include <iostream>
#include <fstream>
#include <string>
#include "aes.h"


void open_file(std::ifstream &infile)
{
	std::string file_name;										//String to hold file name
	
	char character;											//To hold character read from fil
	
	bool read_success = false;
	
	while(read_success == false)
	{
		std::cout << "Enter the name of a file to be read: ";		//Prompt to get file name
		std::cin >> file_name;
		
		file_name = (file_name + ".txt");						//Adds .txt extension so file can be read
			
		infile.open(file_name.c_str());							//Opens file
		
		if (infile.is_open())									//Checks if file is open
		{
			std::cout << "\n***The file has been opened.***\n\n";
			read_success = true;
		}
		else													//Outputs if file is not vaild
		{
			std::cout << "\nThat is not a valid file name.\n\n";
		}
	}
}
