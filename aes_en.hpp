/*File for holding the core functions used in 128 bit aes encryption*/

#ifndef _AES_ENCRYPTION_FUNCTIONS_
#define _AES_ENCRYPTION_FUNCTIONS_

#include <array>
#include <iostream>
#include <vector>
#include <algorithm>

#include "aes_constants.hpp"
#include "aes_strings.hpp"

void encrypt_state(
    std::array<std::array<unsigned char, 44>, 4> &round_key,
    std::array<std::array<unsigned char, 4>, 4> &state
);

int add_round_key(
    std::array<std::array<unsigned char, 4>, 4> &state,
    std::array<std::array<unsigned char, 44>, 4> round_key,
    int count
);

std::array<std::array<unsigned char, 4>, 4> sub_bytes(
    std::array<std::array<unsigned char, 4>, 4> state
);

void shift_rows(
    std::array<std::array<unsigned char, 4>, 4> &state
);

std::array<std::array<unsigned char, 4>, 4> mix_columns(
    std::array<std::array<unsigned char, 4>, 4> state
);

std::array<std::array<unsigned char, 4>, 4> cipher();

#endif // _AES_ENCRYPTION_FUNCTIONS_




