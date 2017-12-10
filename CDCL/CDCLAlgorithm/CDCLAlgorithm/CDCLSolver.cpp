#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <utility>
#include <deque>
#include <queue>
#include "CDCLSolver.h"
#include "ImplicationGraphNode.h"
using namespace std;

CDCLSolver :: CDCLSolver(int numVariables, vector<Clause> cl, bool _printFlag)
{
    noOfVariables = numVariables;
    clauses = cl;
    bool temp;
	printLogs = _printFlag;
    return;
}

void CDCLSolver :: printSATAssignment()
{
    for(int i = 1; i <= noOfVariables; i++)
    {
        if(thisImplicationGraph[i].value)
            cout << i << " ";
        else
            cout << -1 * i << " ";
    }
    cout << endl;
}


void CDCLSolver::backtrack(int d)
{
	unordered_set<int> deleteNodes;
	for (auto it = thisImplicationGraph.begin(); it != thisImplicationGraph.end(); it++)
	{
		if (thisImplicationGraph[it->first].d > d)
		{
			deleteNodes.insert(it->first);
		}
	}
	for (auto it = deleteNodes.begin(); it != deleteNodes.end(); it++)
	{
		thisImplicationGraph.erase(*it);
	}
	for (auto it = thisImplicationGraph.begin(); it != thisImplicationGraph.end(); it++)
	{
		for (auto iter = deleteNodes.begin(); iter != deleteNodes.end(); iter++)
		{
			if (thisImplicationGraph[it->first].parentsOfThisNode.find(*iter) != thisImplicationGraph[it->first].parentsOfThisNode.end())
			{
				thisImplicationGraph[it->first].parentsOfThisNode.erase(*iter);
			}
			else if (thisImplicationGraph[it->first].childrenOfThisNode.find(*iter) != thisImplicationGraph[it->first].childrenOfThisNode.end())
			{
				thisImplicationGraph[it->first].childrenOfThisNode.erase(*iter);
			}

		}
	}
	return;
}

void CDCLSolver :: findSingletons()
{
    int n = clauses.size();
    for(int i = 0; i < n; i++)
    {
        if(clauses[i].variableSignMap.size() == 1)
        {
            interestingClauses.push_back(i);
            cout << "adding " << i << " to interesting clauses" << endl;
        }
    }
}

pair<int, bool> CDCLSolver :: makeDecision()
{
    int next;
    bool truthValue;
    while(true)                                                                                                                                                              
    {
        next = rand() % noOfVariables + 1;
        if(thisImplicationGraph.find(next) != thisImplicationGraph.end())
            continue;
        truthValue = (rand() % 2 == 0 ? false : true);
        break;
    }
    return make_pair(next, truthValue);
}

pair<int, bool> CDCLSolver :: unitRule(Clause c)
{
    for(auto it = c.variableSignMap.begin(); it != c.variableSignMap.end(); it++)
    {
        if(thisImplicationGraph.find(it->first) == thisImplicationGraph.end())
        {
            return make_pair(it->first, it->second);
        }
    }
}

void CDCLSolver :: moveWatch(int c)
{
    for(auto it = clauses[c].variableSignMap.begin(); it != clauses[c].variableSignMap.end(); it++)
    {
        if(clauses[c].watchedSet.find(it->first) != clauses[c].watchedSet.end())
            continue;
        if(thisImplicationGraph.find(it->first) == thisImplicationGraph.end())
        {
            clauses[c].watchedSet.insert(it->first);
            return;
        }
        if(thisImplicationGraph[it->first].value == it->second)
        {
            clauses[c].watchedSet.insert(it->first);
            return;
        }
    }
}

int CDCLSolver :: pickClause(int d)
{
    int c;
    bool jumpToNextIteration;
    int w0, w1;
    unordered_set<int>::iterator it;
    while(!interestingClauses.empty())
    {
        c = *(interestingClauses.begin());
        interestingClauses.pop_front();
        if(clauses[c].watchedSet.size() == 0)
            moveWatch(c);
        if(clauses[c].watchedSet.size() == 1)
            moveWatch(c);
        if(clauses[c].watchedSet.size() == 0)
        {
            thisImplicationGraph.insert(make_pair(-1, ImplicationGraphNode(true, d)));
            return c;
        }
        if(clauses[c].watchedSet.size() == 1)
        {
            if(thisImplicationGraph.find(*(clauses[c].watchedSet.begin())) == thisImplicationGraph.end())
                return c;
            if(thisImplicationGraph[*(clauses[c].watchedSet.begin())].value == clauses[c].variableSignMap[*(clauses[c].watchedSet.begin())])
                continue;
            thisImplicationGraph.insert(make_pair(-1, ImplicationGraphNode(true, d)));
            return c;
        }
        jumpToNextIteration = false;
        for(auto it = clauses[c].watchedSet.begin(); it != clauses[c].watchedSet.end(); it++)
            if(thisImplicationGraph.find(*it) != thisImplicationGraph.end() && thisImplicationGraph[*it].value == clauses[c].variableSignMap[*it])
                jumpToNextIteration = true;
        if(jumpToNextIteration)
            continue;
        it = clauses[c].watchedSet.begin();
        w0 = *it;
        it++;
        w1 = *it;
        if(thisImplicationGraph.find(w0) != thisImplicationGraph.end() && thisImplicationGraph[w0].value == clauses[c].variableSignMap[w0])
            continue;
        if(thisImplicationGraph.find(w1) != thisImplicationGraph.end() && thisImplicationGraph[w1].value == clauses[c].variableSignMap[w1])
            continue;
        if(thisImplicationGraph.find(w0) == thisImplicationGraph.end())
        {
            if(thisImplicationGraph.find(w1) == thisImplicationGraph.end())
                continue;
            clauses[c].watchedSet.erase(w1);
            moveWatch(c);
            if(clauses[c].watchedSet.size() == 2)
                continue;
            return c;
        }
        else
        {
            clauses[c].watchedSet.erase(w0);
            moveWatch(c);
            if(clauses[c].watchedSet.size() == 2)
            {
                interestingClauses.push_back(c);
                continue;
            }
            if(thisImplicationGraph.find(w1) == thisImplicationGraph.end())
                return c;
            interestingClauses.clear();
            thisImplicationGraph.insert(make_pair(-1, ImplicationGraphNode(true, d)));
            return c;
        }
    }
    return -2;
}

void CDCLSolver :: unitPropagate(int d)
{
    pair<int, bool> p;
    int clauseNum;
    while(true)
    {
        clauseNum = pickClause(d);
        if(clauseNum == -2)
        {
            return;
        }
        if(thisImplicationGraph.find(-1) != thisImplicationGraph.end())
        {
            for(auto it = clauses[clauseNum].variableSignMap.begin(); it != clauses[clauseNum].variableSignMap.end(); it++)
            {
                thisImplicationGraph[-1].parentsOfThisNode.insert(it->first);
            }
            return;
        }
        p = unitRule(clauses[clauseNum]);
        for(int i = 0; i < clauses.size(); i++)
            if(i != clauseNum && clauses[i].variableSignMap.find(p.first) != clauses[i].variableSignMap.end())
                interestingClauses.push_back(i);
        thisImplicationGraph.insert(make_pair(p.first, ImplicationGraphNode(p.second, d)));
        for(auto it = clauses[clauseNum].variableSignMap.begin(); it != clauses[clauseNum].variableSignMap.end(); it++)
        {
            if(it->first == p.first)
                continue;
            if(thisImplicationGraph.find(it->first) != thisImplicationGraph.end())
            {
                thisImplicationGraph[it->first].childrenOfThisNode.insert(p.first);
                thisImplicationGraph[p.first].parentsOfThisNode.insert(it->first);
            }
        }
    }
    return;
}

int CDCLSolver :: analyzeLearn(int d)
{
    queue<int> bfs, temp;
    int nodesAtHighestLevel = 0;
    int current;
    int secondHighestLevel = 0;
    vector<int> literals;
    for(auto it = thisImplicationGraph[-1].parentsOfThisNode.begin(); it != thisImplicationGraph[-1].parentsOfThisNode.end(); it++)
    {
        bfs.push(*it);
        if(thisImplicationGraph[*it].d == d)
            nodesAtHighestLevel++;
    }
    while(nodesAtHighestLevel > 1)
    {
        current = bfs.front();
        bfs.pop();
        if(thisImplicationGraph[current].d == d)
            nodesAtHighestLevel--;
        for(auto it = thisImplicationGraph[current].parentsOfThisNode.begin(); it != thisImplicationGraph[current].parentsOfThisNode.end(); it++)
        {
            bfs.push(*it);
            if(thisImplicationGraph[*it].d == d)
                nodesAtHighestLevel++;
            else if(thisImplicationGraph[*it].d > secondHighestLevel)
                secondHighestLevel = thisImplicationGraph[*it].d;
        }
    }
    while(!bfs.empty())
    {
        current = bfs.front();
        bfs.pop();
        if(find(literals.begin(), literals.end(), current) != literals.end() || find(literals.begin(), literals.end(), -1 * current) != literals.end())
            continue;
        if(thisImplicationGraph[current].value)
            literals.push_back(-1 * current);
        else
            literals.push_back(current);
    }
	if (printLogs) {
		cout << "Added clause: ";
		for (int i = 0; i < literals.size(); i++)
			cout << literals[i] << " ";
		cout << endl;
	}
    clauses.push_back(Clause(literals));
    if(secondHighestLevel == 0)
    {
        if(d == 0)
            return -1;
        return 0;
    }
    return secondHighestLevel;
}


CDCLSolver :: ~CDCLSolver()
{
}
