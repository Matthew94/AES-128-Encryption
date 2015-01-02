#ifndef _AES_KEY_
#define _AES_KEY_

#include <array>

#include "aes_constants.hpp"

/*
Runs the AES key schedule and generates the round key.
*/
std::array<std::array<unsigned char, 44>, 4> key_schedule(
    std::array<std::array<unsigned char, 4>, 4> cipher_key
);

std::array<unsigned char, 4> rot_word(
    std::array<std::array<unsigned char, 44>, 4> round_key,
    int i
);

std::array<unsigned char, 4> s_box_k(
    std::array<unsigned char, 4> rot_word
);
#endif // _AES_KEY_



