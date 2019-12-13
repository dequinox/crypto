#include "RC6.h"

RC6::RC6()
{
}

void RC6::set_key(unsigned char key[], unsigned key_length)
{
    generate_keys(key, key_length);
}

unsigned char* RC6::encrypt(unsigned char in[], unsigned length, unsigned& output_length)
{
    output_length = (((length - 1) / 16) + 1) * 16; 
    unsigned char *out = new unsigned char[output_length];  
    unsigned char chunk[16];

    for (unsigned i = 0; i < length; i += 16)
    {
        for (unsigned j = 0; j < 16; j++)
        {
            chunk[j] = 0;
            if (i + j < length)
                chunk[j] = in[i + j];
        }
        
        cipher(chunk, out + i);
    }

    return out;
}

unsigned char* RC6::decrypt(unsigned char in[], unsigned int length)
{
    unsigned char *out = new unsigned char[length];  
    unsigned char chunk[16];

    for (unsigned i = 0; i < length; i += 16)
    {
        for (unsigned j = 0; j < 16; j++)
        {
            chunk[j] = in[i + j];
        }
        
        decipher(chunk, out + i);
    }

    return out;
}

uint32_t max(uint32_t a, uint32_t b)
{
    return a > b ? a : b;
}

uint32_t RC6::rotate_left(uint32_t a, uint32_t b)
{
    b <<= (w - log_w);
    b >>= (w - log_w);

    return (a << b) | (a >> (w - b));
}

void RC6::generate_keys(unsigned char key[], unsigned key_length)
{
    unsigned c = key_length / 4;
    uint32_t L[c];
    S = new uint32_t[2 * number_of_rounds + 4];

    for (unsigned i = 0; i < c; i++)
    {
        L[i] = (key[i * 4] << 24) | (key[i * 4] << 16) | (key[i * 4] << 8) | (key[i * 4]);
    }
    S[0] = P;

    for (unsigned i = 1; i < number_of_rounds * 2 + 4; i++)
    {
        S[i] = S[i - 1] + Q;
    }

    uint32_t A = 0, B = 0, i = 0, j = 0;
    unsigned v = 3 * max(c, 2 * number_of_rounds + 4);

    for (unsigned s = 0; s < v; s++)
    {
        A = S[i] = rotate_left(S[i] + A + B, 3);
        B = L[j] = rotate_left(L[j] + A + B, A + B);
        i = (i + 1) % (2 * number_of_rounds + 4);
        j = (j + 1) % c;
    }
}

void cipher(unsigned char in[], unsigned char out[])
{
}

void decipher(unsigned char in[], unsigned char out[])
{
}
