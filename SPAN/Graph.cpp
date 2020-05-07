// Graph.cpp
// By Sebastian Hamel
// For EECS 2510, Spring 2020
// Written 05/02/2020
// The implementation of "Graph" which supports reading in a specifically formatted file contains a graph represented by
// a weight matrix.  Graph parses the weights into distinct edges, which enables the Prim and Kruskal algorithms to
// perform operations on said edges to obtain the graph's Minimum Spanning Tree, MST.  The MST is displayed for both 
// Prim and Kruskal by printing each edge by its u-v vertices and respective weight.

#include "Graph.h"
#include <fstream>
#include <iostream>

Graph::Graph(string inputFileName)
{
    ifstream inputFile;
    inputFile.open(inputFileName);

    if (!inputFile)
    {
        cout << "Error: Input file could not be opened!" << endl;
        exit(0);
    }

    int nodeCount;
    inputFile >> nodeCount;
    string nodeName;
    string* verts = new string[nodeCount];
    for (int i = 0; i < nodeCount; i++) // read in node names
    {
        inputFile >> nodeName;
        verts[i] = nodeName;
    }

    double** weights = new double*[nodeCount];
    double weight;
    for (int i = 0; i < nodeCount; i++) // process input matrix rows
    {
        double* vert_row = new double[nodeCount];
        for (int j = 0; j < nodeCount; j++) // process input matrix columns
        {
            inputFile >> weight;
            vert_row[j] = weight;
        }
        weights[i] = vert_row; // make this row point to all values in i-th row
    }

    this->weights = weights;
    this->verts = verts;
    this->vertCount = nodeCount;

    setNode** sets = new setNode*[nodeCount];
    this->sets = sets;

    int maxNumEdges = (nodeCount * nodeCount - nodeCount) / 2;
    graphNode** edges = new graphNode*[maxNumEdges];

    // Parse the weight matrix into distinct edges that can be iterated over in operations for Prim and Kruskal
    int edgeCount = 0;
    for (int i = 0; i < this->vertCount; i++)
    {
        for (int j = 0; j < nodeCount; j++)
        {
            string uVert = verts[i];
            string vVert = verts[j];

            int weight = getEdgeWeight(uVert, vVert);
            if (uVert >= vVert || !weight)
                continue;        

            graphNode* newEdge = new graphNode();
            newEdge->u = uVert;
            newEdge->v = vVert;
            newEdge->key = weight;

            edges[edgeCount++] = newEdge;

        }

    }

    this->edges = edges;
    this->edgeCount = edgeCount;

}

void Graph::displayMSTKruskal()
{
    cout << "Kruskal" << endl;
    graphNode** result = new graphNode*[edgeCount];
    double totalWeight = 0;
    int numEdgesFound = 0;
    MSTKruskal(result, totalWeight, numEdgesFound);

    // Sort the edges by their second vertex name as the key (v)
    for (int j = 1; j < numEdgesFound; j++)
    {
        graphNode* key = result[j];
        int i = j - 1;
        while (i >= 0 && result[i]->v > key->v)
        {
            result[i + 1] = result[i];
            i = i - 1;
        }
        result[i + 1] = key;
    }

    // Sort the edges by their first vertex name as the key (u)
    for (int j = 1; j < numEdgesFound; j++)
    {
        graphNode* key = result[j];
        int i = j - 1;
        while (i >= 0 && result[i]->u > key->u)
        {
            result[i + 1] = result[i];
            i = i - 1;
        }
        result[i + 1] = key;
    }

    // Display the total weight followed by the sorted edges and their respective weights
    cout << totalWeight << endl;
    for (int i = 0; i < numEdgesFound; i++)
    {
        graphNode* currentEdge = result[i];
        cout << currentEdge->u << "-" << currentEdge->v << ": " << getEdgeWeight(currentEdge->u, currentEdge->v) << endl;
    }

}

void Graph::displayMSTPrim()
{
    graphNode** result = new graphNode * [vertCount];

    double totalWeight = 0;
    int numEdgesFound = 0;
    MSTPrim(result, totalWeight, numEdgesFound);
    
    // Sort the edges by their second vertex name as the key (v)
    for (int j = 1; j < numEdgesFound; j++)
    {
        graphNode* key = result[j];
        int i = j - 1;
        while (i >= 0 && result[i]->v > key->v)
        {
            result[i + 1] = result[i];
            i = i - 1;
        }
        result[i + 1] = key;
    }

    // Sort the edges by their first vertex name as the key (u)
    for (int j = 1; j < numEdgesFound; j++)
    {
        graphNode* key = result[j];
        int i = j - 1;
        while (i >= 0 && result[i]->u > key->u)
        {
            result[i + 1] = result[i];
            i = i - 1;
        }
        result[i + 1] = key;
    }

    // Display the total weight followed by the sorted edges and their respective weights
    cout << totalWeight << endl;
    for (int i = 0; i < numEdgesFound; i++)
    {
        graphNode* currentEdge = result[i];
        cout << currentEdge->u << "-" << currentEdge->v << ": " << getEdgeWeight(currentEdge->u, currentEdge->v) << endl;
    }
}

int Graph::getVertIndex(string vertName)
{
    for (int i = 0; i < vertCount; i++)
    {
        if (verts[i] == vertName)
            return i;
    }
    return -1;
}

double Graph::getEdgeWeight(string u, string v)
{
    return weights[getVertIndex(u)][getVertIndex(v)];
}

string* Graph::get_adjacent(string v, int& count)
{
    string* result = new string[vertCount];
    int column = getVertIndex(v);
    for (int i = 0; i < vertCount; i++)
    {
        if (weights[i][column] > 0)
        {
            result[count++] = verts[i];
        }
    }
    return result;
}

void Graph::MSTKruskal(graphNode** result, double& totalWeight, int& numEdgesFound)
{
    for (int i = 0; i < vertCount; i++)
        makeSet(verts[i]);
    
    Graph::MinPriorityQueue* queue = new Graph::MinPriorityQueue(edgeCount);
    for (int i = 0; i < edgeCount; i++)
    {
        graphNode* currentEdge = edges[i];
        queue->insert(currentEdge);
    }
    
    for (int i = 0; i < edgeCount; i++)
    {
        graphNode* currentEdge = queue->extractMin();

        if (findSet(currentEdge->u) != findSet(currentEdge->v))
        {
            result[numEdgesFound++] = currentEdge;
            unionSet(currentEdge->u, currentEdge->v);
            totalWeight += getEdgeWeight(currentEdge->u, currentEdge->v);
        }
    }

}

void Graph::MSTPrim(graphNode** edges, double& totalWeight, int& totalEdges)
{
    cout << "Prim" << endl;
    Graph::MinPriorityQueue* queue = new Graph::MinPriorityQueue(vertCount);
    graphNode** vertices = new graphNode * [vertCount];

    for (int i = 0; i < vertCount; i++)
    {
        graphNode* uVert = new graphNode();
        uVert->name = verts[i];
        uVert->parent = "";
        uVert->key = DBL_MAX;
        if (i == 0)
            uVert->key = 0;
        vertices[i] = uVert;
        queue->insert(uVert);
    }
    
    int vertsAdded = 0;
    while (!queue->isEmpty())
    {
        vertsAdded++;
        graphNode* u = queue->extractMin();
        
        int adjCount = 0;
        string* adj = get_adjacent(u->name, adjCount);
        
        for (int i = 0; i < adjCount; i++)
        {
            graphNode* v = vertices[getVertIndex(adj[i])];
            if (queue->contains(v) && getEdgeWeight(u->name, v->name) < v->key)
            {
                v->parent = u->name;
                v->key = getEdgeWeight(u->name, v->name);
            }
            
        }

        if (vertsAdded > 1)
        {
            graphNode* addEdge = new graphNode();
            string uName = getVertIndex(u->name) < getVertIndex(u->parent) ? u->name : u->parent;
            string vName = getVertIndex(u->name) < getVertIndex(u->parent) ? u->parent : u->name;
            addEdge->u = uName;
            addEdge->v = vName;
            totalWeight += getEdgeWeight(u->name, u->parent);
            edges[totalEdges++] = addEdge;
        }

    }

}

void Graph::makeSet(string vert)
{
    setNode* head = new setNode();
    head->vert = vert;
    head->next = nullptr;

    sets[getVertIndex(vert)] = head;
}

int Graph::findSet(string vert)
{
    for (int i = 0; i < vertCount; i++)
    {
        if (sets[i] == nullptr)
            continue;
        setNode* setHead = sets[i];
        setNode* P = setHead;
        while (P != nullptr)
        {
            if (P->vert == vert)
                return i;
            P = P->next;
        }
    }
    return -1;
}

void Graph::unionSet(string u, string v)
{
    int uSet = findSet(u);
    int vSet = findSet(v);
    setNode* uSetHead = sets[uSet];
    setNode* vSetHead = sets[vSet];
    setNode* P = uSetHead;
    setNode* uSetTail = nullptr;
    while (P != nullptr)
    {
        uSetTail = P;
        P = P->next;
    }

    uSetTail->next = vSetHead;
    sets[vSet] = nullptr;
}

void Graph::debugSets()
{
    for (int i = 0; i < vertCount; i++)
    {
        if (sets[i] == nullptr)
            continue;
        cout << "Set " << i << ": ";
        
        setNode* P = sets[i];
        while (P != nullptr)
        {
            cout << P->vert << " ";
            P = P->next;
        }
        cout << endl;
    }
}

Graph::MinPriorityQueue::MinPriorityQueue(int size)
{
    A = new graphNode*[size + 1];
    heapSize = 0;
    graphNode* emptyEdge = new graphNode();
    emptyEdge->u = " ";
    emptyEdge->v = " ";
    emptyEdge->key = -1;
    A[0] = emptyEdge;
}

bool Graph::MinPriorityQueue::isEmpty() {
    if (heapSize < 1)
        return true;
    else
        return false;
}

bool Graph::MinPriorityQueue::contains(graphNode* e)
{
    for (int i = 1; i <= heapSize; i++)
    {
        if (A[i] == e)
            return true;
    }
    return false;
}
void Graph::MinPriorityQueue::insert(Graph::graphNode* e)
{
    heapSize++;
    decreaseKey(heapSize, e);
}

void Graph::MinPriorityQueue::decreaseKey(int indx, Graph::graphNode* key)
{
    int i = indx;
    A[indx] = key;
    //if (key->weight < A[indx]->weight)
    //    return;
    while ((i > 1) && (A[parent(i)]->key > A[i]->key))
    {
        // Swap A[i] with A[parent]
        graphNode* tmp = A[i];
        A[i] = A[parent(i)];
        A[parent(i)] = tmp;

        i = parent(i);
    }
}

Graph::graphNode* Graph::MinPriorityQueue::extractMin()
{
    //if (heapSize < 1)
    //    return new edge();
    minHeapify(1);
    graphNode* min = A[1];
    A[1] = A[heapSize];
    heapSize--;
    minHeapify(1);
    return min;
}

void Graph::MinPriorityQueue::debugDisplay()
{
    for (int i = 0; i <= heapSize; i++)
    {
        cout << "A[" << i << "]= " << A[i]->name << " " <<  A[i]->key << endl;
    }
}

void Graph::MinPriorityQueue::minHeapify(int indx)
{
    int l = leftChild(indx);
    int r = rightChild(indx);

    int smallest;
    if ((l <= heapSize) && (A[l]->key < A[indx]->key))
        smallest = l;
    else
        smallest = indx;
    if ((r <= heapSize) && (A[r]->key < A[smallest]->key))
        smallest = r;
    if (smallest != indx)
    { 
        // Swap A[i] with A[smallest]
        graphNode* tmp = A[indx];
        A[indx] = A[smallest];
        A[smallest] = tmp;

        minHeapify(smallest);
    }
}

int Graph::MinPriorityQueue::leftChild(int indx)
{
    int leftIndx = 2 * indx;
    return leftIndx;
}

int Graph::MinPriorityQueue::rightChild(int indx)
{
    int rightIndx = 2 * indx + 1;
    return rightIndx;
}

int Graph::MinPriorityQueue::parent(int indx)
{
    int parentIndx = indx / 2;
    return parentIndx;
}
