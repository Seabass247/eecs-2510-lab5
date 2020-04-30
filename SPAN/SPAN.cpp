#include <iostream>
#include "Graph.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 2) // If the argument count is not correct...
    { // ... then disaply an error message and exit.
        cout << "Error: Only 1 argument (input file name) should be passed to the program! (" << (argc - 1) << " were supplied)";
        return 0; // exit the program
    }

    
    string inputFilename = (string)argv[1];
    Graph* graph = new Graph(inputFilename);
    graph->displayMSTKruskal();

}

