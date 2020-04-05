#include "Parser.h"
#include <stack>

NFA::NFA(std::set<char> alphabet) {
    this->alphabet = alphabet;
}

Parser::Parser(std::set<char> alphabet) {
    this->alphabet = alphabet;
    this->id_count = 0;
}

NFA Parser::parse_regex(const std::string& str) { 
    std::vector<char> tokens = this->str_to_tokens(str);
    std::vector<char> RPN = this->tokens_to_RPN(tokens);
    return this->RPN_to_NFA(RPN);
}

std::vector<char> Parser::str_to_tokens(const std::string& str) { 
    std::string exp = str;
    std::set<char> not_l{ '+', '.', '(' };
    std::set<char> not_r{ '*', ')', '+', '.' };
    exp.erase(
        std::remove_if(exp.begin(), exp.end(), std::isspace),
        exp.end());
    std::vector<char> tokens;
    for(int i = 0; i < exp.length(); i++) {
        tokens.push_back(exp[i]);
        // manually add . in between to denote concatenation
        if(i + 1 < exp.length()) {
            char l = exp[i]; char r = exp[i + 1];
            if(not_l.find(l) == not_l.end() && not_r.find(r) == not_r.end()) {
                tokens.push_back('.');
            }
        }
    }
    return tokens;
}

std::vector<char> Parser::tokens_to_RPN(std::vector<char> tokens) { 
    std::vector<char> RPN;
    std::stack<char> st;
    std::map<char, int> op_prec = {{'*', 1}, {'.', 2}, {'+', 3} };
    for(char c : tokens) {
        if(alphabet.find(c) != alphabet.end()) {
            RPN.push_back(c);
        }else {
            switch(c) {
                case '(':
                    st.push(c);
                    break;
                case ')':
                    while(st.top() != '(') {
                        RPN.push_back(st.top());
                        st.pop();
                    }
                    st.pop();
                    break;
                default:    // c in {'*', '.', '+'}
                    while(!st.empty() && op_prec[st.top()] <= op_prec[c] 
                            && st.top() != '(') {
                        RPN.push_back(st.top());
                        st.pop();
                    }
                    st.push(c);
            }
        }
    }
    while(!st.empty()) {
        RPN.push_back(st.top());
        st.pop();
    }
    return RPN;
}

NFA Parser::RPN_to_NFA(std::vector<char> RPN) {
    return NFA(this->alphabet);
}