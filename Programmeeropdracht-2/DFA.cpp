#include "DFA.h"

using json = nlohmann::json;

int main() {

    DFA dfa("DFA.json");
    std::cout << std::boolalpha << dfa.accepts("0010110100") << std::endl << dfa.accepts("0001") << std::endl;
    return 0;
}
