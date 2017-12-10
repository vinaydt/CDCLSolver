#include <iostream>
#include <vector>
#include <cstdio>
#include <ctime>
#include <cstring>
#define _USE_MATH_DEFINES
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <fstream>
#include <sstream>
#include "Clause.h"
#include "CDCLSolver.h"
#include "ImplicationGraphNode.h"
#define TIME_LIMIT 300
using namespace std;

clock_t start;

int main(int argc, char* argv[])
{
    ifstream input;
    int noOfVar;
    long long int noOfClauses;
    string token;
    vector<int> literals;
    vector<Clause> clauses;
    istringstream stringStreamVar;
    double timeElapsedInSeconds = 0.0;
    pair<int, bool> decision;
    int decisionCount = 0;

    input.open(argv[1]);
    if(!input.is_open())
    {
        cout << "The file does not exist or could not be opened" << endl;
		cin.get();
        return 1;
    }
    for(string line; getline(input, line, '\n');)
    {
        stringStreamVar.str(line);
        if(line[0] == 'c' || line[0] == ' ')
            continue;
        if(line[0] == 'p')
        {
            stringStreamVar >> token;
            stringStreamVar >> token;
            stringStreamVar >> token;
            noOfVar = stoi(token);
            stringStreamVar >> token;
            noOfClauses = stoll(token);
        }
        else
        {
            while(getline(stringStreamVar, token, ' '))
            {
                if(token == "0")
                    break;
				try {
					literals.push_back(stoi(token));
				}
				catch (...) {
				}
            }
            clauses.push_back(Clause(literals));
            literals.clear();
        }
        stringStreamVar.clear();
    }

	bool printFlag = false;
	if (argv[2]!=NULL && strcmp(argv[2],"true")==0)
		printFlag = true;
	
    CDCLSolver solver(noOfVar, clauses, printFlag);
    solver.findSingletons();

    start = clock();
    while(true)
    {
        solver.unitPropagate(decisionCount);
        if(solver.thisImplicationGraph.find(-1) == solver.thisImplicationGraph.end())
        {
            if(solver.thisImplicationGraph.size() == solver.noOfVariables)
            {
                cout << "SAT" << endl;
                solver.printSATAssignment();
                break;
            }
            decision = solver.makeDecision();
            decisionCount++;
            for(int i = 0; i < solver.clauses.size(); i++)
            {
                if(solver.clauses[i].variableSignMap.find(decision.first) != solver.clauses[i].variableSignMap.end())
                    solver.interestingClauses.push_back(i);
            }
            solver.thisImplicationGraph.insert(make_pair(decision.first, ImplicationGraphNode(decision.second, decisionCount)));
        }
        else
        {
            decisionCount = solver.analyzeLearn(decisionCount);
			if(printFlag)
				cout << "Backtracking to level " << decisionCount << endl;
            if(decisionCount < 0)
            {
                cout << "UNSAT" << endl;
                break;
            }
            solver.backtrack(decisionCount);
            solver.thisImplicationGraph.erase(-1);
        }
        timeElapsedInSeconds = (clock() - start) / (double) CLOCKS_PER_SEC;
        if(timeElapsedInSeconds > TIME_LIMIT)
        {
            cout << "TIMEOUT" << endl;
            break;
        }
    }
	cin.get();
    return 0;
}
