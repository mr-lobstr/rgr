#include <vector>
#include <tuple>
#include "byte_word_block.h"
using namespace std;

u32 mul_alpha (byte_t b)
{
    return DWord {
        mulX_pow(b, 23,  0xa9),
        mulX_pow(b, 245, 0xa9),
        mulX_pow(b, 48,  0xa9),
        mulX_pow(b, 239, 0xa9) 
    };
}


u32 div_alpha (byte_t b)
{
    return DWord {
        mulX_pow(b, 16, 0xa9),  
        mulX_pow(b, 39, 0xa9),
        mulX_pow(b, 6,  0xa9),
        mulX_pow(b, 64, 0xa9)
    };
}


u32 s1 (u32 _w)
{
    DWord w = _w;
    sub_bytes (w.begin(), w.end(), sBox);
    auto [b0, b1, b2, b3] = w.bytes;

    return DWord {
        mulX(b0, 0x1b) ^ mulX(b3, 0x1b) ^ b3 ^ b1 ^ b2,
        mulX(b0, 0x1b) ^ mulX(b1, 0x1b) ^ b0 ^ b2 ^ b3,
        mulX(b1, 0x1b) ^ mulX(b2, 0x1b) ^ b3 ^ b1 ^ b0,
        mulX(b2, 0x1b) ^ mulX(b3, 0x1b) ^ b2 ^ b1 ^ b0
    };
}


u32 s2 (u32 _w)
{
    DWord w = _w;
    sub_bytes (w.begin(), w.end(), sBox_q);
    auto [b0, b1, b2, b3] = w.bytes;

    return DWord {
        mulX(b0, 0x69) ^ mulX(b3, 0x69) ^ b1 ^ b2 ^ b3,
        mulX(b0, 0x69) ^ mulX(b1, 0x69) ^ b0 ^ b2 ^ b3,
        mulX(b1, 0x69) ^ mulX(b2, 0x69) ^ b0 ^ b1 ^ b3,
        mulX(b2, 0x69) ^ mulX(b3, 0x69) ^ b0 ^ b1 ^ b2
    };
}


// ------------------------ раздел 1.2.2.1 ------------------------
void LFSR_init_mode (vector<u32>& LFSR, u32 f)
{
    auto [a0, a1, a2, a3] = static_cast<DWord>(LFSR[0]).bytes;
    auto [b0, b1, b2, b3] = static_cast<DWord>(LFSR[11]).bytes;

    u32 newS0  = DWord {a1, a2, a3, 0};
    u32 newS11 = DWord {0, b0, b1, b2};

    u32 lastWord = newS0 ^ newS11 ^ mul_alpha(a0) ^ div_alpha(b3) ^ LFSR[2] ^ f;

    rotate(begin(LFSR), begin(LFSR) + 1, end(LFSR));
    LFSR[15] = lastWord;
}


void LFSR_keystream_mode (vector<u32>& LFSR)
{
    LFSR_init_mode(LFSR, 0);
}


u32 clock_FSM (vector<u32>& LFSR, vector<u32>& FSM)
{
    u32 F = (LFSR[15] + FSM[1]) ^ FSM[2];
    u32 r = FSM[2] + (FSM[3] ^ LFSR[5]);

    FSM[3] = s2(FSM[2]);
	FSM[2] = s1(FSM[1]);
	FSM[1] = r;
	return F;
}


pair<vector<u32>, vector<u32>>
initialize (Block128 masterKey, Block128 initV)
{
    vector<u32> LFSR(16);
    vector<u32> FSM(4);
    
    for (size_t i = 0; i < 16; ++i)
    {
        LFSR[i] = masterKey[i % 4] ^ (i % 8 < 4 ? 0xffffffff : 0x0);
    }

    LFSR[15] ^= initV[0];
    LFSR[12] ^= initV[1];
    LFSR[10] ^= initV[2];
    LFSR[9]  ^= initV[3];

    for (size_t i = 0; i < 32; ++i)
    {
        u32 F = clock_FSM(LFSR, FSM);
        LFSR_init_mode(LFSR, F);
    }

    return {LFSR, FSM};
}


vector<DWord> gen_key_stream (vector<u32>& LFSR, vector<u32>& FSM, size_t n)
{
    clock_FSM(LFSR, FSM);
    LFSR_keystream_mode(LFSR);
    vector<DWord> keys(n);

    for (auto& el : keys)
    {
        el = clock_FSM(LFSR, FSM) ^ LFSR[0];
        LFSR_keystream_mode(LFSR);
    }

    return keys;
}


string snow3g_code (string str, string const& pass, size_t fresh)
{
    size_t keyCnt = str.size() / 4 + 1;
    Block128 masterKey = password_hash(pass);
    Block128 initV {
        (keyCnt << 27) | (fresh << 26),
        keyCnt,
        (keyCnt << 27) | (fresh << 26),
        keyCnt,
    };

    auto [LFSR, FSM] = initialize (masterKey, initV);
    auto keys = gen_key_stream(LFSR, FSM, keyCnt);

    auto it = str.begin();

    for (auto const& key : keys)
    {
        if (it == str.end())
            break;

        for (auto byte : key)
        {
            *it++ ^= byte;
        }        
    }

    return str;
}