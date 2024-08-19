#include "NFA.h"
#include "DFA.h"

int main() {
    // Uncomment the following lines to test NFA to DFA conversion
    // NFA nfa("input-ssc1.json");
    // nfa.toDFA().print();

    // Uncomment the following lines to test DFA loading and printing
    // DFA dfa("DFA.json");
    // dfa.print();

    // Load two DFAs and compute their intersection
    DFA dfa1("input-product-and1.json");
    DFA dfa2("input-product-and2.json");
    DFA product(dfa1, dfa2, true); // true means intersection, false means union
    product.print();

    return 0;
}