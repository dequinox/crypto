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

unsigned char* BLOWFISH::encrypt(unsigned char in[], unsigned int length, unsigned int& output_length)
{
    output_length = (((length - 1) / 8) + 1) * 8; 
    unsigned char *out = new unsigned char[output_length];  
    unsigned char chunk[8];

    for (unsigned i = 0; i < length; i += 8)
    {
        for (unsigned j = 0; j < 8; j++)
        {
            chunk[j] = 0;
            if (i + j < length)
                chunk[j] = in[i + j];
        }
        
        cipher(chunk, out + i);
    }

    return out;
}

unsigned char* BLOWFISH::decrypt(unsigned char in[], unsigned int length)
{
    unsigned char *out = new unsigned char[length];  
    unsigned char chunk[8];

    for (unsigned i = 0; i < length; i += 8)
    {
        for (unsigned j = 0; j < 8; j++)
        {
            chunk[j] = in[i + j];
        }
        
        decipher(chunk, out + i);
    }

    return out;
}

uint32_t LOAD32(unsigned char key[])
{
    uint32_t a = key[0];
    uint32_t b = key[1];
    uint32_t c = key[2];
    uint32_t d = key[3];

    return (a << 24) | (b << 16) | (c << 8) | (d);
}

void UPLOAD32(uint32_t data, unsigned char out[])
{
    out[0] = data >> 24;
    out[1] = data >> 16 & 0xff;
    out[2] = data >> 8 & 0xff;
    out[3] = data & 0xff;
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

    left ^= P[16];
    right ^= P[17];
    std::swap(left, right);

    UPLOAD32(left, out);
    UPLOAD32(right, out + 4);
}

void BLOWFISH::decipher(unsigned char in[], unsigned char out[])
{
    uint32_t left = LOAD32(in);
    uint32_t right = LOAD32(in + 4);
    uint32_t temp;
 
    for (unsigned round = 17; round > 1; round--)
    {
        left = left ^ P[round];
        temp = F(left);
        right = temp ^ right;

        std::swap(left, right);
    }

    left ^= P[1];
    right ^= P[0];
    std::swap(left, right);

    UPLOAD32(left, out);
    UPLOAD32(right, out + 4);
}