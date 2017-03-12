#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <time.h>
#include <sys/time.h>

using namespace std;

class MPG {
public:
    bool loadMatrix(const char * matrixPath);

private:
    int nodes;
    int **matrix;
    void setMatrixSize();
};

void NCG::setMatrixSize() {
    matrix = new int*[nodes];
    for (int i = 0; i < nodes; i++) 
        matrix[i] = new int[nodes];
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
    char c;
    for (int j = 0; j < nodes; j++) {
        getline(inFile, line);
        istringstream is(line);
        for (int i = 0; i < nodes; i++) {
            is.get(c) >> matrix[j][i];
            
            if (i != j && matrix[j][i] == 0)
                FWDistanceMatrix[j][i] = INT_MAX;
            else
                FWDistanceMatrix[j][i] = matrix[j][i];
        }
    }

    return true;
}

int main( int argc, const char* argv[] )
{

    MPG mpg;

    if ( argc != 2 ) {
        cout << "Bad Input.. 1st parameter: Path to file" << endl;
        return 1;
    }
    mpg.loadMatrix(argv[1]);



}