#ifndef _AES_FILE_FUNCTIONS_
#define _AES_FILE_FUNCTIONS_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::vector<unsigned char>> get_state_from_file(
    std::ifstream &infile
);

void write_to_file(
    std::vector<std::vector<unsigned char>> state,
    std::ofstream &outfile
);

void open_file(std::ifstream &infile, std::ofstream &outfile);

#endif // _AES_FILE_FUNCTIONS_
