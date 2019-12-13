#ifndef _RC6_H
#define _RC6_H

#include <cstdint>

class RC6
{
    public:
        RC6();
    
    public:
        void set_key(unsigned char key[], unsigned key_length);
        unsigned char* encrypt(unsigned char in[], unsigned length, unsigned &output_length);
        unsigned char* decrypt(unsigned char in[], unsigned length);
    
    private:
        void generate_keys(unsigned char in[], unsigned key_length);
        void cipher(unsigned char in[], unsigned char out[]);
        void decipher(unsigned char in[], unsigned char out[]);

    private:
        uint32_t rotate_left(uint32_t a, uint32_t b);

    private:
        uint32_t* S;
        unsigned int number_of_rounds = 20;
        const uint32_t P = 0xb7e15163;
        const uint32_t Q = 0x9e3779b9;
        const unsigned w = 32;
        const unsigned log_w = 5;
};

#endif /* _RC6_H */