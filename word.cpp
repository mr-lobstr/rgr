#include "byte_word_block.h"
using namespace std;

DWord::DWord (u32 u) 
    : bytes {
        u >> 24 & 0xff,
        u >> 16 & 0xff,
        u >> 8 & 0xff,
        u & 0xff
    }
{}


DWord::DWord (byte_t b0, byte_t b1, byte_t b2, byte_t b3)
    : bytes {b0, b1, b2, b3}
{}


DWord::operator uint32_t ()
{
    return static_cast<uint32_t> (bytes[0]) << 24
         | static_cast<uint32_t> (bytes[1]) << 16
         | static_cast<uint32_t> (bytes[2]) << 8
         | static_cast<uint32_t> (bytes[3]);
}


byte_t & DWord::operator[] (size_t i)
{
    return bytes[i];
}


byte_t const& DWord::operator[] (size_t i) const
{
    return bytes[i];
}


byte_t * DWord::begin ()
{
    return bytes;
}


byte_t * DWord::end ()
{
    return this->begin() + sizeof(DWord);
}


byte_t const* DWord::begin () const
{
    return bytes;
}


byte_t const* DWord::end () const
{
    return this->begin() + sizeof(DWord);
}


ostream & operator<<(ostream &out, DWord const& w)
{
    for (auto b : w.bytes)
    {
        out << hex << (int) b << " ";
    }
    return out;
}