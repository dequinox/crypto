#ifndef _DES_H
#define _DES_H

#include <string>
#include <bitset>

class DES 
{
	public:
		DES();
		void set_key(std::string _str);

	public:
		std::string encrypt(std::string _data);
		std::string decrypt(std::string _data);

	private:
		void initialise_subkeys();

	private:
		std::bitset<64> string2key(std::string _str);
		const std::string hex2bin(char _c) const;
		std::bitset<28> left_shift(std::bitset<28> _key, unsigned _index);
	
	private:
		std::bitset<64> key;
		std::bitset<48> subkeys[16];
		unsigned short PC_1[56] = {
			57,   49,    41,   33,    25,    17,    9,
             1,   58,    50,   42,    34,    26,   18,
            10,    2,    59,   51,    43,    35,   27,
            19,   11,     3,   60,    52,    44,   36,
            63,   55,    47,   39,    31,    23,   15,
             7,   62,    54,   46,    38,    30,   22,
            14,    6,    61,   53,    45,    37,   29,
            21,   13,     5,   28,    20,    12,    4
		};
		unsigned short PC_2[48] = {
			14,    17,   11,    24,     1,    5,
            3,     28,   15,     6,    21,   10,
            23,    19,   12,     4,    26,    8,
            16,     7,   27,    20,    13,    2,
            41,    52,   31,    37,    47,   55,
            30,    40,   51,    45,    33,   48,
            44,    49,   39,    56,    34,   53,
        	46,    42,   50,    36,    29,   32
		};
		unsigned short Rotations[16] = {
			1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
		};
};

#endif /* _DES_H */