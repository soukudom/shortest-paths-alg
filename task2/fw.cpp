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

int num_threads;

#define FW_MAX 5000

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
    int** __restrict__ matrix; //Input matrix
    int** __restrict__ FWDistanceMatrix; // Distance matrix for Floyd-Warshall
    int** __restrict__ FWPathMatrix; // Path Matrix for Floyd-Warshall
};

//measuring function
double get_wall_time() {
    struct timeval time;
    if (gettimeofday(&time, NULL)) {
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
double get_cpu_time() {
    return (double)clock() / CLOCKS_PER_SEC;
}

NCG::~NCG() {
    for (int i = 0; i < nodes; i++) {
        delete[] FWDistanceMatrix[i];
        delete[] FWPathMatrix[i];
    }
    delete[] FWDistanceMatrix;
    delete[] FWPathMatrix;
}

int NCG::getNodes() {
    return nodes;

}

void NCG::TESTPrintMatrix() {
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            cout << FWDistanceMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

void NCG::FWShortestPathFrom(int from) {
    for (int i = 0; i < nodes; i++) {
        if (FWDistanceMatrix[from][i] == FW_MAX && from != i) {
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
    const  int n = nodes;
    for ( int i = 0; i < n; i++)
        for ( int j = 0; j < n; j++) {
            /*  if (FWDistanceMatrix[i][j] == FW_MAX)
                  FWPathMatrix[i][j] = -1;
              else
                  FWPathMatrix[i][j] = i;*/
            FWPathMatrix[i][j] = ( FWDistanceMatrix[i][j] == FW_MAX ? -1 : i);
        }

    #pragma omp parallel num_threads(num_threads)
    for ( int k = 0; k < n; ++k)
        #pragma omp for schedule (static)
        for ( int i = 0; i < n; ++i) {
            for ( int j = 0; j < n; ++j) {
                // if zrusen, pocitam se spojitym grafem
                //if (FWDistanceMatrix[i][k] ==  INT_MAX || FWDistanceMatrix[k][j] == INT_MAX)
                //    continue;

                /* 6.9s
                sum = FWDistanceMatrix[i][k] + FWDistanceMatrix[k][j];
                if (FWDistanceMatrix[i][j] >  sum) {
                    FWDistanceMatrix[i][j] = sum;
                    FWPathMatrix[i][j] = FWPathMatrix[k][j];
                }*/


                //6.7s
                /*if (FWDistanceMatrix[i][j] >  FWDistanceMatrix[i][k] + FWDistanceMatrix[k][j]) {
                    FWDistanceMatrix[i][j] = FWDistanceMatrix[i][k] + FWDistanceMatrix[k][j];
                    FWPathMatrix[i][j] = FWPathMatrix[k][j];
                }*/
                
              //  FWDistanceMatrix[i][j] = min( FWDistanceMatrix[i][k] + FWDistanceMatrix[k][j] ,  FWDistanceMatrix[i][j] );


              FWDistanceMatrix[i][j] = ( FWDistanceMatrix[i][j] >  FWDistanceMatrix[i][k] + FWDistanceMatrix[k][j] ? FWDistanceMatrix[i][k] + FWDistanceMatrix[k][j] : FWDistanceMatrix[i][j]);


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
            is >> FWDistanceMatrix[j][i];
            if (i != j && FWDistanceMatrix[j][i] == 0)
                FWDistanceMatrix[j][i] = FW_MAX;
        }
    }

    return true;
}

void NCG::setMatrixSize() {
    FWDistanceMatrix = new int*[nodes];
    FWPathMatrix = new int*[nodes];
    for (int i = 0; i < nodes; i++) {
        FWPathMatrix[i] = new int[nodes];
        FWDistanceMatrix[i] = new int[nodes];
    }
}



int main( int argc, const char* argv[] )
{
    NCG ncg;

    if ( argc != 3 ) {
        cout << "Bad Input.. 1st parameter: Path to file" << endl;
        return 1;
    }
    ncg.loadMatrix(argv[1]);
    num_threads = atoi(argv[2]);

    //start of measuring
    double wall0 = get_wall_time();
    double cpu0  = get_cpu_time();

    ncg.FloydWarshall();

    //end of measuring
    double wall1 = get_wall_time();
    double cpu1  = get_cpu_time();

//   ncg.TESTPrintMatrix();

    //prints results
    /* for (int i = 0; i < ncg.getNodes(); i++) {
         ncg.FWShortestPathFrom(i);
     }*/
    cout << "Wall Time = " << wall1 - wall0 << endl;
    cout << "CPU Time  = " << cpu1  - cpu0  << endl;
    return 0;
}
