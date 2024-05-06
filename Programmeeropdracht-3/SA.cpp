#include "SA.h"

void SA::addState(State* state)
{
    states.push_back(state);
}

void State::addTransition(State* state, double probability)
{
    transitions[state] = probability;
}