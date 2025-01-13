#include <iostream>
#include <windows.h>
#include "console_interface.h"
using namespace std;

int main()
{
	SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string usersPass = "abs";
    string ciphersPass = "12345";

    system("cls");
    cout << menu["hello"] << endl;
    check_password (usersPass);

    while (true)
    {
        try
        {
            auto params = enter_parameters(ciphersPass);

            if (get<0>(params) == 0)
            {
                system("cls");
                break;
            }

            enter_text(params, ciphersPass);
        }
        catch (runtime_error const& e)
        {
            system("cls");
            cout << e.what() << endl;
            expectation();
        }
    }
}