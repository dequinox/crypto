#ifndef _A51_H
#define _A51_H

	const int R1Mask = 0x07FFFF; /* 19 bits */
	const int R2Mask = 0x3FFFFF; /* 22 bits */
	const int R3Mask = 0x7FFFFF; /* 23 bits */
	const int R1MidBit = 0x000100; /* bit 8 */
	const int R2MidBit = 0x000400; /* bit 10 */
	const int R3MidBit = 0x000400; /* bit 10 */

	const int R1Taps = 0x072000; /* bits 18,17,16,13 */
	const int R2Taps = 0x300000; /* bits 21,20 */
	const int R3Taps = 0x700080; /* bits 22,21,20,7 */
	const int R1Out = 0x040000; /* bit 18 (the high bit) */
	const int R2Out = 0x200000; /* bit 21 (the high bit) */
	const int R3Out = 0x400000; /* bit 22 (the high bit) */

    int parity(int r)
    {
        r ^= r >> 16;
        r ^= r >> 8;
        r ^= r >> 4;
        r ^= r >> 2;
        r ^= r >> 1; 
        return r & 1;
    }

    int majority(int R1, int R2, int R3)
    {
        if (parity(R1 & R1MidBit) + parity(R2 & R2MidBit) + parity(R3 & R3MidBit) >= 2)
            return 1;

        return 0;
    }

    int clockRegister(int reg, int mask, int taps)
    {
        int t = reg & taps;
        reg = (reg << 1) & mask;
        reg |= parity(t);

        return reg;
    }

    void clock(int &R1, int &R2, int &R3)
    {
        int maj = majority(R1, R2, R3);

        if (R1 & R1MidBit != 0 && maj == 1)
            R1 = clockRegister(R1, R1Mask, R1Taps);
        if (R2 & R2MidBit != 0 && maj == 1)
            R2 = clockRegister(R2, R2Mask, R2Taps);
        if (R3 & R3MidBit != 0 && maj == 1)
            R3 = clockRegister(R1, R3Mask, R3Taps);
    }    

    int getOutputBit(int R1, int R2,int R3)
    {
        return parity(R1 & R1Out) ^ parity(R2 & R2Out) ^ parity(R3 & R3Out);
    }

#endif /* _A51 */