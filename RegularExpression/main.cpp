#include "Parser.h"
#include <assert.h>
#include <iostream>
int main() { 
    Parser parser({'a', 'b', 'c', 'd'});

    // strings that start with a and end with b
    NFA nfa = parser.parse_regex("(a(a+b)*b)*");
    nfa.simplify();
    assert(nfa.accepts("abab") && !nfa.accepts("babababbaab"));
    
    while(true) {
        std::cout << "Enter regular expression (^ to exit): \n" ;
        std::string input;
        std::cin >> input;
        if(input == "^")
            break;
        nfa = parser.parse_regex(input);    nfa.simplify();
        std::cout << nfa.str();
        while(true) {
            std::cout << "Enter a string (^ to exit): \n";
            std::cin >> input;
            if(input == "^")
                break;
            if(nfa.accepts(input)) {
                std::cout << "Accepted!\n";
            } else {
                std::cout << "Rejected!\n";
            }
        }
    }
    return 0;
}