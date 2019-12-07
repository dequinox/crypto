#ifndef _RC4_H
#define _RC4_H

#include <algorithm> 

class RC4
{
    public:
        RC4(unsigned int key[], int length);
        unsigned int getByte();
  
    private:
        int s[256], k[256];
        int i, j;
};

#endif /* _RC4_H */