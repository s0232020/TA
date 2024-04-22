#ifndef PROGRAMMEEROPDRACHT_3_NFA_H
#define PROGRAMMEEROPDRACHT_3_NFA_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <iomanip>
#include "json.hpp"
#include <string>
#include "DFA.h"
#include <queue>
#include <algorithm>

using json = nlohmann::json;

class NFA
{
public:

    NFA(std::string FileName)
    {
        std::ifstream input(FileName);
        if (!input.is_open())
        {
            throw std::runtime_error("File name not found");
        }

        json j;
        if (!(input >> j))
        {
            throw std::runtime_error("Failed to parse JSON data from file");
        }

        if(j["type"] == "NFA"){
            alphabet = j["alphabet"].get<std::vector<std::string>>();
            for(const auto &state:j["states"])
            {
                states.insert(state["name"].get<std::string>());
                std::string state_name = state["name"];
                if(state["starting"] == true)
                {
                    starting_state.insert(state_name);
                }
                else if(state["accepting"] == true)
                {
                    accepting_states.insert(state_name);
                }
            }
            for(auto& transition:j["transitions"])
            {
                std::string transition_from = transition["from"];
                std::string transition_to = transition["to"];
                std::string transition_input = transition["input"];
                transitions[transition_from][transition_input].insert(transition_to);
            }
        }
        input.close();
    }

    DFA toDFA() {
        DFA dfa;
        dfa.alphabet = alphabet;
        dfa.starting_states = starting_state;
        for (auto &state : accepting_states)
        {
            dfa.accepting_states.insert(state);
        }
        for (auto& state : states)
        {
            dfa.states.insert(state);
        }

//        std::queue<std::unordered_set<std::string>> queue;
//        queue.push(starting_state);
//
//        while (!queue.empty())
//        {
//            std::unordered_set<std::string> current_state = queue.front();
//            queue.pop();
//
//            for (const std::string& symbol : alphabet)
//            {
//                std::unordered_set<std::string> next_state;
//                for (const std::string& state : current_state)
//                {
//                    if (transitions[state].find(symbol) != transitions[state].end())
//                    {
//                        next_state.insert(transitions[state][symbol].begin(), transitions[state][symbol].end());
//                    }
//                }
//
//                bool all_states_exist = true;
//                for (const auto& state : next_state) {
//                    if (dfa.states.find(state) == dfa.states.end()) {
//                        all_states_exist = false;
//                        break;
//                    }
//                }
//
//                if (all_states_exist)
//                {
//                    for (const auto& state : next_state) {
//                        dfa.states.insert(state);
//                    }
//                    queue.push(next_state);
//                }
//
//                for (const auto& state : current_state) {
//                    for (const auto& next : next_state) {
//                        dfa.transitions[state][symbol] = next;
//                    }
//                }
//            }
//        }
//
//        for (const auto& state : dfa.states)
//        {
//            bool is_accepting = false;
//            for (const std::string& accepting_state : accepting_states)
//            {
//                if (state.find(accepting_state) != std::string::npos)                {
//                    is_accepting = true;
//                    break;
//                }
//            }
//            if (is_accepting)
//            {
//                dfa.accepting_states.insert(state);
//            }
//        }

        return dfa;
    }

    bool accepts(std::string input)
    {
        std::unordered_set<std::string> current_states = starting_state;

        for (char c : input)
        {
            std::unordered_set<std::string> next_states;

            for (const std::string& state : current_states)
            {
                if (transitions[state].find(std::string(1, c)) != transitions[state].end())
                {
                    const std::unordered_set<std::string>& next_states_for_symbol = transitions[state][std::string(1, c)];
                    next_states.insert(next_states_for_symbol.begin(), next_states_for_symbol.end());
                }
            }

            current_states = next_states;

            if (current_states.empty())
            {
                return false;
            }
        }

        for (const std::string& state : current_states)
        {
            if (accepting_states.find(state) != accepting_states.end())
            {
                return true;
            }
        }

        return false;
    }

private:
    std::unordered_set<std::string> states;
    std::vector<std::string> alphabet;
    std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_set<std::string>>> transitions;
    std::unordered_set<std::string> accepting_states;
    std::unordered_set<std::string> starting_state;
};

#endif //PROGRAMMEEROPDRACHT_3_NFA_H