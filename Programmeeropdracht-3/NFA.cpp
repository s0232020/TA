#include "NFA.h"
#include <fstream>
#include "DFA.h"

NFA::NFA(std::string FileName)
{
    std::ifstream input(FileName);
    if (!input.is_open()) throw std::runtime_error("File name not found");

    json j;
    if (!(input >> j)) throw std::runtime_error("Failed to parse JSON data from file");

    if (j["type"] == "NFA")
    {
        alphabet = j["alphabet"].get<std::vector<std::string>>();
        for (const auto &state: j["states"])
        {
            states.emplace_back(state["name"]);
            std::string state_name = state["name"];
            if(state["starting"] == true)
            {
                starting_state = state_name;
            }
            else if (state["accepting"] == true)
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

DFA NFA::toDFA()
{
    // Create a new DFA object
    DFA dfa;

    // Copy the alphabet from the NFA to the DFA
    dfa.alphabet = alphabet;

    // Initialize a queue to hold the new states of the DFA
    std::queue<std::unordered_set<std::string>> newStatesQueue;

    // Add the starting state of the NFA to the queue
    newStatesQueue.push(std::unordered_set<std::string>{starting_state});

    // While there are still new states to process
    while (!newStatesQueue.empty()) {
        // Get the next new state
        std::unordered_set<std::string> newState = newStatesQueue.front();
        newStatesQueue.pop();

        // Convert the set of states to a string to use as the name of the new state in the DFA
        std::string newStateName = setToString(newState);

        // Convert the set of starting states to a string to use as the name of the starting state in the DFA
        std::string startingStateName = setToString(std::unordered_set<std::string>{starting_state});
        dfa.starting_state = startingStateName;

        // If the new state is not already a state in the DFA
        if (std::find(dfa.states.begin(), dfa.states.end(), newStateName) == dfa.states.end()) {
            // Add the new state to the DFA
            dfa.states.emplace_back(newStateName);
        }

        // If the new state contains an accepting state of the NFA, it is an accepting state of the DFA
        for (const std::string& state : newState) {
            if (accepting_states.find(state) != accepting_states.end()) {
                dfa.accepting_states.insert(newStateName);
                break;
            }
        }

        // For each symbol in the alphabet
        for (const std::string& symbol : alphabet) {
            // Initialize a set to hold the states reachable from the new state on the symbol
            std::unordered_set<std::string> reachableStates;

            // For each state in the new state
            for (const std::string& state : newState) {
                // If there is a transition on the symbol
                if (transitions[state].find(symbol) != transitions[state].end()) {
                    // Add the states reachable from the state on the symbol to the set of reachable states
                    const std::unordered_set<std::string>& nextStates = transitions[state][symbol];
                    reachableStates.insert(nextStates.begin(), nextStates.end());
                }
                    // If there is a '.' transition
                else if (transitions[state].find(".") != transitions[state].end()) {
                    // Add the states reachable from the state on the '.' transition to the set of reachable states
                    const std::unordered_set<std::string>& nextStates = transitions[state]["."];
                    reachableStates.insert(nextStates.begin(), nextStates.end());
                }
            }

            // If there are any reachable states
            if (!reachableStates.empty()) {
                // Convert the set of reachable states to a string to use as the name of the new state in the DFA
                std::string reachableStateName = setToString(reachableStates);

                // If the new reachable state is not already a state in the DFA
                if (std::find(dfa.states.begin(), dfa.states.end(), reachableStateName) == dfa.states.end()) {
                    // Add the new reachable state to the queue of new states to process
                    newStatesQueue.push(reachableStates);
                }

                // Add a transition in the DFA from the new state, on the symbol, to the new reachable state
                dfa.transitions[newStateName][symbol] = reachableStateName;
            }
        }
    }

    // Return the constructed DFA
    return dfa;
}


std::string NFA::setToString(const std::unordered_set<std::string>& stateSet) {
    // Convert the set to a vector
    std::vector<std::string> stateVector(stateSet.begin(), stateSet.end());

    // Sort the vector
    std::sort(stateVector.begin(), stateVector.end());

    // Convert the sorted vector to a string
    std::string stateString = "{";
    for (const std::string& state : stateVector) {
        if (stateString.length() > 1) {
            stateString += ",";
        }
        stateString += state;
    }
    stateString += "}";
    return stateString;
}

bool NFA::accepts(std::string input)
{
    std::string current_states = starting_state;
    for (char c : input)
    {
        std::unordered_set<std::string> next_states;

        for (const std::string& state : std::unordered_set<std::string>{current_states})
        {
            if (transitions[state].find(std::string(1, c)) != transitions[state].end())
            {
                const std::unordered_set<std::string>& next_states_for_symbol = transitions[state][std::string(1, c)];
                next_states.insert(next_states_for_symbol.begin(), next_states_for_symbol.end());
            }
        }

        current_states = *next_states.begin();

        if (current_states.empty())
        {
            return false;
        }
    }

    for (const std::string& state : std::unordered_set<std::string>{current_states})
    {
        if (accepting_states.find(state) != accepting_states.end())
        {
            return true;
        }
    }

    return false;
}