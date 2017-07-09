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

int num_workers;
int num_gangs;
int nodes; //number of nodes
int FWDistanceMatrix[2000][2000]; // Distance matrix for Floyd-Warshall
int FWPathMatrix[2000][2000]; // Path Matrix for Floyd-Warshall

#define FW_MAX 5000
int BLOCK_SIZE;


bool loadMatrix(const char * matrixPath);
void TESTPrintMatrix();
void FWShortestPathFrom(int from);

void setMatrixSize();
void FWGetPath(int from, int to); // print shortest path


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


void TESTPrintMatrix() {
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            cout << FWDistanceMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

void FWShortestPathFrom(int from) {
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

void FWGetPath(int from, int to) {
    if ( from == to)
        cout << to;
    else if (FWPathMatrix[from][to] == -1)
        cout << "Path does not exist";
    else {
        FWGetPath(from, FWPathMatrix[from][to]);
        cout << " -> " << to ;
    }
}

void FloydWarshall() {
    int n = nodes;
    int s = BLOCK_SIZE;
    int pocetDlazdic = (n + s - 1) / s;

#pragma acc parallel copy(FWDistanceMatrix) num_gangs(512) num_workers(16) vector_length(16)
    {
        for (int b = 0; b < pocetDlazdic; b++)
        {
	#pragma acc loop  gang
            for (int k = b * s; k < min((b + 1)*s, n); k++)
	#pragma acc loop worker
                for (int i = b * s; i < min((b + 1)*s, n); i++)
		#pragma acc loop vector
                    for (int j = b * s; j < min((b + 1)*s, n); j++) {
                        FWDistanceMatrix[i][j] = min( FWDistanceMatrix[i][k] + FWDistanceMatrix[k][j] ,  FWDistanceMatrix[i][j] );
                    }
            
	    
            for (int ib = 0; ib < pocetDlazdic; ib++) {
            if (ib == b) continue;
            #pragma acc loop gang
            for (int k = b * s; k < min((b + 1)*s, n); k++)
            #pragma acc loop worker 
            for (int i = b * s; i < min((b + 1)*s, n); i++)
            #pragma acc loop vector
               for (int j = ib * s; j < min((ib + 1)*s, n); j++)
                    FWDistanceMatrix[i][j] = min( FWDistanceMatrix[i][k] + FWDistanceMatrix[k][j] ,  FWDistanceMatrix[i][j] );
            }

            for (int jb = 0; jb < pocetDlazdic; jb++) {
            if (jb == b) continue;
            #pragma acc loop gang
            for (int k = b * s; k < min((b + 1)*s, n); k++ )
            #pragma acc loop worker 
            for (int i = jb * s; i < min((jb + 1) * s, n); i++ )
            #pragma acc loop vector 
                for (int j = b * s; j < min((b + 1)*s, n); j++)
                    FWDistanceMatrix[i][j] = min( FWDistanceMatrix[i][k] + FWDistanceMatrix[k][j] ,  FWDistanceMatrix[i][j] );
            }

            for (int ib = 0; ib < pocetDlazdic; ib++) {
            if (ib == b) continue;
            #pragma acc loop gang
            for (int jb = 0; jb < pocetDlazdic; jb++) {
            if (jb == b) continue;
            #pragma acc loop worker 
            for ( int k = b * s; k < min((b + 1)*s, n); k++)
            #pragma acc loop vector 
                for ( int i = jb * s; i < min( (jb + 1)*s, n ); i++)
                    for (int j = ib * s; j < min( (ib + 1)*s, n ); j++ )
                        FWDistanceMatrix[i][j] = min( FWDistanceMatrix[i][k] + FWDistanceMatrix[k][j] ,  FWDistanceMatrix[i][j] );
            }
            }

        }
    }



}

bool loadMatrix(const char * matrixPath) {
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

    //initialize predecessors
    for ( int i = 0; i < nodes; i++)
        for ( int j = 0; j < nodes; j++) {
            /*  if (FWDistanceMatrix[i][j] == FW_MAX)
                  FWPathMatrix[i][j] = -1;
              else
                  FWPathMatrix[i][j] = i;*/
            FWPathMatrix[i][j] = ( FWDistanceMatrix[i][j] == FW_MAX ? -1 : i);
        }


    return true;
}

void setMatrixSize() {
    // (int)acc_create(*FWDistanceMatrix, sizeof(int)*nodes);
    /*
        FWDistanceMatrix = new int*[nodes];
        FWPathMatrix = new int*[nodes];
        for (int i = 0; i < nodes; i++) {
            FWPathMatrix[i] = new int[nodes];
            FWDistanceMatrix[i] = new int[nodes];
        }*/
}



int main( int argc, const char* argv[] )
{

    if ( argc != 5 ) {
        cout << "Bad Input.. 1st parameter: Path to file. 2rd = block size" << endl;
        cout << "3th = num worker. 4th = num worker" << endl;
        return 1;
    }


    loadMatrix(argv[1]);
    BLOCK_SIZE = atoi(argv[2]);
    num_gangs = atoi(argv[3]);
    num_workers = atoi(argv[4]);

    //start of measuring
    double wall0 = get_wall_time();
    double cpu0  = get_cpu_time();

    FloydWarshall();

    //end of measuring
    double wall1 = get_wall_time();
    double cpu1  = get_cpu_time();

    //TESTPrintMatrix();

    //prints results
/*    for (int i = 0; i < nodes; i++) {
         FWShortestPathFrom(i);
     }*/
    cout << "Wall Time = " << wall1 - wall0 << endl;
    cout << "CPU Time  = " << cpu1  - cpu0  << endl;
    return 0;
}
