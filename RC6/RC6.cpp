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

uint32_t LOAD32(unsigned char key[])
{
    uint32_t a = key[0];
    uint32_t b = key[1];
    uint32_t c = key[2];
    uint32_t d = key[3];

    return (a << 24) | (b << 16) | (c << 8) | (d);
}

uint32_t LOAD32BE(unsigned char key[])
{
    uint32_t a = key[0];
    uint32_t b = key[1];
    uint32_t c = key[2];
    uint32_t d = key[3];

    return (d << 24) | (c << 16) | (b << 8) | (a);
}

void RC6::generate_keys(unsigned char key[], unsigned key_length)
{
    unsigned c = key_length / 4;
    uint32_t L[c];
    S = new uint32_t[2 * number_of_rounds + 4];

    for (unsigned i = 0; i < c; i++)
    {
        L[i] = LOAD32BE(key + i * 4);
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

void UPLOAD32BE(uint32_t data, unsigned char out[])
{
    out[0] = data >> 24;
    out[1] = data >> 16 & 0xff;
    out[2] = data >> 8 & 0xff;
    out[3] = data & 0xff;
}

void UPLOAD32LE(uint32_t data, unsigned char out[])
{
    out[3] = data >> 24;
    out[2] = data >> 16 & 0xff;
    out[1] = data >> 8 & 0xff;
    out[0] = data & 0xff;
}

void RC6::cipher(unsigned char in[], unsigned char out[])
{
    uint32_t A = LOAD32BE(in); 
    uint32_t B = LOAD32BE(in + 4);
    uint32_t C = LOAD32BE(in + 8);
    uint32_t D = LOAD32BE(in + 12);

    B = B + S[0];
    D = D + S[1];

    for (unsigned round = 1; round <= number_of_rounds; round++)
    {
        uint32_t t = rotate_left(B * (2*B + 1), log_w);
        uint32_t u = rotate_left(D * (2*D + 1), log_w);
        A = rotate_left(A ^ t, u) + S[2 * round];
        C = rotate_left(C ^ u, t) + S[2 * round + 1];

        uint32_t temp = A;
        A = B; B = C; C = D; D = temp;
    }

    A = A + S[2 * number_of_rounds + 2];
    C = C + S[2 * number_of_rounds + 3];

    UPLOAD32LE(A, out);
    UPLOAD32LE(B, out + 4);
    UPLOAD32LE(C, out + 8);
    UPLOAD32LE(D, out + 12);
}

void RC6::decipher(unsigned char in[], unsigned char out[])
{
}
