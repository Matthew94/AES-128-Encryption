#include "aes_cipher.hpp"

//Function to get user to enter cipher
std::array<std::array<unsigned char, 4>, 4> get_cipher_key()
{
    //Used to split up each char in the key
    unsigned int cipher_count = 0;

    std::cout << aes_const::ENTER_KEY;

    std::array<std::array<unsigned char, 4>, 4> cipher_key;

    //Writes input to cipher
    std::string cipher;
    std::cin >> cipher;

    //Splits string up into cipher_key array
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            //Checks if the key is less than 16 characters
            if(cipher.length() <= cipher_count)
            {
                //If it is it makes the remaining characters blank
                cipher_key[j][i] = ' ';
            }
            else
            {
                //Reads char from input, writes to cipher_key array
                cipher_key[j][i] = cipher.at(cipher_count);
            }

            //Increments count to read the next char from the input
            cipher_count++;
        }
    }

    return cipher_key;
}

