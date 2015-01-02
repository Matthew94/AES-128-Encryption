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

void main_menu();

void test();

void encrypt_file();

void print_test_array(
    std::vector<std::vector<unsigned char>> &state
);

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
            test();
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

void test()
{
    const std::array<std::array<unsigned char, 4>, 4> cipher_key =
    {
        {
            {{0x2b, 0x28, 0xab, 0x09}},
            {{0x7e, 0xae, 0xf7, 0xcf}},
            {{0x15, 0xd2, 0x15, 0x4f}},
            {{0x16, 0xa6, 0x88, 0x3c}}
        }
    };

    std::vector<std::vector<unsigned char>> state
    {
        {
            {{0x32, 0x88, 0x31, 0xe0}},
            {{0x43, 0x5a, 0x31, 0x37}},
            {{0xf6, 0x30, 0x98, 0x07}},
            {{0xa8, 0x8d, 0xa2, 0x34}}
        }
    };
    //Expands the entire round key
    auto round_key = key_schedule(cipher_key);

    //Resets count for the next loop
    encrypt_state(round_key, state);

    print_test_array(state);
}

void print_test_array(
    std::vector<std::vector<unsigned char>> &state
)
{
    std::cout << "\nThe results are:\n";

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            std::cout << std::hex << static_cast<int>(state[i][j]) << "\t";
        }
        std::cout << "\n";
    }
}



//Performs AES 128 Bit Encryption on a file
void encrypt_file()
{
    std::vector<std::vector<unsigned char>> state {
        {0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00},
    };

    auto cipher_key = cipher();

    //Expands the entire round key
    auto round_key = key_schedule(cipher_key);

    std::ifstream infile;
    std::ofstream outfile;

    open_file(infile, outfile);

    while(infile)
    {
        //Writes 16 characters to the state array
        write_to_array(infile, state);

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
