/*File for holding the core functions used in 128 bit aes encryption*/

#ifndef _AES_ENCRYPTION_FUNCTIONS_
#define _AES_ENCRYPTION_FUNCTIONS_

#include <array>
#include <iostream>
#include <vector>
#include <algorithm>

#include "constants.hpp"
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

void s_box(
    std::array<std::array<unsigned char, 4>, 4> &state
);

void row_shift(
    std::array<std::array<unsigned char, 4>, 4> &state
);

std::array<std::array<unsigned char, 4>, 4> mix_columns(
    std::array<std::array<unsigned char, 4>, 4> state
);

std::array<std::array<unsigned char, 44>, 4> key_schedule(
    std::array<std::array<unsigned char, 4>, 4> cipher_key
);

std::array<unsigned char, 4> rot_word(
    std::array<std::array<unsigned char, 44>, 4> round_key,
    std::array<unsigned char, 4> rot_word,
    int i
);

std::array<unsigned char, 4> s_box_k(
    std::array<unsigned char, 4> rot_word
);

std::array<std::array<unsigned char, 4>, 4> cipher();

#endif // _AES_ENCRYPTION_FUNCTIONS_




