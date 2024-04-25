#include "NFA.h"
#include "DFA.h"

int main() {

    NFA nfa("input-ssc2.json");
    nfa.toDFA().print();



    return 0;
}