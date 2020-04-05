#include "Parser.h"
#include <assert.h>
int main() { 
    Parser parser({'a', 'b', 'c'});

    // strings that start with a and has b for remaining characters
    NFA nfa = parser.parse_regex("ab*");
    assert(nfa.accepts("ab") && nfa.accepts("abbb"));

    // strings that start with a and end with b
    nfa = parser.parse_regex("a(a+b)*b");
    assert(nfa.accepts("abab") && nfa.accepts("aabababbaab"));
    std::vector<char> char_arr = parser.str_to_tokens("aabb(a+b+c)*(bab+c)");
    char_arr = parser.tokens_to_RPN(char_arr);
    getchar();
}