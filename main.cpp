#include <json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <thread>
#include <stdexcept>
#include <sstream>
using namespace std;
using json = nlohmann::json;
//TODO bug when on option select typing a string
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
    int index = 1;
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
            entries.clear();

            for (auto& item : jFile["Entries"]) {
                entries.push_back(item.get<Entry>());
            }
        }

        prntmenu("1.Add Password", "2.Show Passwords", "3.Close", "", "Choose an option:");

        cin >> answer1;

        switch (answer1) {

            case 1: {
                RunLoop = true; //festlegen das der while loop ausgeführt wird, falls weg = infinite loop
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

                    prntmenu("Please choose an option\n", "1.Return to menu", "2.Edit Passwords", "3.Delete Entry",   "4.Quit\nChoose an option:");

                    cin >> answer;

                    if (answer == "1") {

                    }
                    else if (answer == "2") {
                        prntmenu("Select an Entry:\n\n");
                        // TODO i= options, dynamicly choose entry
                        index = 1;
                        for (auto& e : entries) {
                            cout << index <<"."+e.jsonURL << "\n";
                            index++;
                        }
                        cout << "Select an Entry:\n\n";
                        cin >> answer1;
                        if (answer1 >= 1 || answer1 <= entries.size()) {
                            Entry& e = entries[answer1 -1];

                            cout << "\nYou selected:\n";
                            cout << "1.URL: " << e.jsonURL << "\n";
                            cout << "2.Username: " << e.jsonUsername << "\n";
                            cout << "3.Password: " << e.jsonPassword << "\n";
                            cout << "Which information would you like to edit?\n\n";
                            cin >> answer1;
                            switch (answer1) {
                                case 1: {
                                    cout << "Please enter the new URL:\n\n";
                                    cin >> e.jsonURL;
                                    cout << "URL Saved!\n\n";
                                    jFile["Entries"].clear();
                                    for (auto& entry : entries) {
                                        jFile["Entries"].push_back(entry);
                                    }
                                    ofstream out("../../data.json");
                                    out << jFile.dump(4);
                                    out.close();
                                    continue;
                                }
                                case 2: {
                                    cout << "Please enter the new Username:\n\n";
                                    cin >> e.jsonUsername;
                                    cout << "Username Saved!\n\n";
                                    jFile["Entries"].clear();
                                    for (auto& entry : entries) {
                                        jFile["Entries"].push_back(entry);
                                    }
                                    ofstream out("../../data.json");
                                    out << jFile.dump(4);
                                    out.close();
                                    continue;
                                }
                                case 3: {
                                    cout << "Please enter the new Password:\n\n";
                                    cin >> e.jsonPassword;
                                    jFile["Entries"].clear();
                                    for (auto& entry : entries) {
                                        jFile["Entries"].push_back(entry);
                                    }
                                    ofstream out("../../data.json");
                                    out << jFile.dump(4);
                                    out.close();
                                    cout << "Password Saved!\n\n";


                                    continue;
                                }
                            }
                        }
                        else
                            cout << "Invalid input.\n\n";
                        }
                    else if (answer == "3") {
                        RunLoop = true;
                        while (RunLoop) {
                           index = 1;
                            for (auto& e : entries) {
                                cout << index <<"."+e.jsonURL << "\n";
                                index++;
                            }
                            cout << "Select an Entry:\n\n";
                            cin >> answer1;
                            if (answer1 >= 1 || answer1 <= entries.size()) {
                                Entry& e = entries[answer1 -1];
                                RunLoop = true;
                                while (RunLoop) {
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
                                        ofstream out("../../data.json");
                                        out << jFile.dump(4);
                                        out.close();
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
                            else if (answer1 <= 1 || answer1 >= entrysize) { //TODO doesnt execute = Fatal error
                                cout << "Invalid input.\n\n";
                                RunLoop = true;
                            }


                        }

                    }
                    else if (answer == "4") {
                        return 0;
                    }
                    else
                        cout << "Invalid input.\n\n";

                }
                break;


            }

            case 3:
                cout << "Closing...\n";
                return 0;
        }
    } while (true);


}