// Main.cpp
// By Sebastian Hamel
// For EECS 2510, Spring 2020
// Written 05/02/2020
// Contains the main function which is the entry point to this program.  Parses arguments passed to the program,
// and if the arguments are in a valid format, finds the minimum spanning tree (MST) of the graph from the file.

#include <iostream>
#include "Graph.h"

using namespace std;

// The entry point to SPAN.exe.  Handles the arguments from the command line such that
// the first (and ONLY argument) is a string containing the file name which has graph building information
// such that its Minimum Spanning Tree can be found and printed to console in a nicely-formatted manner.
int main(int argc, char* argv[])
{
    if (argc != 2) // If the argument count is not correct...
    { // ... then disaply an error message and exit.
        cout << "Error: Only 1 argument (input file name) should be passed to the program! (" << (argc - 1) << " were supplied)";
        return 0; // exit the program
    }
    
    // Otherwise, the argument is assumed a valid file containing a correctly formatted graph represent by
    // vertex names and a weight matrix.
    // Create a new graph from the file, specified by the argument passed to this program, ...
    string inputFilename = (string)argv[1];
    Graph* graph = new Graph(inputFilename);
    // ... and display the MST from Kruskal...
    graph->displayMSTKruskal();
    // ... followed by the MST from Prim after this visual divider
    cout << "-------------------------------------------" << endl;
    graph->displayMSTPrim();
}

