#ifndef BLOCK128_AND_WORD_H
#define BLOCK128_AND_WORD_H

#include <cstdint>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <string>


using byte_t = uint8_t;
using u32 = uint32_t;

byte_t sBox (byte_t);
byte_t sBox_q (byte_t);
byte_t inv_sBox (byte_t);

void sub_bytes (byte_t *, byte_t *, byte_t (*) (byte_t));
void rot_bytes_left (byte_t *, byte_t *, size_t);

byte_t mulX (byte_t, byte_t);
byte_t mulX_pow (byte_t, byte_t, byte_t);
byte_t mul (byte_t, byte_t);


struct DWord
{
public:
    byte_t bytes[4] = {};

public:    
    DWord () = default;
    DWord (u32);
    DWord (byte_t, byte_t, byte_t, byte_t);

    operator uint32_t ();

    byte_t & operator[] (std::size_t);
    byte_t const& operator[] (std::size_t ) const;

    byte_t * begin ();
    byte_t * end ();
    byte_t const* begin () const;
    byte_t const* end () const;  
};


struct Block128
{
public:
    DWord words [4] = {};

public:
    Block128 () = default;
    Block128 (u32, u32, u32, u32);

    DWord & operator[] (std::size_t);
    DWord const& operator[] (std::size_t) const;

    byte_t * begin ();
    byte_t * end ();
    byte_t const* begin () const;
    byte_t const* end () const;
};

Block128 password_hash (std::string const&);

std::ostream & operator<< (std::ostream&, DWord const&);
std::ostream & operator<< (std::ostream&, Block128 const&);

#endif