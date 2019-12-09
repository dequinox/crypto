#include "DES.h"
#include <bitset>
#include <iostream>

DES::DES()
{
}

void DES::set_key(std::string _str)
{
    key = string2key(_str);
    initialise_subkeys();
}

std::bitset<64> DES::string2key(std::string _str)
{
    std::string temp;
    for (int i = 0; i < _str.size(); i++)
    {
        temp += hex2bin(_str[i]);
    }
    return std::bitset<64>(temp);
}

const std::string DES::hex2bin(char _c) const
{
    switch (toupper(_c))
    {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
        default:  return "0000";
    }
}

void DES::initialise_subkeys()
{
    std::bitset<56> K;
    for (unsigned i = 0; i < 56; i++)
    {
        K[i] = key[63 - (PC_1[i] - 1)];
    }

    std::bitset<28> left, right;
    std::bitset<56> compressedKey;

    for (unsigned i = 0; i < 28; i++)
    {
        left[i] = K[i];
        right[i] = K[i + 28];
    }

    for (unsigned round = 0; round < 16; round++)
    {
        left = left_shift(left, Rotations[round]);
        right = left_shift(right, Rotations[round]);

        for (unsigned i = 0; i < 28; i++)
        {
            compressedKey[i] = left[i];
            compressedKey[i + 28] = right[i];
        }
    
        for (unsigned i = 0; i < 48; i++)
        {
            subkeys[round][47 - i] = compressedKey[PC_2[i] - 1];
        }
    }
}

std::bitset<64> DES::initial_permutation(std::bitset<64> _data)
{
    std::bitset<64> result;
    for (unsigned i = 0; i < 64; i++)
    {
        result[i] = _data[IP[i] - 1];
    }

    return result;
}

std::bitset<64> DES::final_permutation(std::bitset<64> _data)
{
    std::bitset<64> result;
    for (unsigned i = 0; i < 64; i++)
    {
        result[i] = _data[IP_1[i] - 1];
    }

    return result;
}

std::bitset<64> DES::encode(std::bitset<64> _data)
{
    std::bitset<64> ip;
    ip = initial_permutation(_data);

    std::bitset<32> last_left, last_right;
    std::bitset<32> left, right;
    for (unsigned i = 0; i < 32; i++)
    {
        last_left[i] = ip[i + 32];
        last_right[i] = ip[i];
    }

    for (unsigned round = 0; round < 16; round++)
    {
        left = last_right;
        right = last_left ^ F(last_right, subkeys[round]);

        last_left = left;
        last_right = right;
    }

    for (unsigned i = 0; i < 32; i++)
    {
        ip[63 - i] = right[31 - i];
        ip[63 - i - 32] = left[31 - i];   
    }

    std::cout << "before " << ip << std::endl;
    ip = final_permutation(ip);
    std::cout << "after " << ip << std::endl;
    return ip;
}

std::bitset<32> DES::F(std::bitset<32> _data, std::bitset<48> _key)
{
    std::bitset<48> extended_data;
    for (unsigned i = 0; i < 48; i++)
    {
        extended_data[i] = _data[E[i] - 1];
    }

    extended_data = extended_data ^ _key;

    std::bitset<32> temp;
    unsigned index = 0;
    for (unsigned i = 0; i < 48; i += 6)
    {
        unsigned row = extended_data[47 - i] * 2 + extended_data[47 - i - 5];
        unsigned col = extended_data[47 - i - 1] * 8 
                     + extended_data[47 - i - 2] * 4 
                     + extended_data[47 - i - 3] * 2 
                     + extended_data[47 - i - 4];
        unsigned num = S[i / 6][row][col];

        std::bitset<4> binary(num);
        for (unsigned j = 0; j < 4; j++)
            temp[31 - index - j] = binary[3 - j];

        index += 4;
    }

    std::bitset<32> result;
    for (unsigned i = 0; i < 32; i++)
    {
        result[31 - i] = temp[31 - (P[i] - 1)];
    }

    return result;
}

std::string DES::encrypt(std::string _data)
{
    std::bitset<64> m = string2key(_data);
    encode(m);
    return _data;
}

std::bitset<28> DES::left_shift(std::bitset<28> _key, unsigned _index)
{
    std::bitset<28> temp = _key;
    for (unsigned i = 0; i < 28; i++)
    {
        if (i + _index < 28)
            temp[i] = _key[i + _index];
        else
            temp[i] = _key[i + _index - 28];
    }

    return temp;
}