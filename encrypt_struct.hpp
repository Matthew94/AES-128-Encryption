#ifndef _AES_DATA_STRUCT_
#define _AES_DATA_STRUCT_

struct aes_data{
    std::array<std::array<unsigned char, 44>, 4> round_key;
    std::vector<std::vector<unsigned char>> state;
};

#endif // _AES_DATA_STRUCT_
