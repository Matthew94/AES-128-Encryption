#include "aes_cipher.hpp"

std::array<std::array<unsigned char, 4>, 4> get_cipher_from_user(){
    std::cout << aes_const::ENTER_KEY;

    //Writes input to cipher
    std::string cipher;
    std::cin >> cipher;

    return get_cipher_array(cipher);
}
std::array<std::array<unsigned char, 4>, 4> get_cipher_array(
    std::string cipher_input
)
{
    std::array<std::array<unsigned char, 4>, 4> cipher_key;

    //Splits string up into cipher_key array
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            const unsigned int CIPHER_INDEX = (i * 4) + j;

            //Reads char from input, writes to cipher_key array
            cipher_key[j][i] = cipher_input.at(CIPHER_INDEX);
        }
    }

    return cipher_key;
}

