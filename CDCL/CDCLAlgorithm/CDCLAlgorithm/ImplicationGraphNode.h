#ifndef IMPLICATIONGRAPHNODE_H
#define IMPLICATIONGRAPHNODE_H

#include<iostream>
#include<unordered_set>
using namespace std;

class ImplicationGraphNode
{
    public:
        bool value;
        int d;
        unordered_set<int> childrenOfThisNode;
        unordered_set<int> parentsOfThisNode;
        ImplicationGraphNode();
        ImplicationGraphNode(bool, int);
};

#endif
