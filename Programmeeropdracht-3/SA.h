/**
 * SA.h
 * Author: Jelle Jacobs
 */

#ifndef PROGRAMMEEROPDRACHT_3_SA_H
#define PROGRAMMEEROPDRACHT_3_SA_H

#include <map>
#include <vector>
#include <set>
#include <string>

typedef std::string state;

class SA {
private:
    std::vector<state*> states;
    std::vector<char> alphabet;
    state* startState;
    std::map<std::pair<state*, char>, std::pair<state*, double>> transitions;
    std::vector<std::string> strings;

    std::map<char, int> letterOccurrences;
    std::map<std::pair<char, char>, double> pairProbabilities;

public:
    SA();

    void countOccurrences(std::string& input, bool trainVariablesOnly);

    char calculateProbability(char c);

    void addState(state* state);

    void setAlphabet(const std::vector<char>& alph);

    void setStartState(state* state);

    void addTransition(state* from, char symbol, state* to, double probability);

    state* predictNextState(state* currentState);

    std::vector<std::string> findWithPrefix(std::string& prefix);

    void addString(const std::string& input);

    void addPair(const std::pair<char, char>& input);

    void addLetter(char c);
};

#endif // PROGRAMMEEROPDRACHT_3_SA_H