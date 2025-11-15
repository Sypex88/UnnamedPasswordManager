#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void menu(string option1, string option2 = "", string option3 = "", string option4 = "", string text1 = "")
{
    cout << option1 << "\n"
         << option2 << "\n"
         << option3 << "\n"
         << option4 << "\n"
         << text1 << "\n\n";
}

int main() {
    int anwser;
    string URL;
    string username;
    string password;
    menu("1.Add Password", "2.Show Passwords", "3.Close", "","Choose an option:");
    cin >> anwser;
    switch (anwser) {
        case 1:
            menu("Website URL:");
            cin >> URL;
            menu("Username:");
            cin >> username;
            menu("Password:");
            cin >> password;
            menu("Are these Informations correct?:", "URL:"+URL, "Username:"+username,"Password:"+password);
            //if correct >> Save information in json file, if not >> Change options/Cancel password entry//
            break;
        case 2:;
            //add other options//
    }
    //add en- decryption
}
