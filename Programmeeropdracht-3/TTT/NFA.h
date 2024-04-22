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

using json = nlohmann::json;

class NFA
{
public:

    NFA(std::string FileName) {
        std::ifstream input(FileName);
        if (!input.is_open()) {
            // Error handling
            throw std::runtime_error("File name not found");
        }

        // Parse the JSON data
        json j;
        if (!(input >> j)) {
            throw std::runtime_error("Failed to parse JSON data from file");
        }

        if(j["type"] == "NFA"){
            alphabet = j["alphabet"].get<std::vector<std::string>>();
            for(const auto &state:j["states"]){
                states.push_back(state["name"]);
                std::string state_name = state["name"];
                if(state["starting"] == true){
                    starting_state.emplace(state_name);
                }
                else if(state["accepting"] == true) {
                    accepting_states.emplace(state_name);
                }
            }
            for(auto& transition:j["transitions"]){
                std::string transition_from = transition["from"];
                std::string transition_to = transition["to"];
                std::string transition_input = transition["input"];
                transitions[transition_from][transition_input] = transition_to;
            }
        }
        input.close();
    }

    DFA toDFA()
    {
        ;
    }

//    bool accepts(std::string input) {
//        std::string current_state = *starting_state.begin();
//        for (char c : input) {
//// Controleer of de overgang bestaat
//            if (transitions[current_state].count(std::string(1,c)) == 0) {
//                return false;
//            }
//
//            // Ga naar de volgende toestand
//            current_state = transitions[current_state][std::string(1,c)];
//        }
//
//// Controleer of de huidige toestand een acceptatiestaat is
//        return accepting_states.find(current_state) != accepting_states.end();
//    }

//    bool accepts(std::string input) {
//        // 1. Find all reachable states from starting states
//        std::unordered_set<std::string> reachable_states;
//        findReachableStates(starting_state, reachable_states);
//
//        // 2. Initialize current state set (all reachable states)
//        std::unordered_set<std::string> current_states = reachable_states;
//
//        // 3. Process input string symbol by symbol
//        for (char c : input) {
//            // 4. Find next state set for the current input symbol
//            std::unordered_set<std::string> next_states;
//            for (const std::string& state : current_states) {
//                for (const auto& [symbol, to_state] : transitions[state]) {
//                    if (symbol == std::string(1, c)) {
//                        next_states.insert(to_state);
//                    }
//                }
//            }
//
//            // 5. Check for empty next state set (no transitions for the symbol)
//            if (next_states.empty()) {
//                return false; // No possible transitions, language not accepted
//            }
//
//            // 5. Update current state set for the next symbol
//            current_states = next_states;
//
//        }
//
//        // 6. Check if any state in the final state set is accepting
//        return std::any_of(current_states.begin(), current_states.end(), [&](const std::string& state) {
//            return accepting_states.find(state) != accepting_states.end();
//        });
//    }
//
//// Helper function to find all reachable states from a set of states (used internally)
//    void findReachableStates(const std::unordered_set<std::string>& states, std::unordered_set<std::string>& reachable_states) {
//        // Use a queue to explore states level by level (BFS)
//        std::queue<std::string> to_explore;
//        for (const std::string& state : states) {
//            to_explore.push(state);
//        }
//
//        while (!to_explore.empty()) {
//            std::string current_state = to_explore.front();
//            to_explore.pop();
//
//            if (reachable_states.find(current_state) != reachable_states.end()) {
//                // Already explored this state
//                continue;
//            }
//
//            reachable_states.insert(current_state);
//
//            // Explore transitions from the current state
//            for (const auto& [symbol, to_state] : transitions[current_state]) {
//                to_explore.push(to_state);
//            }
//        }
//    }



bool accepts(std::string input)
{
    std::string current_state = *starting_state.begin();
    for (int i = 0; i < input.length(); i++)
    {

    }
}



protected:
        std::vector<std::string> states;
        std::vector<std::string> alphabet;
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> transitions;
        std::unordered_set<std::string> accepting_states;
        std::unordered_set<std::string> starting_state;
};


#endif //PROGRAMMEEROPDRACHT_3_NFA_H
