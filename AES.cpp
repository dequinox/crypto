#include "AES.h"

std::string AES::encrypt(std::string _data)
{
}

std::string AES::decrypt(std::string _data)
{
}

void AES::cipher(unsigned char in[], unsigned char out[])
{
    unsigned char **state = new unsigned char *[4];
    unsigned char **key = new unsigned char *[number_of_rounds + 1];
    for (unsigned  i = 0; i < 4; i++)
        state[i] = new unsigned char[block_size];

    for (unsigned round = 0; round <= number_of_rounds; round++)
        key[round] = new unsigned char[4 * block_size];

    for (unsigned col = 0; col < block_size; col++)
        for (unsigned row = 0; row < 4; row++)
            state[row][col] = in[row + col * 4];
    
    add_round_key(state, key[0]);

    for (unsigned round = 1; round <= number_of_rounds - 1; round++)
    {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, key[round]);
    }

    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, key[number_of_rounds]);

    for (unsigned i = 0; i < 4; i++)
        delete[] state[i];
    
    for (unsigned round = 0; round <= number_of_rounds; round++)
        delete[] key[round];
    
    delete[] state;
    delete[] key;

}

void AES::sub_bytes(unsigned char **state)
{
    for (unsigned col = 0; col < block_size; col++)
        for (unsigned row = 0; row < 4; row++)
        {
            unsigned char num = state[row][col];
            state[row][col] = Sbox[num / 16][num % 16];
        }
}

void AES::shift_rows(unsigned char **state)
{
    for (unsigned row = 1; row < 4; row++)
        for (unsigned col = 0; col + row < block_size; col++)
            std::swap(state[row][col], state[row][col + row]);
}

void AES::mix_columns(unsigned char **state)
{
    for (unsigned col = 0; col < block_size; col++)
    {
        unsigned char s0c = state[0][col]; 
        unsigned char s1c = state[1][col];  
        unsigned char s2c = state[2][col];
        unsigned char s3c = state[3][col];

        state[0][col] = multiply(0x02, s0c) ^ multiply(0x03, s1c) ^ s2c ^ s3c;
        state[1][col] = s0c ^ multiply(0x02, s1c) ^ multiply(0x03, s2c) ^ s3c;
        state[2][col] = s0c ^ s1c ^ multiply(0x02, s2c) ^ multiply(0x03, s3c);
        state[3][col] = multiply(0x03, s0c) ^ s1c ^ s2c ^ multiply(0x02, s3c); 
    }
}

unsigned char xtime(unsigned char x)
{
    if (x & 0x80)
    {
        x = x << 1;
        x ^ 0x1b;
    }
    else 
        x = x << 1;

    return x;
}

unsigned char AES::multiply(unsigned char a, unsigned char b)
{
    unsigned char result = 0, tmp = a;
    for (unsigned i = 0; i < 8; i++)
    {
        if (b & 1)
        {
            result ^= tmp;
        }

        tmp = xtime(tmp);
    }

    return result;
}

void AES::add_round_key(unsigned char** state, unsigned char* key)
{
    for (unsigned row = 0; row < 4; row++)
        for (unsigned col = 0; col < block_size; col++)
            state[row][col] ^= key[row + col * 4];
}