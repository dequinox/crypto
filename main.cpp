#include <iostream>
#include <iomanip>
#include "DES/DES.h"
#include "AES/AES.h"
#include "IDEA/IDEA.h"
#include "BLOWFISH/BLOWFISH.h"

using namespace std;

int main()
{
    /*DES cipher;
    cipher.set_key("133457799BBCDFF1"); 
    cout << cipher.encrypt("0123456789ABCDEF") << endl;
    cout << cipher.decrypt("85E813540F0AB405") << endl;
    */

    unsigned char in[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    unsigned char in2[16] = {0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30, 0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a};
    unsigned char key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f}; 
    unsigned char *out, *out2;

    /*AES c(AES_128);
    c.set_key(key);

    unsigned output_len;

    out = c.encrypt(in, 16, output_len);

    for (unsigned i = 0; i < output_len; i++)
    {
        std::cout << std::setfill('0') << std::setw(2) << std::hex << (0xff & (unsigned int)out[i]);
    }
    std::cout << std::endl;

    out2 = c.decrypt(in2, 16);
    for (unsigned i = 0; i < 16; i++)
    {
        std::cout << std::setfill('0') << std::setw(2) << std::hex << (0xff & (unsigned int)out2[i]);
    }
    std::cout << std::endl;
    */

   unsigned char key2[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

   BLOWFISH b;
   b.set_key(key2, 8);

    return 0;
}