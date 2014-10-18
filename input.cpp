//Function to read the first 16 characters from a text file into an array

#include <iostream>
#include <fstream>
#include <stdio.h>

int main()
{
	using namespace std;

	ifstream infile;
	infile.open("input.text");
	
	char array[4][4] = {
						{'0','0','0','0'},
						{'0','0','0','0'},
						{'0','0','0','0'},
						{'0','0','0','0'}						
						};
	char temp;
	
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			array[i][j] = getchar();
		}
	}
	
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			std::cout << array[i][j] << "\t";
		}
		
		std::cout << "\n";
	}
	
	return 0;
}