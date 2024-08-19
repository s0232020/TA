#include "DFA.h"
#include <fstream>
#include <iostream>
#include <queue>

DFA::DFA(std::string FileName)
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
DFA::DFA(DFA dfa1, DFA dfa2, bool intersection)
{
    // Initialize resulting dfa
    if (dfa1.alphabet != dfa2.alphabet)
    {
        throw std::runtime_error("Alphabets of DFA's do not match");
    }

    alphabet = dfa1.alphabet;

    // Create a queue and a map to store the processed states
    std::queue<std::string> statesQueue;
    std::map<std::string, bool> processedStates;

    // Start with the starting state
    std::string startingState = "(" + dfa1.starting_state + "," + dfa2.starting_state + ")";
    states.emplace_back(startingState);
    starting_state = startingState;
    statesQueue.push(startingState);
    processedStates[startingState] = true;

    // Process each state
    while (!statesQueue.empty())
    {
        std::string state = statesQueue.front();
        statesQueue.pop();

        // If both states are accepting states (for intersection), make the new state an accepting state
        std::string state1 = state.substr(1, state.find(",") - 1);
        std::string state2 = state.substr(state.find(",") + 1, state.length() - state.find(",") - 2);
        if (intersection && dfa1.accepting_states.count(state1) > 0 && dfa2.accepting_states.count(state2) > 0) {
            accepting_states.insert(state);
        }

        if(!intersection && (dfa1.accepting_states.count(state1) || dfa2.accepting_states.count(state2)) )
        {
            accepting_states.insert(state);
        }

        // For each symbol in the alphabet
        for (const auto& symbol : alphabet) {
            // Check if there is a valid transition for each state on the symbol
            if (dfa1.transitions[state1].count(symbol) > 0 && dfa2.transitions[state2].count(symbol) > 0) {
                // Add a transition from the new state to the state corresponding to the pair of states that the DFAs would transition to on that symbol
                std::string transitionState1 = dfa1.transitions[state1][symbol];
                std::string transitionState2 = dfa2.transitions[state2][symbol];
                std::string newState = "(" + transitionState1 + "," + transitionState2 + ")";

                // If the new state is not in the map of processed states, add it to the queue and the map
                if (processedStates.count(newState) == 0) {
                    states.emplace_back(newState);
                    statesQueue.push(newState);
                    processedStates[newState] = true;
                }

                transitions[state][symbol] = newState;
            }
        }
    }
}

bool DFA::accepts(std::string input)
{
    std::string current_state = starting_state;
    for (char c : input) {
        // Check if the transition exists
        if (transitions[current_state].count(std::string(1,c)) == 0) {
            // If the transition does not exist, check if there is a '.' transition
            if (transitions[current_state].count(".") == 0) {
                return false;
            }
                // If there is a '.' transition, use it
            else {
                current_state = transitions[current_state]["."];
            }
        }
        else {
            // Go to the next state
            current_state = transitions[current_state][std::string(1,c)];
        }
    }

    // Check if the current state is an accepting state
    return accepting_states.find(current_state) != accepting_states.end();
}

void DFA::print()
{
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
