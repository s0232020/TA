/**
 * SA.cpp
 * Markov model implementation
 * A stochastic automata model that predicts the next state based on the current state and the probability of each transition.
 * Jelle Jacobs
 */

#include <sstream>
#include <fstream>
#include <iostream>
#include "SA.h"

SA::SA() {
    for (char l1 = 'a'; l1 <= 'z'; ++l1) {
        for (char l2 = 'a'; l2 <= 'z'; ++l2) {
            pairProbabilities[{l1, l2}] = 0;
        }
    }
}

void SA::countOccurrences(std::string& input, bool trainVariablesOnly) {
    if (!trainVariablesOnly) {
        std::ifstream inFile(input);
        if (!inFile.is_open()) {
            std::cerr << "Error opening file: " << input << std::endl;
            return;
        }
        char previous = ' ';
        std::string line;

        while (std::getline(inFile, line)) {
            for (char& c : line) {
                if (previous == ' ') {
                    letterOccurrences[c]++;
                    previous = c;
                    continue;
                } else if (c == ' ') {
                    previous = c;
                    continue;
                }
                letterOccurrences[c]++;
                pairProbabilities[{previous, c}]++;
                if (c == ';') {
                    previous = ' ';
                } else {
                    previous = c;
                }
            }
        }
    } else {
        char previous = ' ';
        for (char& c : input) {
            if (previous == ' ') {
                letterOccurrences[c]++;
                previous = c;
                continue;
            } else if (c == ' ') {
                previous = c;
                continue;
            }
            letterOccurrences[c]++;
            pairProbabilities[{previous, c}]++;
            if (c == ';') {
                previous = ' ';
            } else {
                previous = c;
            }
        }
    }

    for (auto& pair : pairProbabilities) {
        double occurrencePair = pairProbabilities[pair.first];
        double occurrenceLetter = letterOccurrences[pair.first.first];
        if (occurrenceLetter == 0) {
            continue;
        }
        double probabilityPair = occurrencePair / occurrenceLetter;
        pairProbabilities[pair.first] = probabilityPair;
    }
}

char SA::calculateProbability(char c) {
    double highestProbability = 0;
    char mostLikely;
    for (auto& pair : pairProbabilities) {
        if (pair.first.first == c) {
            if (highestProbability < pair.second) {
                highestProbability = pair.second;
                mostLikely = pair.first.second;
            }
        }
    }
    return mostLikely;
}

void SA::addState(state* state) {
    states.push_back(state);
}

void SA::setAlphabet(const std::vector<char>& alph) {
    alphabet = alph;
}

void SA::setStartState(state* state) {
    startState = state;
}

void SA::addTransition(state* from, char symbol, state* to, double probability) {
    transitions[{from, symbol}] = {to, probability};
}

state* SA::predictNextState(state* currentState) {
    state* nextState = nullptr;
    double maxProbability = -1.0;

    for (const auto& transition : transitions) {
        if (transition.first.first == currentState && transition.second.second > maxProbability) {
            nextState = transition.second.first;
            maxProbability = transition.second.second;
        }
    }

    return nextState;
}

std::vector<std::string> SA::findWithPrefix(std::string& prefix) {
    std::vector<std::string> result;
    bool keepLooking = true;

    while (keepLooking) {
        std::vector<std::string> tempResult;
        for (const auto& str : strings) {
            if (str.substr(0, prefix.size()) == prefix && str != prefix) {
                tempResult.push_back(str);
            }
        }
        if (tempResult.empty()) {
            keepLooking = false;
        } else {
            for (const auto& w : result) {
                if (std::find(tempResult.begin(), tempResult.end(), w) == tempResult.end()) {
                    tempResult.push_back(w);
                }
            }
            result = tempResult;
            char lastLetter = prefix.back();
            char extraChar = calculateProbability(lastLetter);
            prefix += extraChar;
        }
    }
    return result;
}

void SA::addString(const std::string& input) {
    strings.push_back(input);
}

void SA::addPair(const std::pair<char, char>& input) {
    if (pairProbabilities.find(input) != pairProbabilities.end()) {
        pairProbabilities[input]++;
    } else {
        pairProbabilities[input] = 1;
    }
}

void SA::addLetter(char c) {
    if (letterOccurrences.find(c) != letterOccurrences.end()) {
        letterOccurrences[c]++;
    } else {
        letterOccurrences[c] = 1;
    }
}