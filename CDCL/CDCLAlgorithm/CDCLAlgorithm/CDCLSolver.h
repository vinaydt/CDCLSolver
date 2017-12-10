#ifndef CDCLSOLVER_H
#define CDCLSOLVER_H

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_set>
#include <deque>
#include <unordered_map>
#include "Clause.h"
#include "ImplicationGraphNode.h"
using namespace std;

class CDCLSolver
{
    public:
        int noOfVariables;
        vector<Clause> clauses;
        deque<int> interestingClauses;
        unordered_map<int, ImplicationGraphNode> thisImplicationGraph;
        CDCLSolver(int, vector<Clause>, bool);
        void printSATAssignment();
        void findSingletons();
        pair<int, bool> makeDecision();
        void unitPropagate(int);
        void backtrack(int);
        pair<int, bool> unitRule(Clause);
        int pickClause(int);
        void moveWatch(int);
        int analyzeLearn(int);
        ~CDCLSolver();
		bool printLogs = false;
};

#endif
