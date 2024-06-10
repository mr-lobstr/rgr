#include <vector>
#include "byte_word_block.h"
using namespace std;

// ------------------------------ для Block128 ------------------------------
pair<vector<Block128>, size_t>
string_to_blocks (const string& s)
{
    vector<Block128> blocks (s.size() / 16 + 1);
    auto it = s.begin();

    for (auto& block : blocks)
    {
        for (auto& byte : block)
        {
            if (it == s.end())
                break;
            
            byte = *it++;
        }
    }

    return { blocks, 16 - (s.size() % 16) };
}


string blocks_to_string (vector<Block128> const& blocks)
{
    string res;
    
    for (auto const& block : blocks)
    {
        copy(block.begin(), block.end(), back_inserter(res));
    }
    return res;
}


// ------------------------------ общие функции ------------------------------
void mul_by_column (Block128& block, Block128 const& mtx, size_t n)
{
    Word temp;
    
    for (size_t i = 0; i < 4; ++i)
    {
        for (size_t j = 0; j < 4; ++j)
        {
            temp[i] ^= mul(mtx[i][j], block[j][n]);
        }
    }

    for (size_t i = 0; i < 4; ++i)
    {
        block[i][n] = temp[i];
    }
}


void mix_columns (Block128& block, Block128 const& mtx)
{
    for (size_t i = 0; i < 4; ++i)
    {
        mul_by_column (block, mtx, i);
    }
}


void add_round_key (Block128& block, const Block128& key)
{
    transform (block.begin(), block.end(), key.begin(), block.begin(), bit_xor<>{});   
}


// ------------------------------ функции для зашифровки ------------------------------
void shift_rows (Block128& block)
{
    rot_bytes_left(block[1].begin(), block[1].end(), 3);
    rot_bytes_left(block[2].begin(), block[2].end(), 2);
    rot_bytes_left(block[3].begin(), block[3].end(), 1);
}


void encrypt_block (Block128& block, vector<Block128> const& keys)
{
    Block128 mtx = {
        0x02030101,
        0x01020301,
        0x01010203,
        0x03010102
    };
    
    add_round_key(block, keys[0]);

    for (size_t i = 1; i <= 10 - 1; ++i)
    {
        sub_bytes(block.begin(), block.end(), sBox);
        shift_rows(block);
        mix_columns(block, mtx);
        add_round_key(block, keys[i]);
    }

    sub_bytes(block.begin(), block.end(), sBox);
    shift_rows(block);
    add_round_key(block, keys[10]);
}


// ------------------------------ функции для расшифровки ------------------------------
void inv_shift_rows (Block128& block)
{
    rot_bytes_left(block[1].begin(), block[1].end(), 1);
    rot_bytes_left(block[2].begin(), block[2].end(), 2);
    rot_bytes_left(block[3].begin(), block[3].end(), 3);
}


void decrypt_block (Block128& block, vector<Block128> const& keys)
{
    Block128 mtx = {
        0x0e0b0d09,
        0x090e0b0d,
        0x0d090e0b,
        0x0b0d090e,
    };

    add_round_key(block, keys[10]);
    inv_shift_rows(block);
    sub_bytes(block.begin(), block.end(), inv_sBox);
    
    for (size_t i = 10-1; i >= 1; --i)
    {
        add_round_key(block, keys[i]);
        mix_columns(block, mtx);
        sub_bytes(block.begin(), block.end(), inv_sBox);
        inv_shift_rows(block);
    }

    add_round_key(block, keys[0]);
}


// ------------------------------ зашифровка и расшифровка ------------------------------
vector<Block128> key_expansion (const Block128& ikey, int Np);

string aes_encrypt (string const& str, string const& pass)
{
    Block128 masterKey = password_hash (pass);
    vector<Block128> keys = key_expansion(masterKey, 10);

    auto [blocks, cnt] = string_to_blocks(str);

    for (auto& block : blocks)
    {
        encrypt_block(block, keys);
    }

    return blocks_to_string(blocks);
}


string aes_decrypt (string const& str, string const& pass)
{
    Block128 masterKey = password_hash (pass);
    vector<Block128> keys = key_expansion(masterKey, 10);

    auto [blocks, cnt]  = string_to_blocks(str);

    for (auto& block : blocks)
    {
        decrypt_block(block, keys);
    }

    auto s = blocks_to_string(blocks);
    s.erase(s.end() - cnt, s.end());
    return s;
}