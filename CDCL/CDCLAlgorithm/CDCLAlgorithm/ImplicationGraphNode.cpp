#include <iostream>
#include "ImplicationGraphNode.h"

using namespace std;

ImplicationGraphNode :: ImplicationGraphNode()
{
}

ImplicationGraphNode :: ImplicationGraphNode(bool val,int depth)
{
    value = val;
    d = depth;
}

