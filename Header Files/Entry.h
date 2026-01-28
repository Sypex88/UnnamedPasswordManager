//
// Created by trbec on 28.01.2026.
//

#ifndef UNTITLED4_ENTRY_H
#define UNTITLED4_ENTRY_H

#endif //UNTITLED4_ENTRY_H
#include <string>
using namespace std;

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