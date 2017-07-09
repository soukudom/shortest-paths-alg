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

#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"


using namespace std;

#define FW_MAX      50000
#define BLOCK_SIZE  16


int nodes; //number of nodes
int* matrix; //Input matrix
int* FWDistanceMatrix; // Distance matrix for Floyd-Warshall
int* FWPathMatrix; // Path Matrix for Floyd-Warshall




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
            cout << FWDistanceMatrix[i * nodes + j] << " ";
        }
        cout << endl;
    }
}

__global__ void GPU_FloydWarshall(int i, int * deviceDistanceMatrix, int * devicePathMatrix, int nodes) {
    int idy = blockDim.y * blockIdx.y + threadIdx.y;
    int idx = blockDim.x * blockIdx.x + threadIdx.x;

    if (idy < nodes && idx < nodes)
    {
        int actuatPosition = idy * nodes + idx;
        int newDistance = deviceDistanceMatrix[idy * nodes + i] + deviceDistanceMatrix[nodes * i + idx];
        int oldDistance = deviceDistanceMatrix[actuatPosition];
        if (newDistance < oldDistance)
        {
            deviceDistanceMatrix[actuatPosition] = newDistance;
            devicePathMatrix[actuatPosition] = devicePathMatrix[i * nodes + idx];
        }
    }
}

void FloydWarshall() {
    int *deviceDistanceMatrix;
    int *devicePathMatrix;

    cudaError_t err;
    err = cudaSetDevice(0);
    if (err != cudaSuccess) cout << "CHYBA!" << endl;

    err = cudaMalloc((int**)&deviceDistanceMatrix, nodes * nodes * sizeof(int));
    if (err != cudaSuccess) cout << "chyba" << endl;
    err = cudaMalloc((int**)&devicePathMatrix, nodes * nodes * sizeof(int));
    if (err != cudaSuccess) cout << "chyba" << endl;
    err = cudaMemcpy(deviceDistanceMatrix, FWDistanceMatrix, nodes * nodes * sizeof(int), cudaMemcpyHostToDevice);
    if (err != cudaSuccess) cout << "chyba" << endl;
    err = cudaMemcpy(devicePathMatrix, FWPathMatrix, nodes * nodes * sizeof(int), cudaMemcpyHostToDevice);
    if (err != cudaSuccess) cout << "chyba" << endl;

    dim3 dimGrid((nodes - 1) / BLOCK_SIZE + 1, (nodes - 1) / BLOCK_SIZE + 1, 1);
    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE, 1);

    for ( int i = 0; i < nodes; i++)
    {
        GPU_FloydWarshall <<< dimGrid, dimBlock>>>(i, deviceDistanceMatrix, devicePathMatrix, nodes);
        err = cudaThreadSynchronize();
        if (err != cudaSuccess) cout << "Error" << endl;
    }

    cudaMemcpy(FWDistanceMatrix, deviceDistanceMatrix, nodes * nodes * sizeof(int), cudaMemcpyDeviceToHost);
    cudaMemcpy(FWPathMatrix, devicePathMatrix, nodes * nodes * sizeof(int), cudaMemcpyDeviceToHost);
    cudaFree(devicePathMatrix);
    cudaFree(deviceDistanceMatrix);


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

    FWDistanceMatrix = new int [nodes * nodes];
    FWPathMatrix = new int [nodes * nodes];

    for (int j = 0; j < nodes; j++)
    {
        getline(inFile, line);
        istringstream is(line);
        for (int i = 0; i < nodes; i++)
        {
            is >> FWDistanceMatrix[j * nodes + i];
            if (i != j && FWDistanceMatrix[j * nodes + i] == 0)
                FWDistanceMatrix[j * nodes + i] = FW_MAX;
        }
    }

    //initialize predecessors
    for ( int i = 0; i < nodes; i++)
        for ( int j = 0; j < nodes; j++)
            FWPathMatrix[i * nodes + j] = ( FWDistanceMatrix[i * nodes + j] == FW_MAX ? -1 : i);


    return true;
}



int main( int argc, const char* argv[] )
{

    if ( argc != 2 ) {
        cout << "Bad Input.. 1st parameter: Path to file." << endl;
        return 1;
    }


    loadMatrix(argv[1]);


    //start of measuring
    double wall0 = get_wall_time();
    double cpu0  = get_cpu_time();

    // Initialize CUDA Event
    cudaEvent_t start, stop;
    float elapsedTime;

    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);

    FloydWarshall();

    // Finish recording
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);

    // Calculate elasped time
    cudaEventElapsedTime(&elapsedTime, start, stop);
    elapsedTime /= 1000;
    printf ("Timee : %f s\n", elapsedTime);

    //end of measuring
    double wall1 = get_wall_time();
    double cpu1  = get_cpu_time();

    //TESTPrintMatrix();

    //prints results
    /* for (int i = 0; i < ncg.getNodes(); i++) {
         ncg.FWShortestPathFrom(i);
     }*/
    cout << "Wall Time = " << wall1 - wall0 << endl;
    cout << "CPU Time  = " << cpu1  - cpu0  << endl;
    return 0;
}
