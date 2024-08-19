#ifndef PROGRAMMEEROPDRACHT_3_NFA_H
#define PROGRAMMEEROPDRACHT_3_NFA_H

#include <string>
#include <vector>
#include <unordered_set>
#include <map>
#include "json.hpp"


class DFA;

using json = nlohmann::json;

class NFA
{
public:

    NFA(std::string FileName);

    DFA toDFA();

    std::string setToString(const std::unordered_set<std::string>& stateSet);

    bool accepts(std::string input);

private:
    std::vector<std::string> states;
    std::vector<std::string> alphabet;
    std::map<std::string, std::map<std::string, std::unordered_set<std::string>>> transitions;
    std::unordered_set<std::string> accepting_states;
    std::string starting_state;
};

#endif //PROGRAMMEEROPDRACHT_3_NFA_H