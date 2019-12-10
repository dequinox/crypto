#include "AES.h"

std::string AES::encrypt(std::string _data)
{
}

std::string AES::decrypt(std::string _data)
{
}

void AES::cipher(std::byte in[], std::byte out[])
{
    std::byte **state = new std::byte *[4];
    for (unsigned  i = 0; i < 4; i++)
        state[i] = new std::byte[block_size];

    for (unsigned col = 0; col < block_size; col++)
        for (unsigned row = 0; row < 4; row++)
            state[row][col] = in[row + col * 4];
    
    add_round_key(state);

    for (unsigned round = 0; round < number_of_rounds - 1; round++)
    {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state);
    }

    sub_bytes(state);
    shift_rows(state);
    add_round_key(state);

    for (unsigned i = 0; i < 4; i++)
        delete[] state[i];
    delete[] state;

}