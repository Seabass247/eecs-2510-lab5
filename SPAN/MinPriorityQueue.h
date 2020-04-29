#pragma once
#include <string>
#include "Graph.h"

using namespace std;

class MinPriorityQueue
{
public:
	Graph::edge extractMin();
	void insert();
private:
	int heapSize;
	Graph::edge* array;
	Graph::edge minimum();
	Graph::edge decreaseKey();
};

