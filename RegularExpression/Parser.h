#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>

struct transition {
  std::map<int, std::map<char, std::set<int>>> table;   // transition table sto-
                                                        // ring nodes as keys
                                                        // and a map from char 
                                                        // to set of int
                                                        // as transitions;
};

/*
    A C++ implementation of a Nondeterministic Finite State Autonmata (NFA)
                                                                                
    Conditions:
        -The set of states used by each sets are unique to each NFA. (i.e
*/
class NFA {
 public:
  
    /*
        Initialize NFA with the given alphabet
    */
    NFA(std::set<char> alphabet);

    /* 
        Whether this NFA accepts <str> as input
    */
    bool accepts(const std::string& str);

    /*
        Generates an NFA that admits strings in L(<a>)*
    */
    static NFA kleene_star(NFA a);

    /*
        Generates an NFA that admits strings in L(<a>) union L(<b>)
    */
    static NFA combine(NFA a, NFA b);

    /*
        Generates an NFA that admits strings in L(<a><b>)
    */
    static NFA concat(NFA a, NFA b);

 private:
    transition delta;               // contains all the states & transitions
    std::set<char> alphabet;        // set of alphabet
    std::set<int> accepted_states;  // set of accepted states
    int q0;                         // initial state
};

class Parser {
 public:
    int id_count = 0;       // counts the id used up by all the NFA's to make 
                            // sure no NFA have repeated nodes

    /*
        Parse the regular expression into a NFA
    */
    NFA parse_regex(const std::string& expression);

 private:
    std::vector<char> str_to_tokens(const std::string& expression);
    std::vector<char> tokens_to_RPN(std::vector<char> tokens);
    NFA RPN_to_NFA(std::vector<char> RPN);
};