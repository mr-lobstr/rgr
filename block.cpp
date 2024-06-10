#include "byte_word_block.h"
using namespace std;

Block128::Block128 (u32 u0, u32 u1, u32 u2, u32 u3)
    : words {u0, u1, u2, u3}
{}


Word & Block128::operator[] (size_t i)
{
    return words[i];
}


Word const& Block128::operator[] (size_t i) const
{
    return words[i];
}


byte_t * Block128::begin ()
{
    return words[0].begin();
}


byte_t const* Block128::begin () const
{
    return words[0].begin();
}


byte_t * Block128::end ()
{
    return this->begin() + sizeof(Block128);
}


byte_t const* Block128::end () const
{
    return this->begin() + sizeof(Block128);
}


ostream & operator<<(ostream &out, Block128 const& block)
{
    for (auto w : block.words)
    {
        out << w << endl;
    }
    return out;
}

Block128 password_hash (string const& s)
{
    size_t m = s.size() / 2;
    size_t h1 = hash<string>{} (s.substr(0, m));
    size_t h2 = hash<string>{} (s.substr(m, m-1));

    return {
        h1 >> 32 & 0xffffffff,
        h1 & 0xffffffff,
        h2 >> 32 & 0xffffffff,
        h2 & 0xffffffff,
    };
}