#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <time.h>
#include <sys/time.h>

using namespace std;

class NCG {
public:
	bool loadMatrix(const char * matrixPath);
	void TESTPrintMatrix();
	void Dijkstra();
	void Floyed();
private:
	void setMatrixSize();

	int nodes;
	int** matrix;
};

void NCG::TESTPrintMatrix() {
	for (int i = 0; i < nodes; i++) {
		for (int j = 0; j < nodes; j++)
			cout << matrix[i][j] << " ";
		cout << endl;
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
		}
	}

	return true;
}

void NCG::setMatrixSize() {
	matrix = new int*[nodes];
	for (int i = 0; i < nodes; i++)
		matrix[i] = new int[nodes];
}



int main( int argc, const char* argv[] )
{
	NCG ncg;

	if ( argc != 2 ) {
		cout << "Bad Input.. 1st parameter: Path to file" << endl;
		return 1;
	}
	ncg.loadMatrix(argv[1]);
	ncg.TESTPrintMatrix();
	return 0;
}
