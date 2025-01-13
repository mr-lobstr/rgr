#include <vector>
#include "byte_word_block.h"
using namespace std;

DWord rCon (int round)
{
    byte_t highByte = (round == 10) ? 0x36
                    : (round == 9)  ? 0x1b
                    : 1 << (round - 1);

    return {highByte, 0, 0, 0};
}


DWord rotWord (DWord w)
{
    rot_bytes_left (w.begin(), w.end(), 1);
    return w;
}


DWord subWord(DWord w)
{
    sub_bytes(w.begin(), w.end(), sBox);
    return w;
}


vector<Block128> key_expansion (const Block128& masterKey, int Np)
{   
    vector<Block128> keys = { masterKey };

    for (size_t i = 1; i < Np + 1; ++i)
    {
        auto [p0, p1, p2, p3] = keys[i-1].words;

        DWord w0 = subWord(rotWord(p3)) ^ rCon(i) ^ p0;
        DWord w1 = w0 ^ p1;
        DWord w2 = w1 ^ p2;
        DWord w3 = w2 ^ p3;

        keys.push_back({w0, w1, w2, w3});
    }

    return keys;
}