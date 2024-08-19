#ifndef TA_PROGRAMMEROPDRACHT1_DFA_H
#define TA_PROGRAMMEROPDRACHT1_DFA_H

#include <vector>
#include <unordered_set>
#include "json.hpp"
#include <string>

using json = nlohmann::json;

class DFA {
public:

    DFA() = default;

    DFA(std::string FileName);

    DFA(DFA dfa1, DFA dfa2, bool intersection);

    bool accepts(std::string input);

    void print();

    friend class NFA;

public:
    std::vector<std::string> states;
    std::vector<std::string> alphabet;
    std::map<std::string, std::map<std::string, std::string>> transitions;
    std::unordered_set<std::string> accepting_states;
    std::string starting_state;
};



#endif //TA_PROGRAMMEROPDRACHT1_DFA_H
