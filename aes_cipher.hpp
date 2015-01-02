#ifndef _AES_CIPHER_FUNCTION_
#define _AES_CIPHER_FUNCTION_

#include <array>
#include <iostream>
#include "aes_strings.hpp"

std::array<std::array<unsigned char, 4>, 4> get_cipher_key();

#endif // _AES_CIPHER_FUNCTION_
