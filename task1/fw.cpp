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
    void FloydWarshall(); // Calculate and fill FWDistanceMatrix FWPathMatrix
    void FWShortestPathFrom(int from);
    int getNodes();
    ~NCG();
private:
    void setMatrixSize();
    void FWGetPath(int from, int to); // print shortest path

    int nodes; //number of nodes
    int** matrix; //Input matrix
    int** FWDistanceMatrix; // Distance matrix for Floyd-Warshall
    int** FWPathMatrix; // Path Matrix for Floyd-Warshall
};

NCG::~NCG() {
    for (int i = 0; i < nodes; i++) {
        delete[] matrix[i];
        delete[] FWDistanceMatrix[i];
        delete[] FWPathMatrix[i];
    }
    delete[] matrix;
    delete[] FWDistanceMatrix;
    delete[] FWPathMatrix;
}

int NCG::getNodes(){
    return nodes;

}

void NCG::TESTPrintMatrix() {
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

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

void NCG::FWGetPath(int from, int to) {
    if ( from == to)
        cout << to;
    else if (FWPathMatrix[from][to] == -1)
        cout << "Path does not exist";
    else {
        FWGetPath(from, FWPathMatrix[from][to]);
        cout << " -> " << to ;
    }
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
    ncg.FloydWarshall();
    
    for(int i = 0; i < ncg.getNodes(); i++){    
        ncg.FWShortestPathFrom(i);
    }
    return 0;
}
