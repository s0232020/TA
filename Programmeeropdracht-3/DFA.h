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

    DFA() = default;

    DFA(std::string FileName)
    {
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
            for(const auto &state:j["states"]){
                states.emplace_back(state["name"]);
                std::string state_name = state["name"];
                if(state["starting"] == true)
                {
                    starting_state = state_name;
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
        std::string current_state = starting_state;
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

    void print() {
        json j;
        j["type"] = "DFA";
        j["alphabet"] = alphabet;

        std::vector<json> states_json;
        for (const auto &state : states) {
            json state_json;
            state_json["name"] = state;
            state_json["accepting"] = accepting_states.count(state) != 0;
            state_json["starting"] = starting_state.find(state) == 0;
            states_json.push_back(state_json);
        }

        std::vector<json> transitions_json;
        for (const auto &transition : transitions) {
            for (const auto &input_state : transition.second) {
                json transition_json;
                transition_json["from"] = transition.first;
                transition_json["input"] = input_state.first;
                transition_json["to"] = input_state.second;
                transitions_json.push_back(transition_json);
            }
        }

        std::sort(states_json.begin(), states_json.end(), [](const json &a, const json &b) {
            return std::tie(a["name"], a["starting"], a["accepting"]) <
                   std::tie(b["name"], b["starting"], b["accepting"]);
        });

        // Sort transitions based on "from", "input", and "to"
        std::sort(transitions_json.begin(), transitions_json.end(), [](const json &a, const json &b) {
            return std::tie(a["from"], a["input"], a["to"]) < std::tie(b["from"], b["input"], b["to"]);
        });

        j["states"] = states_json;
        j["transitions"] = transitions_json;

        std::cout << std::setw(4) << j << std::endl;
    }

friend class NFA;

private:

    std::vector<std::string> states;
    std::vector<std::string> alphabet;
    std::map<std::string, std::map<std::string, std::string>> transitions;
    std::unordered_set<std::string> accepting_states;
    std::string starting_state;
};



#endif //TA_PROGRAMMEROPDRACHT1_DFA_H
