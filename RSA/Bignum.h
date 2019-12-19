#ifndef _BIGNUM_H
#define _BIGNUM_H

#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>

class Bignum 
{
    public:
        Bignum();
        Bignum(long long other);
        Bignum(std::string other);

    public:
        void print();

    public:
        void random_fill(unsigned _size);
        void zero_fill(unsigned _size);
        Bignum abs() const;

    public:
        void operator=(const Bignum &other);
        Bignum operator-() const;
        Bignum operator+(const Bignum &other) const;
        Bignum operator-(const Bignum &other) const;
        Bignum operator*(const Bignum &other);

    public:
        bool operator<(const Bignum &other) const;
        bool operator>(const Bignum &other) const;
        bool operator<=(const Bignum &other) const;
        bool operator>=(const Bignum &other) const;
        bool operator!=(const Bignum &other) const;
        bool operator==(const Bignum &other) const;

    private:
        void trim();

    private:
        std::vector<int> data;
        int sign = 1;
};

#endif /* _BIGNUM */
