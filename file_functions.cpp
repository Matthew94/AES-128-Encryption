#include "file_functions.hpp"

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

void open_file(
    std::ifstream &infile,
    std::ofstream &outfile
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

	file_name += ".txt";

	outfile.open(file_name);
}
