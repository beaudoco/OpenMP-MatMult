// generate big, random matrix
//
// gw

#include <iostream>
#include <cstdlib>
using namespace std;

int main (int argc, char *argv[])
{
	// setup/initialize
	if (argc != 2) {
		cerr<< "usage: gen_matrix dim(NxN)" << endl;
		exit (-1);
	}
	int dim = atoi (argv[1]);
	for (int i=0; i < dim; i++)
		for (int j=0; j < dim; j++)
			cout << rand() % 100 << " ";
}