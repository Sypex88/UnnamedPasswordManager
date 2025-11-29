#include <json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <thread>
#include <stdexcept>
#include <sstream>
//TODO if master pw isnt set create variable in json
//TODO bug: password temporary dublicating(not json) on show passwords duplicates
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

    string answer;
    int answer1;
    string URL, username, password;
    bool skip = false;
    int entrysize = 0;
    bool RunLoop = true;

    json jFile;
    string storedMasterPassword;
    vector<Entry> entries;

    ifstream infile("../../data.json");
    if (infile.good()) {
        infile >> jFile;
        if (jFile.contains("MasterPassword"))
            storedMasterPassword = jFile["MasterPassword"].get<string>();
    }
    infile.close();

    if (storedMasterPassword.empty()) {
        cout << "Create Master Password:\n> ";
        cin >> storedMasterPassword;

        jFile["MasterPassword"] = storedMasterPassword;
        jFile["Entries"] = json::array();

        ofstream out("../../data.json");
        out << jFile.dump(4);
        out.close();
    }


    while (true) {
        cout << "Enter Master Password:\n> ";
        cin >> answer;

        if (answer == storedMasterPassword)
            break;

        cout << "Incorrect Master Password!\n\n";
    }

    cout << "Access granted!\n\n";
    this_thread::sleep_for(chrono::milliseconds(800));
    do {
        if (jFile.contains("Entries")) {

            for (auto& item : jFile["Entries"]) {
                entries.push_back(item.get<Entry>());
            }
        }



        prntmenu("1.Add Password", "2.Show Passwords", "3.Close", "", "Choose an option:");
        cin >> answer1;

        switch (answer1) {

            case 1: {
                // Passwort eingeben
                prntmenu("Website URL:");
                cin >> URL;
                prntmenu("Username:");
                cin >> username;
                prntmenu("Password:");
                cin >> password;

                while (RunLoop) {
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

                        ofstream out("../../data.json");
                        out << jFile.dump(4);
                        out.close();

                        prntmenu("Password Saved!\n", "1.Return to menu", "2.Quit", "", "Please Choose an Option");

                        cin >> answer;
                        switch (answer1) {
                            case 1: {
                                RunLoop = false;
                                continue;
                            }
                            case 2: {
                                return 0;
                            }
                        }


                    }
                    else if (answer == "no") {
                        RunLoop = false;
                        continue;
                    }
                    else {
                        cout << "Invalid input.\n\n";
                    }
                    break;
                    break;
                }
                break;
            }

            case 2: {
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

                    prntmenu("What would you like to do??\n", "1.Return to menu", "2.Edit Passwords", "3.Delete Entry",   "4.Quit\n\nPlease Choose an Option \n");

                    cin >> answer;

                    if (answer == "1") {
                        skip = true;
                    }
                    else if (answer == "2") {
                        int i = 1;
                        for (auto& e : entries) {
                            cout << i <<". URL:\n" << e.jsonURL << "\n";
                            i++;
                        }
                        prntmenu("Select an Entry:\n\n");
                        // TODO i= options, dynamicly choose entry

                    }
                    else if (answer == "3") {
                        // TODO Show entries and choose entry copy from above

                    }
                    else if (answer == "4") {
                        return 0;
                    }
                }
                break;


            }

            case 3:
                cout << "Closing...\n";
                return 0;
        }
    } while (true);




}