#include "RC4.h"

RC4::RC4(unsigned int key[], int N)
{
    for (i = 0; i < 255; i++)
    {
        s[i] = i;
        k[i] = key[i % N];
    }

    j = 0;
    for (i = 0; i < 255; i++)
    {
        j = (j + s[i] + k[i]) % 256;
        std::swap(s[i], s[j]);
    }
}

unsigned int RC4::getByte()
{
    i = (i + 1) % 256;
    j = (j + s[i]) % 256;
    
    std::swap(s[i], s[j]);
    int t = (s[i] + s[j]) % 256;

    return s[t];
}
