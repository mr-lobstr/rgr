#ifndef CONSOLE_INTERFACE_h
#define CONSOLE_INTERFACE_h

#include <map>
#include <tuple>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "ciphers.h"

extern std::map<std::string, std::string> menu;

void check_password (std::string const&);
std::tuple <int, int, int, int> enter_parameters (std::string const&);
void enter_text (std::tuple <int, int, int, int> const&, std::string const&);
void expectation ();

#endif