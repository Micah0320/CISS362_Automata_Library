#ifndef NFA_H
#define NFA_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include<bits/stdc++.h>
#include "dfa.h"
#include "util.h"



/*
  Since the delta function of NFAs returns a set of states, we then need an
  unordered_set of strings for delta instead of just a string
 */
//NFA class. Need to template it
class NFA
{
public:
    //Constructor for the class
    NFA(std::vector<std::string> sigma, std::vector<std::string> Q,
        std::string start,
        std::unordered_set<std::string> F,
        std::unordered_map<std::pair<std::string, std::string>, std::vector<std::string>, hash_pair> delta)
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
                //Need to check if the key is valid
                if (delta.find(p) != delta.end())
                {
                    std::vector<std::string> val = (delta.at(p));
                    delta_[p] = val;
                }

                std::pair<std::string, std::string> ep(Q[j], "");
                //Need to check if the key is valid
                if (delta.find(ep) != delta.end())
                {
                    std::vector<std::string> val = (delta.at(ep));
                    delta_[ep] = val;
                }
            }
        }
    }

    //Now we need a way to convert DFAs to NFAs
    NFA(DFA M)
    {
        //Old DFA info
        std::unordered_map<std::pair<std::string, std::string>, std::string, hash_pair> delta = M.delta();
        std::vector<std::string> sigma = M.sigma();
        std::vector<std::string> Q = M.Q();

        //New NFA delta function
        std::unordered_map<std::pair<std::string, std::string>, std::vector<std::string>, hash_pair> delta_ret;
        
        for (int i = 0; i < sigma.size(); ++i)
        {
            for (int j = 0; j < Q.size(); ++j)
            {
                std::pair<std::string, std::string> key;
                key = std::make_pair(Q[j], sigma[i]);
                std::string eval;
                eval = delta[key];
                std::vector<std::string> map;
                map.push_back(eval);
                delta_ret[key] = map;
            }
        }

        //Now we have delta, we can make our return NFA
        sig_ = sigma;
        Q_ = Q;
        start_ = M.start();
        F_ = M.F();
        delta_ = delta_ret;
    }
    
    //Evaluation function. Takes in a string
    bool evaluate(std::string s)
    {
        state_.clear();
        state_.push_back(start_);
        for (int i = 0; i < state_.size(); ++i)
        {
            std::cout << state_[i] << ' ';
        }
        std::cout << std::endl;
        //Handle Epsilon before processing (in case of empty string)
        if (s.size() == 0)
            for (int i = 0; i < state_.size(); ++i)
            {
                std::pair<std::string, std::string> key;
                key = std::make_pair(state_[i], std::string{""});
                std::vector<std::string> evalEp = delta_[key];
                state_.insert(state_.end(), evalEp.begin(), evalEp.end());
            }
        else
        {
        
        //String Case
            for(int i = 0; i < s.size(); ++i)
            {
                std::vector<std::string> eval;
                std::pair<std::string, std::string> key;
                //Check Epsilon
                for (int j = 0; j < state_.size(); ++j)
                {
                    key = std::make_pair(state_[j], std::string{""});
                    std::vector<std::string> evalEp = delta_[key];
                    eval.insert(eval.end(), evalEp.begin(), evalEp.end());
                    //eval.erase(eval.begin() + j);
                }
                //Print State and what is being processed
                for (int j = 0; j < state_.size(); ++j)
                {
                    std::cout << state_[j] << ' ';
                }
                std::cout << s[i] << std::endl;

                //Evaluate
                for (int j = 0; j < state_.size(); ++j)
                {
                    key = std::make_pair(state_[j], s[i]);
                    if (delta_.find(key) != delta_.end())
                    {
                        eval.insert(eval.end(), delta_[key].begin(), delta_[key].end());
                       
                        //std::cout << s[i] << ' ' << std::endl;
                    }
                }
                //Our new state is eval
                state_ = eval;
            }
        }
        for (int i = 0; i < state_.size(); ++i)
        {
            std::cout << state_[i] << ' ';
        }
        std::cout << std::endl;
        for (int i = 0; i < state_.size(); ++i)
        {
            if (F_.count(state_[i]) > 0)
                return true;
        }
        return false;
    }
    bool operator()(std::string a)
    {
        return evaluate(a);
    }
    //Kleene Star Operator
    NFA kleene_star()
    {
        //Create a new Start
        std::string newStart = "qs";
        //Transfer over states
        std::vector<std::string> Q = Q_;
        //Transfer over Final States
        std::unordered_set<std::string> F = F_;
        //Create a new Delta
        std::unordered_map<std::pair<std::string, std::string>, std::vector<std::string>, hash_pair> new_delta;

        std::pair<std::string, std::string> key;
        //Transfer Delta Function
        new_delta = delta_;

        //For all states in the final states, create a transition between them and old start state
        for (auto ptr = F_.begin(); ptr != F_.end(); ++ptr)
        {
            std::string state = *ptr;
            key = std::make_pair(state, "");
            std::vector<std::string> current;
            if (new_delta.find(key) != new_delta.end())
            {
                current = new_delta[key];
            }
            current.push_back(start_);
            new_delta[key] = current;

        }
        //Create an epsilon for the newStart and old Start
        key = std::make_pair(newStart, "");
        //Add new Start to states
        Q.push_back(newStart);
        //Add new Start to final states
        F.insert(newStart);
        std::vector<std::string> c;
        c.push_back(start_);
        new_delta[{newStart, ""}] = c;
        
        return NFA(sig_, Q, newStart, F, new_delta);
    }

    //NFA Union Operation
    NFA NFA_union(NFA n)
    {
        //Create a new Start
        std::string newStart = "qs";
        //Transfer over states
        std::vector<std::string> Q = Q_;
        //Transfer over Final States
        std::unordered_set<std::string> F = F_;
        //Create a new Delta
        std::unordered_map<std::pair<std::string, std::string>, std::vector<std::string>, hash_pair> new_delta;

        //Get Q and F from the passed in function
        std::vector<std::string> Q1 = n.Q();
        std::unordered_set<std::string> F1 = n.F();
        std::unordered_map<std::pair<std::string, std::string>, std::vector<std::string>, hash_pair> delta;
        std::pair<std::string, std::string> key;
        delta = n.delta();
        new_delta = delta_;
        
        //Create a table to map old Q1 names to their new names
        std::unordered_map<std::string, std::string> nameTable;
        int states = Q.size();
        for (int i = 0; i < Q1.size(); ++i)
        {
            int current= i + states;
            std::string newMap = "q" + std::to_string(current); 
            nameTable[Q1[i]] = newMap;
            //Add to states
            Q.push_back(newMap);
        }
        //Creates a new delta start function
        std::vector<std::string> start_map{start_, nameTable[n.start()]};
        key = std::make_pair(newStart, "");
        new_delta[key] =  start_map;
        std::vector<std::string> sig = n.sigma();
        //Create the delta function
        for (int i = 0; i < Q1.size(); ++i)
        {
            for (int j = 0; j < sig.size(); ++i)
            {
                std::string stateName = nameTable[Q1[i]];
                std::string character = sig[j];
                //Pick up here
                new_delta[{stateName, character}] = delta[{Q1[i], character}];
            }
        }

        //Update the sigma
        for( int i = 0; i < sig_.size(); ++i)
        {
            std::string search = sig_[i];
            if (std::count(sig.begin(), sig.end(), search) == 0)
            {
                sig.push_back(sig_[i]);
            }
                
        }
        //Updates Final Accept states
        for (auto ptr = F1.begin(); ptr != F1.end(); ++ptr)
        {
            std::string state = *ptr;
            F.insert(nameTable[state]);
        }
        return NFA(sig, Q, newStart, F, new_delta);
        

    }

    //NFA concat
    NFA NFA_concat(NFA n)
    {
        //Create storage variables for both Q and F.
        std::vector<std::string> Q = Q_;
        std::vector<std::string> Q1 = n.Q();
      
        std::unordered_set<std::string> F = F_;
        std::unordered_set<std::string> F1 = n.F();
        std::unordered_map<std::pair<std::string, std::string>, std::vector<std::string>, hash_pair> delta = n.delta();
        

        //Create a new delta function
        std::unordered_map<std::pair<std::string, std::string>, std::vector<std::string>, hash_pair> new_delta;
        //Transfer over old delta to new delta
        new_delta = delta_;

        //Generate new state names for Q1 states
        std::unordered_map<std::string, std::string> nameTable;
        int states = Q.size();
        for (int i = 0; i < Q1.size(); ++i)
        {
            int current= i + states;
            std::string newMap = "q" + std::to_string(current); 
            nameTable[Q1[i]] = newMap;
            //Add to states
            Q.push_back(newMap);
        }

        std::vector<std::string> sig = n.sigma();
        //update sigma
        std::vector<std::string> sigma = sig_;
        for (int i = 0; i < sig.size(); ++i)
        {
            std::string search = sig[i];
            if (std::count(sig.begin(), sig.end(), search) == 0)
            {
                sigma.push_back(sig[i]);
            }
            
        }
        
        //Create the delta function
        for (int i = 0; i < Q1.size(); ++i)
        {
            for (int j = 0; j < sig.size(); ++i)
            {
                std::string stateName = nameTable[Q1[i]];
                std::string character = sig[j];
                new_delta[{stateName, character}] = delta[{Q1[i], character}];
            }
        }

        //Transition from accept states of original NFA to start of passed in NFA
        for (auto ptr = F.begin(); ptr != F.end(); ++ptr)
        {
            std::string state = *ptr;
            new_delta[{state, ""}] = {Q1[0]};
        }

        //Clear Final states in F and overwrite with final states in F1
        F.clear();
        for (auto ptr = F1.begin(); ptr != F1.end(); ++ptr)
        {
            std::string state = *ptr;
            std::string stateName = nameTable[state];
            F.insert(stateName);
        }
        
        //Return an NFA with updated Q, F, sigma, and Delta
        return NFA(sigma, Q, start_, F, new_delta);
    }

    std::unordered_map<std::pair<std::string, std::string>, std::vector<std::string>, hash_pair> delta()
    {
        return delta_;
    }
    std::string start()
    {
        return start_;
    }

    std::vector<std::string> Q()
    {
        return Q_;
    }

    std::unordered_set<std::string> F()
    {
        return F_;
    }

    std::vector<std::string> sigma()
    {
        return sig_;
    }
    
private:
    std::vector<std::string> sig_;
    std::vector<std::string> Q_;
    std::unordered_set<std::string> F_;
    std::unordered_map<std::pair<std::string, std::string>, std::vector<std::string>, hash_pair> delta_;
    std::string start_;
    std::vector<std::string> state_;
    
};



#endif
