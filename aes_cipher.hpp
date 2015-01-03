#ifndef _AES_CIPHER_FUNCTION_
#define _AES_CIPHER_FUNCTION_

#include <array>
#include <iostream>
#include "aes_strings.hpp"

std::array<std::array<unsigned char, 4>, 4> get_cipher_from_user();

std::array<std::array<unsigned char, 4>, 4> get_cipher_array(
    std::string cipher_input
);

#endif // _AES_CIPHER_FUNCTION_
