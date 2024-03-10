#ifndef TA_PROGRAMMEROPDRACHT1_DFA_H
#define TA_PROGRAMMEROPDRACHT1_DFA_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <iomanip>
#include "json.hpp"
#include <string>
using json = nlohmann::json;

class DFA {
public:

    DFA(std::string FileName){
        // Open the file
        std::ifstream input(FileName);
        if(!input.is_open()){
            // Error handling
            throw std::runtime_error("File name not found");
        }

        // Parse the JSON data
        json j;
        if (!(input >> j)){
            throw std::runtime_error("Failed to parse JSON data from file");
        }

        if(j["type"] == "DFA"){
            alphabet = j["alphabet"].get<std::vector<std::string>>();
            for(auto& state:j["states"]){
                std::string state_name = state["name"];
                if(state["starting"] == true){
                    starting_states.emplace(state_name);
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

    bool accepts(std::string input) {
        std::string current_state = *starting_states.begin();
        for (char c : input) {
// Controleer of de overgang bestaat
            if (transitions[current_state].count(std::string(1,c)) == 0) {
                return false;
            }

            // Ga naar de volgende toestand
            current_state = transitions[current_state][std::string(1,c)];
        }

// Controleer of de huidige toestand een acceptatiestaat is
        return accepting_states.find(current_state) != accepting_states.end();
    }

private:

    std::vector<std::string> states;
    std::vector<std::string> alphabet;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> transitions;
    std::unordered_set<std::string> accepting_states;
    std::unordered_set<std::string> starting_states;
};



#endif //TA_PROGRAMMEROPDRACHT1_DFA_H
