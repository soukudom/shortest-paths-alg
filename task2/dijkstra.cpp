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
#include <omp.h>

using namespace std;

    bool loadMatrix(const char * matrixPath); //loads matrix from a file
    void printMatrix(); //prints loaded matrix
    void dijkstra(int start); //finds the shortest paths from start node
    void printDijkstra(int start); //prints finded results
    void allocateMatrix(); //allocates structures on the heap
    void deleteMemory(); //descructor
    void setMatrixSize(); //allocated structures for input graph
    void getPath(int from, int to); //prints the shortest path
    void dijkstraAll();

    int nodes; //number of nodes
    int**  matrix; //input matrix
    int ** predecessorsMatrix; //predecessors matrix
    int ** distancesMatrix; //distance matrix
    int threads; // number of threads

//measuring function
double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }   
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}



void deleteMemory() {
    
    for (int i = 0; i < nodes; i++){
        
        delete[] matrix[i];
        delete[] predecessorsMatrix[i];
        delete[] distancesMatrix[i];
    }
    delete[] matrix;
    delete[] predecessorsMatrix;
    delete[] distancesMatrix;
  //  delete[] closed;
}

void allocateMatrix(){
    predecessorsMatrix = new int*[nodes];
    distancesMatrix = new int*[nodes];
    //closed = new int[nodes];
    for(int i = 0; i < nodes; i++){
        predecessorsMatrix[i] = new int[nodes];       
        distancesMatrix[i] = new int[nodes];
    }
}

void printMatrix() {
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

void getPath(int from, int to) {
    if ( from == to) 
        cout << to; 
    else {
        getPath(from, predecessorsMatrix[from][to]);
        cout << " -> " << to ;
    }   
}
    
void printDijkstra(int start){
    for (int i = 0; i < nodes; i++) {
        if (distancesMatrix[start][i] == INT_MAX){
            cout << "From: " << start << ", To: " << i << ", Path does not exist" << endl;
            continue;
        }
        else{
             cout << "From: " << start << ", To: " << i << ", Distance: " << distancesMatrix[start][i] << ", Path: ";
            getPath(start,i);
            cout << endl;
        }
    }

}

//start: begin node
void dijkstra(int start) {
    priority_queue<node> q; //priority queue
    node node_add; //help structure for adding a node
    node node_min; //help structure for choosing a minimal node
    int closed [nodes];
        for(int j = 0; j < nodes; j++){
            closed[j]=0;
        }

    //set attributes for priority queue
    node_add.coordinate = start;
    node_add.distance = 0;
    q.push(node_add);
    //set attributes
    closed[start] = 1;
    distancesMatrix[start][start] = 0;
    predecessorsMatrix[start][start] = -1;

    while (!q.empty()) {
        //take out the most valuable node
        node_min = q.top();
        q.pop();
        closed[node_min.coordinate] = 1;

        for (int i = 0; i < nodes; i++) {
            //if edge exists
            if (matrix[node_min.coordinate][i] != 0) {
                //if node is open
                if (!closed[i]) {
                    //if the new distance is better
                    if (node_min.distance + matrix[node_min.coordinate][i] < distancesMatrix[start][i]) {
                        //modify the distance and insert new nodes to the priority queue
                        distancesMatrix[start][i] = node_min.distance + matrix[node_min.coordinate][i];
                        predecessorsMatrix[start][i] = node_min.coordinate;
                        node_add.coordinate = i;
                        node_add.distance = distancesMatrix[start][i];
                        q.push(node_add);
                    }
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
    //gets number of nodes
    nodes = atoi(line.c_str());
    setMatrixSize();

    for (int j = 0; j < nodes; j++) {
        getline(inFile, line);
        istringstream is(line);
        for (int i = 0; i < nodes; i++) {
            is >> matrix[j][i];
        }
    }

    return true;
}

void setMatrixSize() {
    matrix = new int*[nodes];
    for (int i = 0; i < nodes; i++) {
        matrix[i] = new int[nodes];
    }
}

void dijkstraAll(){
    //initialization
    int tmp = nodes;
    for (int j = 0; j < tmp; j++){
        for (int i = 0; i < tmp; i++) {
            //set default attributes for other nodes
            distancesMatrix[j][i] = INT_MAX;
            predecessorsMatrix[j][i] = -1;
        }
    }

    //run over every node
    #pragma omp parallel for num_threads(threads) schedule(static) shared(matrix,nodes,threads, predecessorsMatrix, distancesMatrix)
    for(int i = 0; i< tmp; i++){
        dijkstra(i);
    }
}

int main( int argc, const char* argv[] )
{

    if ( argc != 3 ) {
        cout << "Bad Input.. 1st parameter: Path to file, 2nd parameter: Number of threads" << endl;
        return 1;
    }
    loadMatrix(argv[1]);
    allocateMatrix();
    threads = atoi(argv[2]);
    //start of measuring 
    double wall0 = get_wall_time();
    double cpu0  = get_cpu_time();
    //double start = omp_get_wtime();

    dijkstraAll();

    //end of measuring
    double wall1 = get_wall_time();
    double cpu1  = get_cpu_time();
  //  double end = omp_get_wtime( );  
    

    //prints results
/*    for(int i = 0; i< nodes; i++){
        printDijkstra(i);
    }*/
    deleteMemory();
    cout << "Wall Time = " << wall1 - wall0 << endl;
    cout << "CPU Time  = " << cpu1  - cpu0  << endl;
//    cout << "OpenMP Time = " << end - start << endl;
    return 0;
}
