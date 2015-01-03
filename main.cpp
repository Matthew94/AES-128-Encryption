/*
AES 128 Bit Encryption Program.

Made for my first year final C++ project. Now being fixed up.
*/

#include <iostream>
#include <fstream>
#include <array>
#include <vector>

#include "aes_constants.hpp"
#include "aes_strings.hpp"
#include "aes_en.hpp"
#include "aes_file_functions.hpp"
#include "aes_key.hpp"
#include "aes_cipher.hpp"
#include "test.hpp"

void main_menu();

void encrypt_file();

void main_menu()
{
    std::cout << aes_const::MENU;

    int choice;
    std::cin >> choice;

    switch(choice)
    {
        case 1:
            encrypt_file();
            break;
        case 2:
            test_encryption();
            test_cipher();
            main_menu();
            break;
        case 3:
            std::cout << aes_const::INFO;
            main_menu();
            break;
        default:
            std::cout << "\nClosing program...";
    }
}

//Performs AES 128 Bit Encryption on a file
void encrypt_file()
{
    const auto cipher_key = get_cipher_from_user();

    //Expands the entire round key
    auto round_key = key_schedule(cipher_key);

    std::ifstream infile;
    std::ofstream outfile;

    open_file(infile, outfile);

    while(infile)
    {
        //Writes 16 characters to the state array
        auto state = get_state_from_file(infile);

        //Resets count for the next loop
        encrypt_state(round_key, state);

        //Outputs the final version of state to the file
        write_to_file(state, outfile);
    }

    std::cout << "\nThe encryption is complete.";
}

int main()
{
    main_menu();

    return 0;
}
