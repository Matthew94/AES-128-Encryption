#ifndef _AES_FILE_FUNCTIONS_
#define _AES_FILE_FUNCTIONS_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void write_to_array(
    std::ifstream &infile,
    std::vector<std::vector<unsigned char>> &state
);

void write_to_file(
    std::vector<std::vector<unsigned char>> state,
    std::ofstream &outfile
);

void open_file(
    std::ifstream &infile,
    std::ofstream &outfile
);

#endif // _AES_FILE_FUNCTIONS_
