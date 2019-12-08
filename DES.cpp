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

    std::cout << K << std::endl;
}