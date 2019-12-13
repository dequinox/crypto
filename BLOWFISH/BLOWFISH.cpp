#include "BLOWFISH.h"
#include <iostream>

BLOWFISH::BLOWFISH()
{
}

void BLOWFISH::set_key(unsigned char key[], unsigned key_length)
{
    if (key_length % 4 != 0 || key_length == 0)
    {
        // TODO return Error
    }

    generate_keys(key, key_length);
}

uint32_t LOAD32(unsigned char key[])
{
    uint32_t a = key[0];
    uint32_t b = key[1];
    uint32_t c = key[2];
    uint32_t d = key[3];

    return (a << 24) | (b << 16) | (c << 8) | (d);
}

void BLOWFISH::generate_keys(unsigned char key[], unsigned key_length)
{
    for (unsigned i = 0; i < 18; i++)
        P[i] = P[i] ^ LOAD32(key + ((i * 4) % key_length));
}

uint32_t BLOWFISH::F(uint32_t data)
{
    uint32_t temp = Sbox[0][data >> 24] + Sbox[1][data >> 16 & 0xff];
    return (temp ^ Sbox[2][data >> 8 & 0xff]) + Sbox[3][temp & 0xff];
}

void BLOWFISH::cipher(unsigned char in[], unsigned char out[])
{
    uint32_t left = LOAD32(in);
    uint32_t right = LOAD32(in + 4);
    uint32_t temp;
 
    for (unsigned round = 0; round < 16; round++)
    {
        left = left ^ P[round];
        temp = F(left);
        right = temp ^ right;

        std::swap(left, right);
    }
}