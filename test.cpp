#include "test.hpp"

void test_encryption()
{
    std::vector<std::vector<unsigned char>> state{
        {
            {{0x32, 0x88, 0x31, 0xe0}},
            {{0x43, 0x5a, 0x31, 0x37}},
            {{0xf6, 0x30, 0x98, 0x07}},
            {{0xa8, 0x8d, 0xa2, 0x34}}
        }
    };
    //Expands the entire round key
    auto round_key = key_schedule(aes_const::CIPHER_KEY);
    //Resets count for the next loop
    encrypt_state(round_key, state);

    auto is_same = true;

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(state[i][j] != aes_const::CORRECT_ENCRYPT_STATE_OUTPUT[i][j])
                is_same = false;
        }
    }
    if(is_same)
        std::cout << "State test passed.\n";
    else
        std::cout << "State test failed.\n";

}

void test_cipher()
{
    const auto round_key = key_schedule(aes_const::CIPHER_KEY);

    auto is_same = true;

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 44; j++){
            if(round_key[i][j] != aes_const::CORRECT_KEYGEN_OUTPUT[i][j])
                is_same = false;
        }
    }
    if(is_same)
        std::cout << "Keygen test passed.\n";
    else
        std::cout << "Keygen test failed.\n";
}
