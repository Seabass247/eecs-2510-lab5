// Graph.h
// By Sebastian Hamel
// For EECS 2510, Spring 2020
// Written 05/02/2020
// A header file for the Graph class.  Specifies the interface of Graph, which the main function 
// interacts with to get and display the MST of a graph parsed from the file passed in Main.  
// Contains all of its method and member variable declarations.

#pragma once
#include <string>
using namespace std;
class Graph
{
public:
	Graph(string inputFileName);
	void displayMSTKruskal();
	void displayMSTPrim();

private:


	struct setNode
	{
		string vert;
		setNode* next;
	};

	struct graphNode
	{
		string u;
		string v;
		string name;
		string parent;
		double key;
	};

	class MinPriorityQueue
	{
	public:
		MinPriorityQueue(int size);
		void insert(Graph::graphNode* e);
		Graph::graphNode* extractMin();
		void debugDisplay();
		bool isEmpty();
		bool contains(graphNode* e);
	private:
		int heapSize;
		Graph::graphNode** A;

		void minHeapify(int indx);
		void decreaseKey(int i, Graph::graphNode* key);
		int leftChild(int indx);
		int rightChild(int indx);
		int parent(int indx);
	};

	int vertCount;
	int edgeCount;
	string* verts;
	double** weights;
	setNode** sets;
	graphNode** edges;

	int getVertIndex(string vertName);
	double getEdgeWeight(string u, string v);
	string* get_adjacent(string v, int& count);
	void MSTKruskal(graphNode** edges, double& totalWeight, int& totalEdges);
	void MSTPrim(graphNode** vertices, double& totalWeight, int& totalEdges);
	void makeSet(string vert);
	int findSet(string vert);
	void unionSet(string u, string v);
	void debugSets();

};

