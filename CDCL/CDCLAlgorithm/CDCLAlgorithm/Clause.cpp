#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include "Clause.h"
using namespace std;

Clause :: Clause(vector<int> variablesVector)
{
    int n = variablesVector.size();
    int var;
    bool sign;
    for(int i = 0; i < n; i++)
    {
        var = abs(variablesVector[i]);
        sign = (variablesVector[i]/var == 1 ? true : false);
        variableSignMap[var] = sign;
        if(watchedSet.size() < 2)
            watchedSet.insert(var);
    }
}

Clause :: ~Clause()
{
}
