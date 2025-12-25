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
    bool isNewVault = false;
    int index = 1;
    string encryptedData;
    string answer;
    int answer1;
    string URL, username, password;
    bool skip = false;
    int entrysize = 0;
    bool RunLoop = true;
    json jFile;
    string storedMasterPassword;
    vector<Entry> entries;


    if(sodium_init() < 0) {
        cout << "Sodium not initialized!\n\n";
        return 1;
    }


    ifstream infile("../../data.json", ios::binary);
    if (infile.good()) {
        encryptedData.assign(
            (istreambuf_iterator<char>(infile)),
            istreambuf_iterator<char>()
        );
    } else {
        isNewVault = true;
    }
    infile.close();



    if (isNewVault) {
        cout << "Create Master Password:\n> ";
        cin >> storedMasterPassword;

        jFile["Entries"] = json::array();

        string plainJson = jFile.dump(4);
        string encryptedOut =
            VaultCrypto::encryptJSON(plainJson, storedMasterPassword);

        ofstream out("../../data.json", ios::binary);
        out.write(encryptedOut.data(), encryptedOut.size());
        out.close();

        cout << "Vault created!\n\n";
    }





    if (!isNewVault) {
        while (true) {
            cout << "Enter Master Password:\n> ";
            cin >> storedMasterPassword;

            try {
                string decryptedJson =
                    VaultCrypto::decryptJSON(encryptedData, storedMasterPassword);
                jFile = json::parse(decryptedJson);
                break;
            } catch (...) {
                cout << "Incorrect Master Password!\n\n";
            }
        }
    }


    cout << "Access granted!\n\n";
    this_thread::sleep_for(chrono::milliseconds(800));

    do {
        if (jFile.contains("Entries")) {
            entries.clear();

            for (auto& item : jFile["Entries"]) {
                entries.push_back(item.get<Entry>());
            }
        }
        while (RunLoop) {
            RunLoop = false;
            prntmenu("1.Add Password", "2.Show Passwords", "3.Close", "", "Choose an option:");
            if (!(cin >> answer1)) {
                cout << "Invalid Input!\n\n";
                cin.clear(); // Fehlerzustand löschen
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Müll entfernen
                RunLoop = true;
                continue;
            }

            if (answer1 == 0) {
                cout << "Invalid Input!\n\n";
                RunLoop = true;
                continue;
            }
            else
            switch (answer1) {

                case 1: {
                    RunLoop = true; //festlegen das der while loop ausgeführt wird, falls weg = infinite loop
                    prntmenu("Website URL:");
                    cin >> URL;
                    prntmenu("Username:");
                    cin >> username;
                    prntmenu("Password:");
                    cin >> password;

                    while (RunLoop) {
                        RunLoop = false;
                        prntmenu("Are these Informations correct?:",
                                 "URL: " + URL,
                                 "Username: " + username,
                                 "Password: " + password,
                                 "");

                        cout << "Please type Yes or No:\n> ";
                        cin >> answer;
                        transform(answer.begin(), answer.end(), answer.begin(), ::tolower);

                        if (answer == "yes") {
                            Entry newEntry{URL, username, password};

                            jFile["Entries"].push_back(newEntry);


                            prntmenu("Password Saved!\n", "1.Return to menu", "2.Quit", "", "Please Choose an Option");

                            cin >> answer;
                            switch (answer1) {
                                case 1: {
                                    continue;//TODO Freeze
                                }
                                case 2: {
                                    return 0; //TODO doesnt end programm just freezes
                                }
                            }


                        }
                        else if (answer == "no") {
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

                        cin >> answer;

                        if (answer == "1") {

                        }
                        else if (answer == "2") {
                            RunLoop = true;
                            while (RunLoop) {
                                RunLoop = false;


                                index = 1;
                                for (auto& e : entries) {
                                    cout << index <<"."+e.jsonURL << "\n";
                                    index++;
                                }
                                cout << "\nSelect an Entry:\n\n";
                                cin >> answer1;
                                if (answer1 >= 1 && answer1 <= entries.size()) {
                                    Entry& e = entries[answer1 -1];
                                    RunLoop = true;
                                    while (RunLoop) {
                                        RunLoop = false;
                                        cout << "\nYou selected:\n";
                                        cout << "1.URL: " << e.jsonURL << "\n";
                                        cout << "2.Username: " << e.jsonUsername << "\n";
                                        cout << "3.Password: " << e.jsonPassword << "\n";
                                        cout << "Which information would you like to edit?\n\n";
                                        cin >> answer1;
                                        if (answer1 < 1 || answer1 > 3) {
                                            cout << "Invalid input!\n\n";
                                            RunLoop = true;
                                            continue;
                                        }
                                        else if (answer1 >= 1 && answer1 <= 3) {
                                            switch (answer1) {
                                                case 1: {
                                                    cout << "Please enter the new URL:\n\n";
                                                    cin >> e.jsonURL;
                                                    cout << "URL Saved!\n\n";
                                                    jFile["Entries"].clear();
                                                    for (auto& entry : entries) {
                                                        jFile["Entries"].push_back(entry);
                                                    }

                                                    RunLoop = true;
                                                    break;//TODO doesnt jump to main menu
                                                }
                                                case 2: {
                                                    cout << "Please enter the new Username:\n\n";
                                                    cin >> e.jsonUsername;
                                                    cout << "Username Saved!\n\n";
                                                    jFile["Entries"].clear();
                                                    for (auto& entry : entries) {
                                                        jFile["Entries"].push_back(entry);
                                                    }

                                                    continue;
                                                }
                                                case 3: {
                                                    cout << "Please enter the new Password:\n\n";
                                                    cin >> e.jsonPassword;
                                                    jFile["Entries"].clear();
                                                    for (auto& entry : entries) {
                                                        jFile["Entries"].push_back(entry);
                                                    }

                                                    cout << "Password Saved!\n\n";
                                                    continue;
                                                }

                                            }

                                        }

                                    }

                                }
                                else if (answer1 <= 1 || answer1 >= entrysize) {
                                    cout << "Invalid input.\n\n";
                                    RunLoop = true;
                                    continue;
                                }
                            }
                        }

                        else if (answer == "3") {
                            RunLoop = true;
                            while (RunLoop) {
                                RunLoop = false;
                               index = 1;
                                for (auto& e : entries) {
                                    cout << index <<"."+e.jsonURL << "\n";
                                    index++;
                                }
                                cout << "Select an Entry:\n\n";
                                cin >> answer1;
                                entrysize = entries.size();
                                if (answer1 >= 1 && answer1 <= entries.size()) {
                                    Entry& e = entries[answer1 -1];
                                    RunLoop = true;
                                    while (RunLoop) {
                                        RunLoop = false;
                                        cout << "\nYou selected:\n";
                                        cout << "URL: " << e.jsonURL << "\n";
                                        cout << "Username: " << e.jsonUsername << "\n";
                                        cout << "Password: " << e.jsonPassword << "\n";
                                        cout << "Are you sure you want to delete the entry? 1.Yes 2.No\n\n";
                                        cin >> answer;
                                        if (answer == "yes" || answer == "1") {
                                            RunLoop = false;
                                            //delete entry
                                            entries.erase(entries.begin() + (answer1 - 1));
                                            jFile["Entries"].clear();
                                            for (auto& entry : entries) {
                                                jFile["Entries"].push_back(entry);
                                            }

                                            cout << "Entry successfully deleted!\n\n";
                                            break;
                                        }
                                        else if (answer == "no" || answer == "2") {
                                            RunLoop = false;
                                            break;
                                        }
                                        else {
                                            cout << "Invalid input.\n\n";
                                            RunLoop = true;
                                        }
                                    }
                                }
                                else if (answer1 < 1 || answer1 > entrysize) {
                                    cout << "Invalid input.\n\n";
                                    RunLoop = true;
                                    continue;
                                }
                            }

                        }
                        else if (answer == "4") {
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

        string plainJson = jFile.dump(4);

        string encryptedOut =
            VaultCrypto::encryptJSON(plainJson, storedMasterPassword);

        ofstream out("../../data.json", ios::binary);
        out.write(encryptedOut.data(), encryptedOut.size());
        out.close();

        sodium_memzero(storedMasterPassword.data(), storedMasterPassword.size());


    } while (true);


}