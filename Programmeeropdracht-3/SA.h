#ifndef PROGRAMMEEROPDRACHT_3_SA_H
#define PROGRAMMEEROPDRACHT_3_SA_H
#include <map>
#include <vector>

class State {
public:
    std::map<State*, double> transitions;

    void addTransition(State* state, double probability);
};

class SA {
public:
    std::vector<State*> states;

    void addState(State* state);
};

#endif //PROGRAMMEEROPDRACHT_3_SA_H
