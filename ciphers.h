#ifndef AES_FUNC_H
#define AES_FUNC_H

#include <string>

std::string aes_encrypt (std::string const& str, std::string const&);
std::string aes_decrypt (std::string const& str, std::string const&);

std::string snow3g_code (std::string, std::string const&, std::size_t);

std::string A1Z26_encrypt (std::string const&);
std::string A1Z26_decrypt (std::string const&);

#endif