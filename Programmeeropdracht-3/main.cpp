#include "NFA.h"
#include "DFA.h"

int main() {

    NFA nfa("input-ssc1.json");
    nfa.toDFA().print();
    std::cout << nfa.toDFA().accepts("c") << std::endl;

    return 0;
}