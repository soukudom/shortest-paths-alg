#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <limits.h>

using namespace std;

class NCG {
public:
    bool loadMatrix(const char * matrixPath);
    void TESTPrintMatrix();
    void Dijkstra(int start);
    void FloydWarshall(); // Calculate and fill FWDistanceMatrix FWPathMatrix
    void FWShortestPathFrom(int from);
private:
    void setMatrixSize();
    bool FWGetPath(int from, int to); // print shortest path

    int nodes; //number of nodes
    int** matrix; //Input matrix
    int** FWDistanceMatrix; // Distance matrix for Floyd-Warshall
    int** FWPathMatrix; // Path Matrix for Floyd-Warshall
};

void NCG::TESTPrintMatrix() {
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

//Node structure for priority queue
struct node
{
    //position of node
    int coordinate;
    //distance from begin node
    int distance;
    bool operator<(const node& n) const
    {
        return distance > n.distance;
    }
};

void NCG::FWShortestPathFrom(int from) {
    for (int i = 0; i < nodes; i++) {
        if (FWDistanceMatrix[from][i] == INT_MAX && from != i) {
            cout << "From: " << from << ", To: " << i << ", Path does not exist" << endl;
            continue;
        }
        cout << "From: " << from << ", To: " << i << ", Distance: " << FWDistanceMatrix[from][i] << ", Path: ";
        FWGetPath(from, i);
        cout << endl;
    }
}

bool NCG::FWGetPath(int from, int to) {
    if ( from == to) {
        cout << to;
        return true;
    }
    else if (FWPathMatrix[from][to] == -1) {
        cout << "Path does not exist";
        return false;
    }
    else {
        if ( FWGetPath(from, FWPathMatrix[from][to]) == false)
            return false;
        cout << " -> " << to ;
    }
    return true;
}

void NCG::FloydWarshall() {
    //initialize predecessors
    for (int i = 0; i < nodes; i++)
        for (int j = 0; j < nodes; j++) {
            if (FWDistanceMatrix[i][j] != 0 && FWDistanceMatrix[i][j] != INT_MAX)
                FWPathMatrix[i][j] = i;
            else
                FWPathMatrix[i][j] = -1;
        }


    for (int k = 0; k < nodes; k++)
        for (int i = 0; i < nodes; i++)
            for (int j = 0; j < nodes; j++) {
                if (FWDistanceMatrix[i][k] ==  INT_MAX || FWDistanceMatrix[k][j] == INT_MAX)
                    continue;
                if (FWDistanceMatrix[i][j] > FWDistanceMatrix[i][k] + FWDistanceMatrix[k][j]) {

                    FWDistanceMatrix[i][j] = FWDistanceMatrix[i][k] + FWDistanceMatrix[k][j];
                    FWPathMatrix[i][j] = FWPathMatrix[k][j];
                }
            }

}

//start: begin node
void NCG::Dijkstra(int start) {
    priority_queue<node> q;
    bool *closed = new bool[nodes];
    int *predecessors = new int[nodes];
    int *distances = new int[nodes];
    node node_add;
    node node_min;

    //initialization
    for (int i = 0; i < nodes; i++) {
        if (i == start) {
            //set attributes for priority queue
            node_add.coordinate = i;
            node_add.distance = 0;
            q.push(node_add);
            //set attributes
            closed[i] = true;
            distances[i] = 0;
            predecessors[i] = -1;
        }
        else {
            //set default attributes for other nodes
            distances[i] = INT_MAX;
            closed[i] = false;
            predecessors[i] = -1;
        }
    }

    while (!q.empty()) {
        //take out the most valuable node
        node_min = q.top();
        q.pop();
        closed[node_min.coordinate] = true;

        for (int i = 0; i < nodes; i++) {
            //if edge exists
            if (matrix[node_min.coordinate][i] != 0) {
                //if node is open
                if (!closed[i]) {
                    //if the new distance is better
                    if (node_min.distance + matrix[node_min.coordinate][i] < distances[i]) {
                        //modify the distance and insert new nodes to the priority queue
                        distances[i] = node_min.distance + matrix[node_min.coordinate][i];
                        predecessors[i] = node_min.coordinate;
                        node_add.coordinate = i;
                        node_add.distance = distances[i];
                        q.push(node_add);
                    }
                }
            }
        }
    }

    //Summarization
    cout << "The shortest paths are: " << endl;
    int j = 0;
    for (int i = 0; i < nodes; i++) {
        cout << "From: " << start << ", To: " << i << ", Distance: " << distances[i] << ", Path: ";
        j = i;
        //Fist node check
        if (j != start) {
            cout << j << " <- ";
        }
        else {
            cout << j;
        }
        //node path print
        while (predecessors[j] != -1) {
            //End predecessor check
            if (predecessors[j] != start) {
                cout << predecessors[j] << " <- ";
            }
            else {
                cout << predecessors[j];
            }
            j = predecessors[j];

        }
        cout << endl;
    }
    //Deallocation
    delete [] distances;
    delete [] closed;
    delete [] predecessors;
}

bool NCG::loadMatrix(const char * matrixPath) {
    string line;

    ifstream inFile (matrixPath);
    if (!inFile.is_open()) {
        cout << "Wrong path to file" << endl;
        return false;
    }

    getline(inFile, line);
    nodes = atoi(line.c_str());
    setMatrixSize();

    for (int j = 0; j < nodes; j++) {
        getline(inFile, line);
        istringstream is(line);
        for (int i = 0; i < nodes; i++) {
            is >> matrix[j][i];
            if (i != j && matrix[j][i] == 0)
                FWDistanceMatrix[j][i] = INT_MAX;
            else
                FWDistanceMatrix[j][i] = matrix[j][i];
        }
    }

    return true;
}

void NCG::setMatrixSize() {
    matrix = new int*[nodes];
    FWDistanceMatrix = new int*[nodes];
    FWPathMatrix = new int*[nodes];
    for (int i = 0; i < nodes; i++) {
        matrix[i] = new int[nodes];
        FWPathMatrix[i] = new int[nodes];
        FWDistanceMatrix[i] = new int[nodes];
    }
}



int main( int argc, const char* argv[] )
{
    NCG ncg;

    if ( argc != 2 ) {
        cout << "Bad Input.. 1st parameter: Path to file" << endl;
        return 1;
    }
    ncg.loadMatrix(argv[1]);
    ncg.Dijkstra(2);
    ncg.FloydWarshall();
    cout << "----------" << endl;
    ncg.FWShortestPathFrom(2);
    //ncg.TESTPrintMatrix();
    return 0;
}
