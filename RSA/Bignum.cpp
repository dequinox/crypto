#include "Bignum.h"

Bignum::Bignum()
{
}

Bignum::Bignum(long long other)
{
    data.clear();

    if (other < 0)
    {
        other = -other;
        sign = -1;
    }
    while (other)
    {
        data.push_back(other % 10);
        other /= 10;
    }
}

Bignum::Bignum(std::string other)
{
    data.clear();
    int last = 0;

    if (other[last] == '-')
    {
        last = 1;
        sign = -1;
    }
    
    for (int i = other.length() - 1; i >= last; i--)
    {
        data.push_back(other[i] - '0');
    }
    
    trim();
}

void Bignum::print()
{
    if (sign == -1)
        std::cout << "-";
    
    for (int i = data.size() - 1; i >= 0; i--)
    {
        std::cout << data[i];
    }
}

void Bignum::random_fill(unsigned _size)
{
    data.clear();
    srand (time(NULL));
    for (unsigned i = 0; i < _size; i++)
    {
        data.push_back(rand() % 10);
    }
}

void Bignum::zero_fill(unsigned _size)
{
    data.clear();
    for (unsigned i = 0; i < _size; i++)
    {
        data.push_back(0);
    }
}

void Bignum::operator=(const Bignum &other)
{
    sign = other.sign;
    data = other.data;
}

Bignum Bignum::operator-() const
{
    Bignum result = (*this);
    result.sign = -sign;
    return result;
}

Bignum Bignum::abs() const 
{
    Bignum result = *this;
    result.sign *= result.sign;
    return result;
}

Bignum Bignum::operator+(const Bignum &other) const
{
    if (sign != other.sign)
        return *this - (-other);

    Bignum result;
    unsigned carry = 0;
    int sum;
    unsigned a, b;
    
    for (unsigned i = 0; i < std::max(data.size(), other.data.size()) || carry; i++)
    {
        a = data.size() > i ? data[i] : 0;
        b = other.data.size() > i ? other.data[i] : 0;
        
        sum = a + b + carry;
        carry = sum >= 10;
        result.data.push_back(sum % 10);
    }

    result.sign = sign;
    return result;
}

Bignum Bignum::operator-(const Bignum &other) const
{
    if (sign != other.sign)
        return *this + (-other);
    
    if (abs() < other.abs())
        return -(other - *this);
    
    Bignum result;
    unsigned carry = 0;
    int sum;
    int a, b;
    
    for (unsigned i = 0; i < std::max(data.size(), other.data.size()) || carry; i++)
    {
        a = data.size() > i ? data[i] : 0;
        b = other.data.size() > i ? other.data[i] : 0;
        
        sum = a - b - carry;
        carry = sum < 0;
        if (carry)
            sum += 10;
        
        result.data.push_back(sum);
    }

    result.sign = sign;
    result.trim();
    return result;
}

Bignum Bignum::operator*(const Bignum &other)
{
    Bignum result;
    unsigned carry = 0;
    int sum;
    
    result.zero_fill(data.size() + other.data.size());
    result.sign = sign * other.sign;

    for (unsigned i = 0; i < data.size(); i++)
    {
        for (unsigned j = 0; j < other.data.size(); j++)
        {
            sum = data[i] * other.data[j] + result.data[i + j] + carry;
            carry = (sum / 10);
            result.data[i + j] = sum % 10;
        }
        result.data[i + other.data.size()] += carry;
    }

    result.trim();
    return result;
}

bool Bignum::operator<(const Bignum &other) const 
{
    if (sign != other.sign)
        return sign < other.sign;
    if (data.size() != other.data.size())
        return data.size() * sign < other.data.size() * other.sign;
    
    for (int i = data.size() - 1; i >= 0; i--)
        if (data[i] != other.data[i])
            return data[i] * sign < other.data[i] * other.sign;

    return false; 
}

bool Bignum::operator>(const Bignum &other) const 
{
    return other < *this;
}

bool Bignum::operator<=(const Bignum &other) const 
{
    return !(other < *this);
}

bool Bignum::operator>=(const Bignum &other) const
{
    return !(*this < other);
}

bool Bignum::operator!=(const Bignum &other) const 
{
    return *this < other || other < *this;
}

bool Bignum::operator==(const Bignum &other) const 
{
    return !(*this < other) && !(other < *this);
}

void Bignum::trim()
{
    while (data.size() > 1 && data[data.size() - 1] == 0)
        data.pop_back();
}