#include "console_interface.h"
using namespace std;

map<string, string> menu
{
    {"hello",               "The program welcomes you "
                            "\"The encoder\". "
                            "Please enter the password:" },
    
    {"ciphers_list_title",  "List of available ciphers:\n" },
    {"ciphers_list",        "<1> AES\n"
                            "<2> Snow3g\n"
                            "<3> A1Z26 (modified)\n"
                            "<0> Exit" },

    {"action_title",        "Text message ..\n" },
    {"action",              "<1> Encrypt\n"
                            "<2> Decrypt\n"
                            "<0> Exit" },
    
    {"in_method_title",     "Select the text message input method:\n" },
    {"in_method",           "<1> from the console\n"
                            "<2> from the file\n"
                            "<0> Exit" },

    {"out_method_title",    "Select the output method:\n" },
    {"out_method",          "<1> to the console\n"
                            "<2> to the file\n"
                            "<0> Exit" }
};

void expectation ()
{
    cout << endl << "Press any key to continue ";
    cin.get();
    cin.get();
}

string file_to_string (const string& name)
{
	ifstream in (name, ios::in | ios::binary | ios::ate);

    if (!in.is_open())
        throw runtime_error (
            "Error reading the file " + name + ": there is no such file or it cannot be opened"
        );

    ifstream::pos_type fileSize = in.tellg();
    in.seekg(0, ios::beg);

    vector<char> bytes(fileSize);
    in.read(bytes.data(), fileSize);

    return string(bytes.data(), fileSize);   
}

void output (const string& name, const string& text)
{
    ofstream out (name, ios::out | ios::binary);
    if (!out.is_open())
        throw runtime_error("Error writing to the file " + name);

    out << text;
    out.close();
}

void check_password (string const& pass)
{
    string usersPass;
    cin >> usersPass;

    while (usersPass != pass)
    {
        system("cls");
        cout << "Invalid password, please try again:" << endl;
        cin >> usersPass;
    }
}

int get_operations_code (int mn, int mx, string const& addMess)
{
    int n;
    cin >> n;

    while (n < mn || mx < n)
    {
        system("cls");
        cout << "Invalid operation code, please try again:" << endl
             << addMess << endl;
        cin >> n;
    }

    return n;
}

tuple <int, int, int, int> enter_parameters (string const& pass)
{
    system("cls");
    cout << menu["ciphers_list_title"] << endl << menu["ciphers_list"] << endl;
         
    int ciph, action, inputMode, outputMode;

    ciph = get_operations_code(0, 3, menu["ciphers_list"]);
    if (ciph == 0)
        return {0, 0, 0, 0};

    system("cls"); 
    cout << "Please enter a cipher password" << endl;
    check_password (pass);

    system("cls");
    cout << menu["action_title"] << endl << menu["action"] << endl;

    action = get_operations_code(0, 2, menu["action"]);
    if (action == 0)
        return {0, 0, 0, 0};

    system("cls");
    cout << menu["in_method_title"] << endl << menu["in_method"] << endl;

    inputMode = get_operations_code(0, 2, menu["in_method"]);
    if (inputMode == 0)
        return {0, 0, 0, 0};

    system("cls");
    cout << menu["out_method_title"] << endl << menu["out_method"] << endl;

    outputMode = get_operations_code(0, 2, menu["out_method"]);
    if (outputMode == 0)
        return {0, 0, 0, 0};
    
    return {ciph, action, inputMode, outputMode};
}


void enter_text (tuple <int, int, int, int> const& params, string const& pass)
{
    auto [ciph, action, inputMode, outputMode] = params;
    string text, fileInName, fileOutName, nextText;

    system("cls");
    cin.get();

    if (inputMode == 1)
    {
        cout << "Enter a text message:" << endl;
        getline(cin, text);
    }
    else
    {
        cout << "Enter the name of the file with the text message:" << endl;
        cin >> fileInName;
        text = file_to_string(fileInName);
    }

    if (ciph == 1)
    {
        auto* cipher = (action == 1) ? aes_encrypt : aes_decrypt;
        nextText = cipher(text, pass);
    }
    else if (ciph == 2)
    {
        nextText = snow3g_code(text, pass, 1234);
    }
    else
    {
        auto* cipher = (action == 1) ? A1Z26_encrypt : A1Z26_decrypt;
        nextText = cipher(text);
    }

    string t = (action == 1) ? "encrypted message: " 
                                 : "decrypted message: ";
    if (outputMode == 1)
    {
        cout << endl << t << endl << nextText <<  endl;
    }
    else
    {
        cout << endl << "Enter the name of the file to be written to " << t << endl;
        cin >> fileOutName;
        output(fileOutName, nextText);
    }

    expectation();
}