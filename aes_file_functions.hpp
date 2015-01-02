#ifndef _AES_FILE_FUNCTIONS_
#define _AES_FILE_FUNCTIONS_

#include <array>
#include <iostream>
#include <fstream>
#include <string>

void write_to_array(
    std::ifstream &infile,
    std::array<std::array<unsigned char, 4>, 4> &state
);

void write_to_file(
    std::array<std::array<unsigned char, 4>, 4> state,
    std::ofstream &outfile
);

void open_file(
    std::ifstream &infile,
    std::ofstream &outfile
);

#endif // _AES_FILE_FUNCTIONS_
