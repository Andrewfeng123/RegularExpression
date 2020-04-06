#include "Parser.h"
#include <stack>
#include <sstream>

template<class T>
bool disjoint(std::set<T> a, std::set<T> b) {
    std::set<T> c = a;
    c.insert(b.begin(), b.end());
    return c.size() == a.size() + b.size();
}

bool NFA::accepts(const std::string& str) {
    std::set<int> reached_states = {q0};
    reached_states.insert(delta[q0]['&'].begin(), delta[q0]['&'].end());
    for(char c : str) {
        std::set<int> newly_traversed;
        for(auto i : reached_states) {
            newly_traversed.insert(delta[i][c].begin(), delta[i][c].end());
        }
        reached_states = newly_traversed;
    }
    return !disjoint(reached_states, accepted_states);
}

void NFA::simplify() {
    // directly connect any path containing &
    for(auto& i : delta) {
        for(auto& j: i.second) {
            char c = j.first;
            std::set<int> to_add;
            for(auto d : j.second) {
                std::set<int> traversed;
                std::set<int> connected = _tr(d, &traversed);
                to_add.insert(connected.begin(), connected.end());
            }
            j.second.insert(to_add.begin(), to_add.end());
        }
    }
    // remove unneeded & transitions (all except those coming from q0)
    for(auto& i : delta) {
        if(i.first != q0) {
            i.second['&'] = std::set<int>();
        }
    }
}

std::set<int> NFA::_tr(int state, std::set<int>* traversed) {
    (*traversed).insert(state);
    std::set<int> result = {state};
    for(int s : delta[state]['&']) {
        if(traversed->find(s) == traversed->end()) {
            std::set<int> res = _tr(s, traversed);
            result.insert(res.begin(), res.end());
        }
    }
    return result;
}

std::string NFA::str() {
    std::stringstream ss;
    ss << "accepted: \n";
    for(auto i :accepted_states) {
        ss << i << '\n';
    }
    ss << "q0: " << q0 << "\n";
    for(auto i : delta) {
        ss << i.first << ": \n";
        for(auto j : i.second) {
            ss << "\t" << j.first << "-> \n";
            for(auto k : j.second) {
                ss << "\t\t" << k << "\n";
            }
        }
    }
    return ss.str();
}

NFA NFA::kleene_star(NFA a) {
    NFA b = a;
    for(auto i : b.accepted_states) {
        b.delta[i]['&'].insert(b.q0);
    }
    b.accepted_states.insert(b.q0);
    return b;
}

NFA NFA::combine(NFA a, NFA b, int new_q0) {
    NFA c(a.alphabet);
    c.q0 = new_q0;
    c.delta.insert(a.delta.begin(), a.delta.end());
    c.delta.insert(b.delta.begin(), b.delta.end());
    c.delta[c.q0] = std::map<char, std::set<int>>();
    c.delta[c.q0]['&'] = {a.q0, b.q0};
    for(char ch : c.alphabet) {
        c.delta[c.q0][ch] = std::set<int>();
    }
    c.accepted_states = a.accepted_states; 
    c.accepted_states.insert(b.accepted_states.begin(),
        b.accepted_states.end());
    return c;
}

NFA NFA::concat(NFA a, NFA b) {
    NFA c(a.alphabet);
    c.q0 = a.q0;
    c.delta.insert(a.delta.begin(), a.delta.end());
    c.delta.insert(b.delta.begin(), b.delta.end());
    for(auto i : a.accepted_states) {
        c.delta[i]['&'].insert(b.q0);
    }
    c.accepted_states = b.accepted_states;
    return c;
}

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
    for(unsigned i = 0; i < exp.length(); i++) {
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
    std::vector<std::pair<NFA, char>> terms;
    std::set<char> ops {'*', '.', '+'};
    for(char c : RPN) {
        if(ops.find(c) == ops.end()) {
            NFA a(alphabet);
            a.q0 = id_count++;
            int accepted = id_count++;
            a.accepted_states.insert(accepted);
            a.delta[a.q0] = std::map<char, std::set<int>>();
            for (char l : alphabet) {
                a.delta[a.q0][l] = std::set<int>();
            }
            a.delta[a.q0]['&'] = std::set<int>();
            a.delta[accepted] = std::map<char, std::set<int>>();
            for (char l : alphabet) {
                a.delta[accepted][l] = std::set<int>();
            }
            a.delta[accepted]['&'] = std::set<int>();
            // handle emptyset symbol, empty string symbol, or alphabet
            if(c == '%') {
                // emptyset, do nothing
            }else {
                // empty string (denoted as &) or alphabet
                a.delta[a.q0][c].insert(accepted);
            }
            terms.push_back(std::make_pair(a, 0));
        } else {
            terms.push_back(std::make_pair(NFA(alphabet), c));
        }
    }
    std::stack<NFA> st;
    for(auto t : terms) {
        if (std::get<1>(t) != 0) {
            switch(std::get<1>(t)) {
            case '*': {
                NFA a = st.top(); st.pop();
                st.push(NFA::kleene_star(a));
                break;}
            case '.': {
                NFA b = st.top(); st.pop();
                NFA a = st.top(); st.pop();
                st.push(NFA::concat(a, b));
                break;}
            case '+': {
                NFA b = st.top(); st.pop();
                NFA a = st.top(); st.pop();
                st.push(NFA::combine(a, b, id_count));
                break;}
            }
        }else {
            st.push(std::get<0>(t));
        }
    }
    return st.top();
}