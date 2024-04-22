#include "NFA.h"

int main() {
    NFA nfa("input-ssc1.json");
    //nfa.toDFA().print();
    std::cout << nfa.accepts("c") << std::endl;
    std::cout << nfa.accepts("cc") << std::endl;
    std::cout << nfa.accepts("ccc") << std::endl;
    std::cout << nfa.accepts("cccc") << std::endl;
    std::cout << nfa.accepts("ccccc") << std::endl;


    return 0;
}