#include <json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <thread>
#include <stdexcept>
#include <sstream>
#include <sodium.h>
#include "VaultCrypto.h"
using namespace std;
using json = nlohmann::json;

void prntmenu(string option1, string option2 = "", string option3 = "", string option4 = "", string text1 = "")
{
    if (option1 != "") option1 += "\n";
    if (option2 != "") option2 += "\n";
    if (option3 != "") option3 += "\n";
    if (option4 != "") option4 += "\n";

    cout << option1 << option2 << option3 << option4 << text1 << "\n";
}

struct Entry {
    string jsonURL;
    string jsonUsername;
    string jsonPassword;
};

void to_json(json& j, const Entry& e) {
    j = json{
        {"URL", e.jsonURL},
        {"Username", e.jsonUsername},
        {"Password", e.jsonPassword}
    };
}

void from_json(const json& j, Entry& e) {
    j.at("URL").get_to(e.jsonURL);
    j.at("Username").get_to(e.jsonUsername);
    j.at("Password").get_to(e.jsonPassword);
}


int main() {
    bool CloseProgram = false;
    bool InvalidInput = false;
    bool IsNewVault = false;
    int Index = 1;
    string EncryptedData;
    string Answer;
    int intAnswer;
    string URL, Username, Password;
    bool LoopSkip = false;
    bool RunLoop = true;
    json JFile;
    string storedMasterPassword;
    vector<Entry> entries;


    if(sodium_init() < 0) {
        cout << "Sodium not initialized!\n\n";
        return 1;
    }


    ifstream infile("../../data.json", ios::binary);
    if (infile.good()) {
        EncryptedData.assign(
            (istreambuf_iterator<char>(infile)),
            istreambuf_iterator<char>()
        );
    } else {
        IsNewVault = true;
    }
    infile.close();



    if (IsNewVault) {
        cout << "Create Master Password:\n> ";
        cin >> storedMasterPassword;

        JFile["Entries"] = json::array();

        string plainJson = JFile.dump(4);
        string encryptedOut =
            VaultCrypto::encryptJSON(plainJson, storedMasterPassword);

        ofstream out("../../data.json", ios::binary);
        out.write(encryptedOut.data(), encryptedOut.size());
        out.close();

        cout << "Vault created!\n\n";
    }





    if (!IsNewVault) {
        while (true) {
            cout << "Enter Master Password:\n> ";
            cin >> storedMasterPassword; //TODO after restart master password always incorrect

            try {
                string decryptedJson =
                    VaultCrypto::decryptJSON(EncryptedData, storedMasterPassword);
                JFile = json::parse(decryptedJson);
                break;
            } catch (...) {
                cout << "Incorrect Master Password!\n\n";
            }
        }
    }


    cout << "Access granted!\n\n";
    this_thread::sleep_for(chrono::milliseconds(800));

    do {
        if (JFile.contains("Entries")) {
            entries.clear();

            for (auto& item : JFile["Entries"]) {
                entries.push_back(item.get<Entry>());
            }
        }
        while (RunLoop) {
            RunLoop = false;
            prntmenu("1.Add Password", "2.Show Passwords", "3.Close", "", "Choose an option:");
            if (!(cin >> intAnswer)) {
                cout << "Invalid Input!\n\n";
                cin.clear(); // Fehlerzustand löschen
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Müll entfernen
                RunLoop = true;
                continue;
            }

            if (intAnswer == 0) {
                cout << "Invalid Input!\n\n";
                RunLoop = true;
                continue;
            }
            else
            switch (intAnswer) {

                case 1: {
                    RunLoop = true; //festlegen das der while loop ausgeführt wird, falls weg = infinite loop
                    prntmenu("Website URL:");
                    cin >> URL;
                    prntmenu("Username:");
                    cin >> Username;
                    prntmenu("Password:");
                    cin >> Password;

                    while (RunLoop) {
                        RunLoop = false;
                        prntmenu("Are these Informations correct?:",
                                 "URL: " + URL,
                                 "Username: " + Username,
                                 "Password: " + Password,
                                 "");

                        cout << "Please type Yes or No:\n> ";
                        cin >> Answer;
                        transform(Answer.begin(), Answer.end(), Answer.begin(), ::tolower);

                        if (Answer == "yes") {
                            Entry newEntry{URL, Username, Password};

                            JFile["Entries"].push_back(newEntry);


                            prntmenu("Password Saved!\n", "1.Return to menu", "2.Quit", "", "Please Choose an Option");

                            cin >> Answer;
                            switch (intAnswer) {
                                case 1: {
                                    continue;//TODO Freeze
                                }
                                case 2: {
                                    return 0; //TODO doesnt end programm just freezes
                                }
                            }


                        }
                        else if (Answer == "no") {
                            continue;
                        }
                        else {
                            cout << "Invalid input.\n\n";
                            RunLoop = true;
                            continue;
                        }
                        break;

                    }
                    break;
                }

                case 2: {
                    RunLoop = true;
                    while (RunLoop) {
                        RunLoop = false;
                        cout << "\n >>>>> STORED PASSWORDS <<<<< \n";

                    if (entries.empty()) {
                        cout << "No entries found!\n\n";
                    }
                    else {
                        for (auto& e : entries) {
                            cout << "--------------------\n";
                            cout << "URL:\n"+e.jsonURL << "\n\n";
                            cout << "Username:\n"+e.jsonUsername << "\n\n";
                            cout << "Password:\n"+e.jsonPassword << "\n\n";
                        }

                        if (InvalidInput == true) {
                            cout << "Invalid input!\n\n";
                        }
                        prntmenu("Please choose an option\n", "1.Return to menu", "2.Edit Entry", "3.Delete Entry",   "4.Quit\nChoose an option:");

                        cin >> Answer;

                        if (Answer == "1") {

                        }
                        else if (Answer == "2") {
                            RunLoop = true;
                            while (RunLoop) {
                                RunLoop = false;


                                Index = 1;
                                for (auto& e : entries) {
                                    cout << Index <<"."+e.jsonURL << "\n";
                                    Index++;
                                }
                                cout << "\nSelect an Entry:\n\n";
                                cin >> intAnswer;
                                if (intAnswer >= 1 && intAnswer <= entries.size()) {
                                    Entry& e = entries[intAnswer -1];
                                    RunLoop = true;
                                    while (RunLoop) {
                                        RunLoop = false;
                                        cout << "\nYou selected:\n";
                                        cout << "1.URL: " << e.jsonURL << "\n";
                                        cout << "2.Username: " << e.jsonUsername << "\n";
                                        cout << "3.Password: " << e.jsonPassword << "\n";
                                        cout << "Which information would you like to edit?\n\n";
                                        cin >> intAnswer;
                                        if (intAnswer < 1 || intAnswer > 3) {
                                            cout << "Invalid input!\n\n";
                                            RunLoop = true;
                                            continue;
                                        }
                                        else if (intAnswer >= 1 && intAnswer <= 3) {
                                            switch (intAnswer) {
                                                case 1: {
                                                    cout << "Please enter the new URL:\n\n";
                                                    cin >> e.jsonURL;
                                                    cout << "URL Saved!\n\n";
                                                    JFile["Entries"].clear();
                                                    for (auto& entry : entries) {
                                                        JFile["Entries"].push_back(entry);
                                                    }

                                                    RunLoop = true;
                                                    break;//TODO doesnt jump to main menu
                                                }
                                                case 2: {
                                                    cout << "Please enter the new Username:\n\n";
                                                    cin >> e.jsonUsername;
                                                    cout << "Username Saved!\n\n";
                                                    JFile["Entries"].clear();
                                                    for (auto& entry : entries) {
                                                        JFile["Entries"].push_back(entry);
                                                    }

                                                    continue;
                                                }
                                                case 3: {
                                                    cout << "Please enter the new Password:\n\n";
                                                    cin >> e.jsonPassword;
                                                    JFile["Entries"].clear();
                                                    for (auto& entry : entries) {
                                                        JFile["Entries"].push_back(entry);
                                                    }

                                                    cout << "Password Saved!\n\n";
                                                    continue;
                                                }

                                            }

                                        }

                                    }

                                }
                                else if (intAnswer <= 1 || intAnswer >= entries.size()) {
                                    cout << "Invalid input.\n\n";
                                    RunLoop = true;
                                    continue;
                                }
                            }
                        }

                        else if (Answer == "3") {
                            RunLoop = true;
                            while (RunLoop) {
                                RunLoop = false;
                               Index = 1;
                                for (auto& e : entries) {
                                    cout << Index <<"."+e.jsonURL << "\n";
                                    Index++;
                                }
                                cout << "Select an Entry:\n\n";
                                cin >> intAnswer;
                                if (intAnswer >= 1 && intAnswer <= entries.size()) {
                                    Entry& e = entries[intAnswer -1];
                                    RunLoop = true;
                                    while (RunLoop) {
                                        RunLoop = false;
                                        cout << "\nYou selected:\n";
                                        cout << "URL: " << e.jsonURL << "\n";
                                        cout << "Username: " << e.jsonUsername << "\n";
                                        cout << "Password: " << e.jsonPassword << "\n";
                                        cout << "Are you sure you want to delete the entry? 1.Yes 2.No\n\n";
                                        cin >> Answer;
                                        if (Answer == "yes" || Answer == "1") {
                                            RunLoop = false;
                                            //delete entry
                                            entries.erase(entries.begin() + (intAnswer - 1));
                                            JFile["Entries"].clear();
                                            for (auto& entry : entries) {
                                                JFile["Entries"].push_back(entry);
                                            }

                                            cout << "Entry successfully deleted!\n\n";
                                            break;
                                        }
                                        else if (Answer == "no" || Answer == "2") {
                                            RunLoop = false;
                                            break;
                                        }
                                        else {
                                            cout << "Invalid input.\n\n";
                                            RunLoop = true;
                                        }
                                    }
                                }
                                else if (intAnswer < 1 || intAnswer > entries.size()) {
                                    cout << "Invalid input.\n\n";
                                    RunLoop = true;
                                    continue;
                                }
                            }

                        }
                        else if (Answer == "4") {
                            return 0;
                        }
                        else {
                            RunLoop = true;
                            InvalidInput = true;
                            continue;
                        }

                    }
                    break;
                    }
                }
            case 3:
                cout << "Closing...\n";
                return 0;
            }
        }

        string plainJson = JFile.dump(4);

        string encryptedOut =
            VaultCrypto::encryptJSON(plainJson, storedMasterPassword);

        ofstream out("../../data.json", ios::binary);
        out.write(encryptedOut.data(), encryptedOut.size());
        out.close();

        sodium_memzero(storedMasterPassword.data(), storedMasterPassword.size());


    } while (true);


}