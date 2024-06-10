#include <iostream>
#include <fstream>
#include <string>
#include "aes_func.h"
using namespace std;

string file_to_string (const string& name)
{
	string res, s;
	ifstream in (name);

	while (getline(in, s))
	{
		res += (s + "\n");
	}

	in.close();
	return res;
}

int main()
{
    string text = "jgf опа опа опа 12 qweкен!!";//file_to_string("text.txt"); 
    string textEncrypt = aes_encrypt(text, "parol");

    ofstream encrypt ("encrypt.txt");
    encrypt << textEncrypt;
    encrypt.close();

    textEncrypt = file_to_string("encrypt.txt");
    string textDecrypt = aes_decrypt(textEncrypt, "parol");

    ofstream decrypt ("decrypt.txt");
    decrypt << textDecrypt;
    decrypt.close();

    cout << text << endl;
    cout << textEncrypt << endl;
    cout << textDecrypt << endl;

    string s4 = snow3g_code (text, "parol", 1234);
    cout << endl << s4 << endl;
    cout << snow3g_code (s4, "parol", 1234) << endl;

    string s5 = A1Z26_encrypt (text);
    cout << endl << s5 << endl;
    cout << A1Z26_decrypt (s5) << endl;
}