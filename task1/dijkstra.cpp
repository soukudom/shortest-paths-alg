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
    bool loadMatrix(const char * matrixPath); //loads matrix from a file
    void printMatrix(); //prints loaded matrix
    void dijkstra(int start); //finds the shortest paths from start node
    void printDijkstra(int start); //prints finded results
    void allocateMatrix(); //allocates structures on the heap
    int getNodes(); //returns number of nodes
    ~NCG(); //descructor
private:
    void setMatrixSize(); //allocated structures for input graph
    void getPath(int from, int to); //prints the shortest path

    int nodes; //number of nodes
    int** matrix; //input matrix
    int ** predecessorsMatrix; //predecessors matrix
    int ** distancesMatrix; //distance matrix
    bool * closed; //closed flags
};

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



NCG::~NCG() {
    for (int i = 0; i < 1500; i++){
        delete[] matrix[i];
        delete[] predecessorsMatrix[i];
        delete[] distancesMatrix[i];
    }
    delete[] matrix;
    delete[] predecessorsMatrix;
    delete[] distancesMatrix;
    delete[] closed;
}

int NCG::getNodes(){
    return nodes;
}

void NCG::allocateMatrix(){
    predecessorsMatrix = new int*[nodes];
    distancesMatrix = new int*[nodes];
    closed = new bool[nodes];
    for(int i = 0; i < nodes; i++){
        predecessorsMatrix[i] = new int[nodes];       
        distancesMatrix[i] = new int[nodes];
    }
}

void NCG::printMatrix() {
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

void NCG::getPath(int from, int to) {
    if ( from == to) 
        cout << to; 
    else {
        getPath(from, predecessorsMatrix[from][to]);
        cout << " -> " << to ;
    }   
}
    
void NCG::printDijkstra(int start){
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
void NCG::dijkstra(int start) {
    priority_queue<node> q; //priority queue
    node node_add; //help structure for adding a node
    node node_min; //help structure for choosing a minimal node

    //initialization
    for (int i = 0; i < nodes; i++) {
        if (i == start) {
            //set attributes for priority queue
            node_add.coordinate = i;
            node_add.distance = 0;
            q.push(node_add);
            //set attributes
            closed[i] = true;
            distancesMatrix[start][i] = 0;
            predecessorsMatrix[start][i] = -1;
        }
        else {
            //set default attributes for other nodes
            distancesMatrix[start][i] = INT_MAX;
            closed[i] = false;
            predecessorsMatrix[start][i] = -1;
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

bool NCG::loadMatrix(const char * matrixPath) {
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

void NCG::setMatrixSize() {
    matrix = new int*[nodes];
    for (int i = 0; i < nodes; i++) {
        matrix[i] = new int[nodes];
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
    ncg.allocateMatrix();

    //start of measuring 
    double wall0 = get_wall_time();
    double cpu0  = get_cpu_time();
    //run over every node
    for(int i = 0; i< ncg.getNodes(); i++){
        ncg.dijkstra(i);
    }

    //end of measuring
    double wall1 = get_wall_time();
    double cpu1  = get_cpu_time();

    //prints results
/*    for(int i = 0; i< ncg.getNodes(); i++){
        ncg.printDijkstra(i);
    }*/
    cout << "Wall Time = " << wall1 - wall0 << endl;
    cout << "CPU Time  = " << cpu1  - cpu0  << endl;
    return 0;
}
