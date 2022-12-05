#ifndef DFA_H
#define DFA_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "util.h"


//DFA class. Need to template it
class DFA
{
public:
    //Constructor for the class
    DFA(std::vector<std::string> sigma, std::vector<std::string> Q,
        std::string start,
        std::unordered_set<std::string> F,
        std::unordered_map<std::pair<std::string, std::string>, std::string, hash_pair> delta)
    {
        start_ = start;
        for(int i = 0; i < sigma.size(); ++i)
        {
            sig_.push_back(sigma[i]);
        }

        for(int i = 0; i < Q.size(); ++i)
        {
            Q_.push_back(Q[i]);
        }
        
        for (auto itr = F.begin(); itr != F.end(); ++itr)
        {
            F_.insert(*itr);
        }

        for(int i = 0; i < sigma.size(); ++i)
        {
            for(int j = 0; j < Q.size(); ++j)
            {
                std::pair<std::string, std::string> p(Q[j], sigma[i]);
                std::string val = (delta.at(p));
                delta_[p] = val;
            }
        }
    }
    //Evaluation function. Takes in a string
    bool evaluate(std::string s)
    {
        state_ = start_;
        for(int i = 0; i < s.size(); ++i)
        {
            std::cout << state_ << ' ' << s[i] << std::endl;
            std::pair<std::string, std::string> key;
            key = std::make_pair(state_, s[i]);
            state_ = delta_[key];
        }
        std::cout << state_ << std::endl;

        if (F_.count(state_) > 0)
            return true;
        return false;
    }
    
    bool operator()(std::string a)
    {
        return evaluate(a);
    }

    //Constructor for a DFA from a DFA
    /*DFA(DFA a)
    {
        sig_ = a.sigma();
        Q_ = a.Q()
            
        }*/

    
    //Function to get the Complement of a DFA
    DFA complement()
    {
        std::unordered_set<std::string> newF;
        for (int i = 0; i < Q_.size(); ++i)
        {
            if (F_.count(Q_[i]) == 0)
            {
                newF.insert(Q_[i]);
            }
        }
        DFA complement(sig_, Q_, start_, newF, delta_);
        return complement;
    }

    //Function to compute the intersection of two DFAs
    DFA intersection(DFA M)
    {
        //First, transfer all states and Final states into this function
        std::vector<std::string> Q = Q_;
        std::vector<std::string> Q1 = M.Q();
        std::vector<std::string> retQ;
        std::unordered_set<std::string> F = F_;
        std::unordered_set<std::string> F1 = M.F();

        //Create a new delta
        std::unordered_map<std::pair<std::string, std::string>, std::string, hash_pair> new_delta;

        //Transfer old delta function from passed machine
        std::unordered_map<std::pair<std::string, std::string>, std::string, hash_pair> delta = M.delta();
        //Create a new State collection
        std::vector<std::string> newQ;
        std::unordered_set<std::string> newF;
        
        //Generate new names for states based on Tuple construction and store in
        //Hash table
        std::unordered_map<std::pair<std::string, std::string>, std::string, hash_pair> nameTable;
        int stateNum = 0;
        for (int i = 0; i< Q.size(); ++i)
        {
            for (int j = 0; j < Q1.size(); ++j)
            {
                std::string newMap = "q" + std::to_string(stateNum);
                nameTable[{Q[i], Q1[j]}] = newMap;
                retQ.push_back(newMap);
                ++stateNum;
            }
        }
        //Create delta using two delta functions
        //If result in F and F', put into new F
        for (int i = 0; i < Q.size(); ++i)
        {
            for (int j = 0; j < Q1.size(); ++j)
            {
                for (int k = 0; k < sig_.size(); ++k)
                {
                    std::string tLeft = delta_[{Q[i], sig_[k]}];
                    std::string tRight = delta[{Q1[j], sig_[k]}];
                    std::string tupleState = nameTable[{Q[i], Q1[i]}];
                    std::string tupleEval = nameTable[{tLeft, tRight}];
                    new_delta[{tupleState, sig_[k]}] = tupleEval;
                    if (F.find(tLeft) != F.end() && F1.find(tRight) != F1.end())
                    {
                        newF.insert(tupleEval);
                    }
                }
            }
        }
        
        //rework
        return DFA(sig_, newQ, newQ[0], newF, new_delta);
    }
    std::vector<std::string> sigma()
    {
        return sig_;
    }

    std::vector<std::string> Q()
    {
        return Q_;
    }

    std::unordered_set<std::string> F()
    {
        return F_;
    }

    std::unordered_map<std::pair<std::string, std::string>, std::string, hash_pair> delta()
    {
        return delta_;
    }

    std::string start()
    {
        return start_;
    }
private:
    std::vector<std::string> sig_;
    std::vector<std::string> Q_;
    std::unordered_set<std::string> F_;
    std::unordered_map<std::pair<std::string, std::string>, std::string, hash_pair> delta_;
    std::string start_;
    std::string state_;
    
};

#endif
