#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>

/*
    transition table storing nodes as keys and a map from char to set of int as
    transitions
*/
typedef std::map<int, std::map<char, std::set<int>>> transition;

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
        Returns the string representation of this NFA
    */
    std::string str();

    /*
        Simplifies the NFA by removing & transitions
    */
    void simplify();

    /*
        Generates an NFA that admits strings in L(<a>)*
    */
    static NFA kleene_star(NFA a);

    /*
        Generates an NFA that admits strings in L(<a>) union L(<b>)
        -<a> and <b> are over the same alphabet
    */
    static NFA combine(NFA a, NFA b, int new_q0);

    /*
        Generates an NFA that admits strings in L(<a><b>)
        -<a> and <b> are over the same alphabet
    */
    static NFA concat(NFA a, NFA b);


    transition delta;               // contains all the states & transitions
    std::set<char> alphabet;        // set of alphabet
    std::set<int> accepted_states;  // set of accepted states; invariant: this
                                    // is a subset of the keys of <delta>
    int q0;                         // initial state

private:
    std::set<int> _tr(int state, std::set<int>* traversed);
};

class Parser {
 public:
    /*
        Parser that parses regular expression over <alphabet>
    */
    Parser(std::set<char> alphabet);
    std::set<char> alphabet;
    int id_count;       // counts the id used up by all the NFA's to make 
                        // sure no NFA have repeated nodes

    /*
        Parse the regular expression into a NFA
        -<str> is a valid regular expression (spaces are fine)
    */
    NFA parse_regex(const std::string& str);

 private:
    std::vector<char> str_to_tokens(const std::string& expression);
    std::vector<char> tokens_to_RPN(std::vector<char> tokens);
    NFA RPN_to_NFA(std::vector<char> RPN);
};