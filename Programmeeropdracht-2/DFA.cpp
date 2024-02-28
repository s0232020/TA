#include "DFA.h"

using namespace std;
using json = nlohmann::json;

int main() {
    // inlezen uit file
    ifstream input("DFA.json");

    json j;
    input >> j;
    // tonen op het scherm
    cout << setw(4) << j << endl;

    // manueel aanmaken
    json j2;
    j2["type"] = "DFA";
    j2["alphabet"] = {"0", "1"};
    cout << setw(4) << j2 << endl;


    DFA dfa("DFA.json");
    cout << boolalpha << dfa.accepts("0010110100") << endl << dfa.accepts("0001") << endl;
    return 0;
}
