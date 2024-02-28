#ifndef TA_PROGRAMMEROPDRACHT1_DFA_H
#define TA_PROGRAMMEROPDRACHT1_DFA_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <iomanip>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

class DFA {
public:
    DFA() {
// Initialiseer de toestanden
        states.push_back("q0");
        states.push_back("q1");
        states.push_back("q2");

// Initialiseer de overgangen
        transitions[0]['0'] = 1;
        transitions[0]['1'] = 2;
        transitions[1]['0'] = 2;
        transitions[1]['1'] = 0;
        transitions[2]['0'] = 1;
        transitions[2]['1'] = 2;

// Initialiseer de acceptatiestaten
        accepting_states.insert("q2");
    }

    DFA(string FileName){
        // Open the file
        ifstream input(FileName);
        if(!input.is_open()){
            // Error handling
            throw runtime_error("File name not found");
            return;
        }

        // Parse the JSON data
        json j;
        input >> j;
        if (!j.is_discarded()){
            // Error handling
            throw runtime_error("Failed to parse JSON data from file");
            return;
        }

        if(j["type"] = "DFA"){
            alphabet = j["alphabet"].get<vector<char>>();
            for(auto& states:j["states"]){
                if(j["starting"] = true){
                    string starting_state = j["name"];
                }
                else if(j["accepting"] = true) {
                    string accepting_state = j["name"];
                }
            }
            //cout << starting_states << accepting_states << endl;
        }

        // Close the file
        input.close();

    }

    bool accepts(string input) {
        string current_state;
        for (char c : input) {
// Controleer of de overgang bestaat
            if (transitions[current_state].find(c) == transitions[current_state].end()) {
                return false;
            }

            // Ga naar de volgende toestand
            current_state = transitions[current_state][c];
        }

// Controleer of de huidige toestand een acceptatiestaat is
        return accepting_states.find(current_state) != accepting_states.end();
    }

private:

    vector<string> states;
    vector<char> alphabet;
    unordered_map<long, unordered_map<char, long>> transitions;
    unordered_set<string> accepting_states;
    unordered_set<string> starting_states;
};



#endif //TA_PROGRAMMEROPDRACHT1_DFA_H
