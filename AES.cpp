#include "AES.h"

std::string AES::encrypt(std::string _data)
{
}

std::string AES::decrypt(std::string _data)
{
}

std::byte* AES::cipher(std::byte in[])
{
    std::byte state[4][block_size];

    for (unsigned col = 0; col < block_size; col++)
    {
        for (unsigned row = 0; row < 4; row++)
        {
            state[row][col] = in[row + col * 4];
        }
    }
}