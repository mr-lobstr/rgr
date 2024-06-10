#include <iostream>
#include <string>
using namespace std;

string A1Z26_encrypt (string const& str)
{
    string res;

    for (unsigned char c : str)
    {
        unsigned n = static_cast<unsigned> (c);
        string s = to_string(n);
        res += string(3 - s.size(), '0') + s;
    }

    return res;
}


string A1Z26_decrypt (string const& str)
{
    string res;

    for (size_t i = 0; i < str.size(); i += 3)
    {
        char n = stoi(str.substr(i, 3));
        res.push_back(n);
    }

    return res;
}