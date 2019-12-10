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
        void cipher(std::byte in[], std::byte out[]);

    private:
        void add_round_key(std::byte** state);
        void inv_mix_columns(std::byte** state);
        void inv_shift_rows(std::byte** state);
        void inv_sub_bytes(std::byte** state);
        void mix_columns(std::byte** state);

    private:
        void rot_word();
        void shift_rows(std::byte** state);
        void sub_bytes(std::byte** state);
        void sub_word(std::byte** state);

    private:
        unsigned key_length;
        unsigned block_size;
        unsigned number_of_rounds;
};

#endif /* _AES_H */