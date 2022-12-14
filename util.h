#ifndef UTIL_H
#define UTIL_H
#include <string>

//Hash Function for a pair
struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
 
        if (hash1 != hash2) {
            return hash1 ^ hash2;             
        }
         
        // If hash1 == hash2, their XOR is zero.
          return hash1;
    }
};

std::string vec_to_string(std::vector<std::string> s)
{
    std::string ret = "";
    for (int i = 0; i < s.size(); ++i)
    {
        ret = ret + s[i] + ',';
    }
    return ret;
}

void print_acceptance(bool a)
{
    if (a)
        std::cout << "String Accepted" << std::endl;
    else
        std::cout << "String Rejected" << std::endl;
}

#endif
