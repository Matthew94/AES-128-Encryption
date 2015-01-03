/*File for holding the core functions used in 128 bit aes encryption*/

#ifndef _AES_ENCRYPTION_FUNCTIONS_
#define _AES_ENCRYPTION_FUNCTIONS_

#include <array>
#include <iostream>
#include <vector>
#include <algorithm>

#include "aes_constants.hpp"
#include "aes_strings.hpp"

std::vector<std::vector<unsigned char>> encrypt_state(
    std::array<std::array<unsigned char, 44>, 4> round_key,
    std::vector<std::vector<unsigned char>> state
);

std::vector<std::vector<unsigned char>> add_round_key(
    std::vector<std::vector<unsigned char>> state,
    std::array<std::array<unsigned char, 44>, 4> round_key,
    int index
);

std::vector<std::vector<unsigned char>> sub_bytes(
    std::vector<std::vector<unsigned char>> state
);

std::vector<std::vector<unsigned char>> shift_rows(
    std::vector<std::vector<unsigned char>> state
);

std::vector<std::vector<unsigned char>> mix_columns(
    std::vector<std::vector<unsigned char>> state
);

#endif // _AES_ENCRYPTION_FUNCTIONS_




