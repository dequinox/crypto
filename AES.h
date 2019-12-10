#ifndef _AES_H
#define _AES_H

#include <string>

enum Standard {
    AES_128,
    AES_192,
    AES_256
};

class AES
{
    public:
        AES();

    public:
        std::string encrypt(std::string _data);
        std::string decrypt(std::string _data);

    private:
        std::byte* cipher(std::byte in[]);
    private:
        void add_round_key();
        void inv_mix_columns();
        void inv_shift_rows();
        void inv_sub_bytes();
        void mix_columns();

    private:
        void rot_word();
        void shift_rows();
        void sub_bytes();
        void sub_word();

    private:
        unsigned key_length;
        unsigned block_size;
        unsigned number_of_rounds;
};

#endif /* _AES_H */