#include "Graph.h"
#include <fstream>
#include <iostream>

Graph::Graph(string inputFileName)
{
    ifstream inputFile;
    inputFile.open(inputFileName);

    if (!inputFile)
    {
        cout << "Error: Dictionary file could not be opened!" << endl;
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

    //cout << nodeCount << endl;
    int** weights = new int*[nodeCount];
    int weight;
    for (int i = 0; i < nodeCount; i++) // process input matrix rows
    {
        int* vert_row = new int[nodeCount];
        for (int j = 0; j < nodeCount; j++) // process input matrix columns
        {
            inputFile >> weight;
            //cout << weight << " ";
            vert_row[j] = weight;
        }
        weights[i] = vert_row; // make this row point to all values in th row
        //cout << endl;
    }

    this->weights = weights;
    this->verts = verts;
    this->vertCount = nodeCount;

    setNode** sets = new setNode*[nodeCount];
    this->sets = sets;

    int maxNumEdges = (nodeCount * nodeCount - nodeCount) / 2;
    edge** edges = new edge*[maxNumEdges];

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

            edge* newEdge = new edge();
            newEdge->u = uVert;
            newEdge->v = vVert;
            newEdge->weight = weight;

            edges[edgeCount++] = newEdge;

            
        }

    }

    this->edges = edges;
    this->edgeCount = edgeCount;
    
    // TODO: consider using ints everywhere for referencing verts until printing the result at the end
    // TODO: redo sets with an array of linked lists
}

void Graph::getMSTKruskal()
{
    edge** result = new edge*[edgeCount];

    for (int i = 0; i < vertCount; i++)
    {
        string vert = verts[i];
        makeSet(vert);
    }
    
    
    Graph::MinPriorityQueue* queue = new Graph::MinPriorityQueue(14);
    for (int i = 0; i < edgeCount; i++)
    {
        edge* currentEdge = edges[i];
        //cout << "INSERT EDGE " << i << " = " << currentEdge->u << "-" << currentEdge->v << " " << currentEdge->weight << endl;
        queue->insert(currentEdge);
    }
    
    //queue->debugDisplay();
    
    int numEdgesFound = 0;
    int totalWeight = 0;
    for (int i = 0; i < edgeCount; i++)
    {
        edge* currentEdge = queue->extractMin();
        //cout << "Extracted " << currentEdge->u << " " << currentEdge->v << " " << currentEdge->weight << endl;

        if (findSet(currentEdge->u) != findSet(currentEdge->v))
        {
            result[numEdgesFound++] = currentEdge;
            unionSet(currentEdge->u, currentEdge->v);
            totalWeight += getEdgeWeight(currentEdge->u, currentEdge->v);
        }
    }

    cout << totalWeight << endl;

    for (int j = 1; j < numEdgesFound; j++)
    {
        edge* key = result[j];
        int i = j - 1;
        while (i >= 0 && result[i]->v > key->v)
        {
            result[i + 1] = result[i];
            i = i - 1;
        }
        result[i + 1] = key;
    }

    for (int j = 1; j < numEdgesFound; j++)
    {
        edge* key = result[j];
        int i = j - 1;
        while (i >= 0 && result[i]->u > key->u)
        {
            result[i + 1] = result[i];
            i = i - 1;
        }
        result[i + 1] = key;
    }

    for (int i = 0; i < numEdgesFound; i++)
    {
        edge* currentEdge = result[i];
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

int Graph::getEdgeWeight(string u, string v)
{
    return weights[getVertIndex(u)][getVertIndex(v)];
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
            //cout << P->vert << " = " << P->vert << "->Next =" << P->next << endl;
            P = P->next;
        }
        cout << endl;
    }
}

Graph::MinPriorityQueue::MinPriorityQueue(int size)
{
    A = new edge*[size + 1];
    heapSize = 0;
    edge* emptyEdge = new edge();
    emptyEdge->u = " ";
    emptyEdge->v = " ";
    emptyEdge->weight = -1;
    A[0] = emptyEdge;
}

void Graph::MinPriorityQueue::insert(Graph::edge* e)
{
    heapSize++;
    decreaseKey(heapSize, e);
}

void Graph::MinPriorityQueue::decreaseKey(int indx, Graph::edge* key)
{
    int i = indx;
    A[indx] = key;
    //if (key->weight < A[indx]->weight)
    //    return;
    while ((i > 1) && (A[parent(i)]->weight > A[i]->weight))
    {
        // Swap A[i] with A[parent]
        edge* tmp = A[i];
        A[i] = A[parent(i)];
        A[parent(i)] = tmp;

        i = parent(i);
    }
}

Graph::edge* Graph::MinPriorityQueue::extractMin()
{
    //if (heapSize < 1)
    //    return new edge();
    edge* min = A[1];
    A[1] = A[heapSize];
    heapSize--;
    minHeapify(1);
    return min;
}

void Graph::MinPriorityQueue::debugDisplay()
{
    for (int i = 0; i <= heapSize; i++)
    {
        cout << "A[" << i << "]= " << A[i]->weight << endl;
    }
}

void Graph::MinPriorityQueue::minHeapify(int indx)
{
    int l = leftChild(indx);
    int r = rightChild(indx);

    int smallest;
    if ((l <= heapSize) && (A[l]->weight < A[indx]->weight))
        smallest = l;
    else
        smallest = indx;
    if ((r <= heapSize) && (A[r]->weight < A[smallest]->weight))
        smallest = r;
    if (smallest != indx)
    { 
        // Swap A[i] with A[smallest]
        edge* tmp = A[indx];
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
