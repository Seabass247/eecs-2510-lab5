#pragma once
#include <string>
using namespace std;
class Graph
{
public:
	Graph(string inputFileName);
	void displayMSTKruskal();



private:


	struct setNode
	{
		string vert;
		setNode* next;
	};

	struct edge
	{
		string u;
		string v;
		int weight;
	};

	class MinPriorityQueue
	{
	public:
		MinPriorityQueue(int size);
		void insert(Graph::edge* e);
		Graph::edge* extractMin();
		void debugDisplay();
	private:
		int heapSize;
		Graph::edge** A;

		void minHeapify(int indx);
		void decreaseKey(int i, Graph::edge* key);
		int leftChild(int indx);
		int rightChild(int indx);
		int parent(int indx);
	};

	int vertCount;
	int edgeCount;
	string* verts;
	int** weights;
	setNode** sets;
	edge** edges;

	int getVertIndex(string vertName);
	int getEdgeWeight(string u, string v);
	
	void MSTKruskal(edge** edges, int& totalWeight, int& totalEdges);
	void makeSet(string vert);
	int findSet(string vert);
	void unionSet(string u, string v);
	void debugSets();

};

