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
	
	private:
		std::bitset<64> key;
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
};

#endif /* _DES_H */