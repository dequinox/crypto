#include "IDEA.h"
#include "bitset"
#include "iostream"

IDEA::IDEA()
{
}

IDEA::~IDEA()
{
}

unsigned char* IDEA::encrypt(unsigned char in[], unsigned length, unsigned &output_length)
{
}

unsigned char* IDEA::decrypt(unsigned char in[], unsigned length)
{
}

void IDEA::set_key(unsigned char key[])
{
    generate_keys(key);
}

uint16_t IDEA::multiply(uint16_t a, uint16_t b)
{
    uint32_t c = a * b;

    if(c != 0)
       c = ((ROL32(c, 16) - c) >> 16) + 1;
    else
       c = 1 - a - b;
    
    return c & 0xFFFF;
}

void IDEA::cipher(unsigned char in[], unsigned char out[])
{
    uint16_t* state = new uint16_t[4];
    for (unsigned int i = 0; i < 4; i++)
    {
        state[i] = (in[i * 2] << 8) | in[i * 2 + 1];
    }

    for (unsigned round = 0; round < 8; round++)
    {
        uint16_t step1 = multiply(state[0], subkeys[round * 6]);
        uint16_t step2 = state[1] + subkeys[round * 6 + 1];
        uint16_t step3 = state[2] + subkeys[round * 6 + 2];
        uint16_t step4 = multiply(state[3], subkeys[round * 6 + 3]);
        uint16_t step5 = step1 ^ step3;
        uint16_t step6 = step2 ^ step4;
        uint16_t step7 = multiply(step5, subkeys[round * 6 + 4]);
        uint16_t step8 = step6 + step7;
        uint16_t step9 = multiply(step8, subkeys[round * 6 + 5]);
        uint16_t step10 = step7 + step9;
        uint16_t step11 = step1 ^ step9;
        uint16_t step12 = step3 ^ step9;
        uint16_t step13 = step2 ^ step10;
        uint16_t step14 = step4 ^ step10;

        state[0] = step11;
        state[1] = step13;
        state[2] = step12;
        state[4] = step14;
    }

    state[0] = multiply(state[0], subkeys[48]);
    state[1] = state[1] + subkeys[49];
    state[2] = state[2] + subkeys[50];
    state[3] = multiply(state[3], subkeys[51]);

    for (unsigned i = 0; i < 4; i++)
    {
        out[i * 2] = state[i] >> 8;
        out[i * 2 + 1] = state[i] & 0xff;
    }

}

void IDEA::decipher(unsigned char in[], unsigned char out[])
{
}

void IDEA::generate_keys(unsigned char key[])
{
    std::bitset<896> bitkey;

    for (unsigned i = 0; i < 16; i++)
        for (int j = 0; j < 8; j++)
            bitkey[i * 8 + j] = (key[i] >> j) & 1;
    
    unsigned shift = 0;
    for (unsigned i = 16; i < 56; i += 8)
    {
        shift += 25;
        for (unsigned j = 0; j < 128; j++)
        {
            bitkey[i * 8 + j] = bitkey[(j + shift) % 128];
        }
    }

    for (unsigned i = 0; i < 52; i++)
    {
        unsigned subkey = 1;
        unsigned mul = 1;
        for (unsigned j = 0; j < 16; j++)
        {
            subkey = subkey + (mul * bitkey[(i * 16) + j]);
            mul *= 16;
        }
        subkeys[i] = subkey;
    }
}

