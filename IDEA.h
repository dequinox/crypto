#ifndef _IDEA_H
#define _IDEA_H

#include <cstdint>

class IDEA
{
    public:
        IDEA();
        ~IDEA();

    public:
        unsigned char* encrypt(unsigned char in[], unsigned length, unsigned &output_length);
        unsigned char* decrypt(unsigned char in[], unsigned length);
        void set_key(unsigned char key[]);
    
    private:
        void cipher(unsigned char in[], unsigned char out[]);
        void decipher(unsigned char in[], unsigned char out[]);
        void generate_keys(unsigned char key[]);

    private:
        uint16_t multiply(uint16_t a, uint16_t b);

    private:
        uint16_t subkeys[52];
};

#endif /* _IDEA_H */