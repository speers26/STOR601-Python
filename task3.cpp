#include <iostream>
#include <unordered_map> 
#include <vector>
#include <utility>
#include <algorithm>

typedef std::unordered_map<std::string,std::vector<std::string>> pref_table;
typedef std::unordered_map<std::string,std::string> match;
typedef std::tuple<pref_table,pref_table,match> output;

output fund2(int N)
{
    match matching;
    std::string x;
    std::string X;
    std::string x_fiance;
    int ind_new;
    int ind_old;
    std::string s;
    std::vector<std::string> males;
    std::vector<std::string> females;
    
    // create males and females
    for (int i = 0; i < N; i++)
    {
        s = std::to_string(i);
        males.push_back("M"+s);
        females.push_back("F"+s);
    }
       
    // create female preference table
    pref_table female_pref;
    for (const auto& it : females)
    {
        std::random_shuffle (males.begin(),males.end());
        female_pref[it]=males;
    }
    
    // create male preference table
    pref_table male_pref;
    for (const auto& it : males)
    {
        std::random_shuffle (females.begin(),females.end());
        male_pref[it]=females;
    }
    
    // copy original versions of preference tables
    pref_table old_male_pref {male_pref};
    pref_table old_female_pref {female_pref};
    
    // first add the imaginary undesirable man 
    for (const auto& it : female_pref)
    {
        female_pref.at(it.first).push_back("O");
    }
    
    // engage all women to imaginary man
    for (const auto& it : females)
    {
        matching[it]="O";
    }
    
    // main loop
    for (const auto& it : males)
    {
        X = it;
        while (X!="O")
        {
            x = male_pref[X][0];
            x_fiance = matching[x];
            
            // find indices of current and potential spouse
            auto i = std::find(female_pref[x].begin(), female_pref[x].end(), x_fiance);
            ind_old = std::distance(female_pref[x].begin(), i);
            i = std::find(female_pref[x].begin(), female_pref[x].end(), X);
            ind_new = std::distance(female_pref[x].begin(), i);
 
            // pair female with current male if they prefer
            if (ind_new < ind_old)
            {
                matching[x]=X;
                X = x_fiance;
            }
            
            // if the man isn't imaginary, remove current best choice from his list
            if (X != "O")
            {
                male_pref[X].erase (male_pref[X].begin());
            }
        }
    }
    output out (old_male_pref,old_female_pref,matching);
    return out;
}

int main()
{
    int N;
    std::cout << "Enter number of males and females:" << std::endl;
    std::cin >> N;  
    
    output test_output {fund2(N)};

    // print male preferences	    
    std::cout << "Male preference table:" << std::endl;
    for (const auto& it : std::get<0>(test_output))
    {
        std::cout << "(" << it.first << ":";
        for (const auto& jt : it.second)
        {
            std::cout << jt << ",";
        }
        std::cout << ")" << std::endl;
    }
    
    // print female preferences
    std::cout << "Female preference table:" << std::endl;	 
    for (const auto& it : std::get<1>(test_output))
    {
        std::cout << "(" << it.first << ":";
        for (const auto& jt : it.second)
        {
            std::cout << jt << ",";
        }
        std::cout << ")" << std::endl;
    }

    // print stable matching
    std::cout << "Stable matching:" << std::endl;
    for (const auto& it : std::get<2>(test_output))
    {
        std::cout << it.first <<":"<< it.second << std::endl;
    }   

    return 0;
}

