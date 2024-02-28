#ifndef TA_PROGRAMMEROPDRACHT1_DFA_H
#define TA_PROGRAMMEROPDRACHT1_DFA_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
using namespace std;

class DFA {
public:
    DFA() {
// Initialiseer de toestanden
        states.push_back(0);
        states.push_back(1);
        states.push_back(2);

// Initialiseer de overgangen
        transitions[0]['0'] = 1;
        transitions[0]['1'] = 2;
        transitions[1]['0'] = 2;
        transitions[1]['1'] = 0;
        transitions[2]['0'] = 1;
        transitions[2]['1'] = 2;

// Initialiseer de acceptatiestaten
        accepting_states.insert(2);
    }

    bool accepts(string input) {
        int current_state = 0;
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
    vector<long> states;
    unordered_map<long, unordered_map<char, long>> transitions;
    unordered_set<long> accepting_states;
};



#endif //TA_PROGRAMMEROPDRACHT1_DFA_H
