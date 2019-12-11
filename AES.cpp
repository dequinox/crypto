#include "AES.h"
#include <iostream>
#include <iomanip>

unsigned char* AES::encrypt(unsigned char in[], unsigned int length, unsigned int& output_length)
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

unsigned char* AES::decrypt(unsigned char in[], unsigned int length)
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

AES::AES()
{
    AES(AES_128);
}

AES::AES(Standard st)
{
    block_size = 4;
    
    switch(st)
    {
        case AES_128:
            key_length = 4;
            number_of_rounds = 10;
            break;

        case AES_192:
            key_length = 6;
            number_of_rounds = 12;
            break;
        
        case AES_256:
            key_length = 8;
            number_of_rounds = 14;
        
        default:
            key_length = 4;
            number_of_rounds = 10;
    }

    w = new unsigned char [(number_of_rounds + 1) * block_size * 4];
}

AES::~AES()
{
    delete[] w;
}

void AES::set_key(unsigned char key[])
{
    generate_keys(key);
}

void print(unsigned char **state)
{
    for (unsigned col = 0; col < 4; col++)
    {
        for (unsigned row = 0; row < 4; row++)
        {
            std::cout << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)state[row][col];
        }
    }
    std::cout << std::endl;
}

void print(unsigned char *state)
{
    for (unsigned col = 0; col < 16; col++)
    {
            std::cout << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)state[col];
    }
    std::cout << std::endl;
}


void AES::cipher(unsigned char in[], unsigned char out[])
{
    unsigned char **state = new unsigned char *[4];
    for (unsigned  i = 0; i < 4; i++)
        state[i] = new unsigned char[block_size];

    for (unsigned col = 0; col < block_size; col++)
        for (unsigned row = 0; row < 4; row++)
            state[row][col] = in[row + col * 4];

    add_round_key(state, w);

    for (unsigned round = 1; round <= number_of_rounds - 1; round++)
    {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, w + round * block_size * 4);
    }

    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, w + number_of_rounds * block_size * 4);

    for (unsigned row = 0; row < 4; row++)
        for (unsigned col = 0; col < block_size; col++)
            out[row + col * 4] = state[row][col];

    for (unsigned i = 0; i < 4; i++)
        delete[] state[i];
        
    delete[] state;
}

void AES::decipher(unsigned char in[], unsigned char out[])
{
    unsigned char **state = new unsigned char *[4];
    for (unsigned  i = 0; i < 4; i++)
        state[i] = new unsigned char[block_size];

    for (unsigned col = 0; col < block_size; col++)
        for (unsigned row = 0; row < 4; row++)
            state[row][col] = in[row + col * 4];
    
    add_round_key(state, w + number_of_rounds * block_size * 4);

    for (unsigned round = number_of_rounds - 1; round >= 1; round--)
    {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(state, w + round * block_size * 4);
        inv_mix_columns(state);
    }

    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_round_key(state, w);

    for (unsigned row = 0; row < 4; row++)
        for (unsigned col = 0; col < block_size; col++)
            out[row + col * 4] = state[row][col];

    for (unsigned i = 0; i < 4; i++)
        delete[] state[i];
        
    delete[] state;
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

void AES::inv_sub_bytes(unsigned char **state)
{
    for (unsigned col = 0; col < block_size; col++)
        for (unsigned row = 0; row < 4; row++)
        {
            unsigned char num = state[row][col];
            state[row][col] = InvSbox[num / 16][num % 16];
        }
}

void AES::inv_shift_rows(unsigned char **state)
{
    unsigned char temp[block_size];
    for (unsigned row = 1; row < 4; row++)
    {
        for (unsigned col = 0; col < block_size; col++)
        {
            temp[col] = state[row][col];
        }
        for (unsigned col = 0; col < block_size; col++)
        {
            unsigned pos = (col >= row) ? col - row : col + block_size - row;
            state[row][col] = temp[pos];
        }
    }
}

void AES::shift_rows(unsigned char **state)
{
    unsigned char temp[block_size];
    for (unsigned row = 1; row < 4; row++)
    {
        for (unsigned col = 0; col < block_size; col++)
        {
            temp[col] = state[row][col];
        }
        for (unsigned col = 0; col < block_size; col++)
        {
            state[row][col] = temp[(col + row) % block_size];
        }
    }
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

void AES::inv_mix_columns(unsigned char **state)
{
    for (unsigned col = 0; col < block_size; col++)
    {
        unsigned char s0c = state[0][col]; 
        unsigned char s1c = state[1][col];  
        unsigned char s2c = state[2][col];
        unsigned char s3c = state[3][col];

        state[0][col] = multiply(0x0e, s0c) ^ multiply(0x0b, s1c) ^ multiply(0x0d, s2c) ^ multiply(0x09, s3c);
        state[1][col] = multiply(0x09, s0c) ^ multiply(0x0e, s1c) ^ multiply(0x0b, s2c) ^ multiply(0x0d, s3c);
        state[2][col] = multiply(0x0d, s0c) ^ multiply(0x09, s1c) ^ multiply(0x0e, s2c) ^ multiply(0x0b, s3c);
        state[3][col] = multiply(0x0b, s0c) ^ multiply(0x0d, s1c) ^ multiply(0x09, s2c) ^ multiply(0x0e, s3c); 
    }
}

unsigned char xtime(unsigned char x)
{
    if (x & 0x80)
    {
        x = x << 1;
        x = x ^ 0x1b;
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
        b = b >> 1;
    }

    return result;
}

void AES::add_round_key(unsigned char** state, unsigned char* key)
{
    for (unsigned row = 0; row < 4; row++)
        for (unsigned col = 0; col < block_size; col++)
            state[row][col] ^= key[row + col * 4];
}

void AES::generate_keys(unsigned char *key)
{
    unsigned i = 0;
    unsigned char* Rcon = new unsigned char[4];
    unsigned char* temp = new unsigned char[4];
    while (i < 4 * key_length)
    {
        w[i] = key[i];
        i++;
    }

    while (i < block_size * (number_of_rounds + 1) * 4)
    {
        temp[0] = w[i - 4];
        temp[1] = w[i - 3];
        temp[2] = w[i - 2];
        temp[3] = w[i - 1];

        if ((i / 4) % key_length == 0)
        {
            rot_word(temp);
            sub_word(temp);
            rcon(Rcon, (i / 4) / key_length);

            temp[0] = temp[0] ^ Rcon[0];
            temp[1] = temp[1] ^ Rcon[1];
            temp[2] = temp[2] ^ Rcon[2];
            temp[3] = temp[3] ^ Rcon[3];
        }
        else if (key_length > 6 && (i / 4) % key_length == 4)
            sub_word(temp);
        
        w[i] = w[i - 4 * key_length] ^ temp[0];
        w[i + 1] = w[i - 4 * key_length + 1] ^ temp[1];
        w[i + 2] = w[i - 4 * key_length + 2] ^ temp[2];
        w[i + 3] = w[i - 4 * key_length + 3] ^ temp[3];
        
        i += 4;
    }

    delete[] Rcon;
    delete[] temp;
}

void AES::rcon(unsigned char* Rcon, unsigned n)
{
    unsigned char result = 1;
    for (unsigned i = 0; i < n - 1; i++)
    {
        result = xtime(result);
    }

    Rcon[0] = result;
    Rcon[1] = Rcon[2] = Rcon[3] = 0;
}

void AES::sub_word(unsigned char* word)
{
    for (unsigned i = 0; i < 4; i++)
        word[i] = Sbox[word[i] / 16][word[i] % 16];
}

void AES::rot_word(unsigned char* word)
{
    for (unsigned i = 0; i < 3; i++)
    {
        std::swap(word[i], word[i + 1]);
    }
}