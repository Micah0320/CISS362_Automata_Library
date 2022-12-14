#include <iostream>
#include "dfa.h"
#include "nfa.h"

//Testing the NFA
int main ()
{
    /*
    std::string a = "a";
    std::string b = "b";
    std::string ep = "b";
    std::vector< std::string > S;
    S.push_back(a);
    S.push_back(b);
    S.push_back(ep);// Sigma

    std::string q0 = "q0";
    std::string q1 = "q1";
    std::string q2 = "q2";
    std::string q3 = "q3";
    std::vector< std::string > Q = {q0,q1,q2,q3};

    std::unordered_set< std::string > F {q3};
    std::unordered_map<std::pair<std::string, std::string>, std::vector<std::string>, hash_pair> delta;

    delta[{q0, a}] = std::vector<std::string>{q0, q1};
    delta[{q0, b}] = std::vector<std::string>{q1, q2};
    delta[{q1, a}] = std::vector<std::string>{q3};
    delta[{q2, a}] = std::vector<std::string>{q2, q3};
    delta[{q2, b}] = std::vector<std::string>{q3};
    delta[{q3, a}] = std::vector<std::string>{q3};
    delta[{q3, b}] = std::vector<std::string>{q3};
    
    NFA M(S, Q, q0, F, delta);
    bool eval =  M.evaluate("aabababaa");
    print_acceptance(eval);
    print_acceptance(M("aabab"));
    */

    //DFA
    std::string a = "a";
    std::string b = "b";
    std::vector< std::string > S;
    S.push_back(a);
    S.push_back(b);// Sigma
    std::string q0 = "q0";
    std::string q1 = "q1";
    std::vector< std::string > Q;
    Q.push_back(q0);
    Q.push_back(q1);// Q
    std::unordered_set< std::string > F {q1}; // F
    std::unordered_map<std::pair<std::string, std::string>, std::string, hash_pair> delta; //delta
    delta[{q0, a}] = q0;
    delta[{q0, b}] = q1;
    delta[{q1, a}] = q1;
    delta[{q1, b}] = q0;
    DFA M1(S, Q, q0, F, delta);
    std::cout << "DFA Evaluation" << std::endl;
    print_acceptance(M1("aababb"));
    print_acceptance(M1("aaaaaa"));
    

    NFA nDFA = M1;
    std::cout << "NFA Evaluation" << std::endl;
    print_acceptance(nDFA("aaaaaa"));
    std::cout << "Converted DFA Evaluation" << std::endl;
    DFA M2( nDFA.convert_to_DFA());
    print_acceptance(M2("aababb"));
    print_acceptance(M2("aaa"));
    /*
    */
    return 0;
}

/*
*/
