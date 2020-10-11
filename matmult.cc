// matmult.cc: implements integer matrix multiplication C = A x B
// for simplication, assumes A and B are both n x n matrices
//
// gw

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <omp.h>
using namespace std;

void read_matrix(int*, int, char*);
void matmult (int*, int*, int*, int);
void print_matrix (int*, int);

int main (int argc, char *argv[])
{
	int dim;	// matrix is dim x dim

	//SET THREADS ALLOWED IN PROGRAM
    omp_set_num_threads(4);
	
	// setup/initialize
	if (argc != 4) {
		cerr<< "usage: matmult dim(NxN) inputA inputB" << endl;
		exit (-1);
	}
	dim = atoi (argv[1]);
	int *A = new int[dim * dim];
	int *B = new int[dim * dim];
	int *C = new int[dim * dim];

	// input A and B matrices
	read_matrix(A, dim, argv[2]);
	read_matrix(B, dim, argv[3]);

	// compute product using straightforward algorithm
	matmult (C, A, B, dim);

	// check results
	print_matrix (C, dim);
	
	delete [] A;
	delete [] B;
	delete [] C;
	
	return 0;
}


void read_matrix(int *data, int size, char *fileName)
{
	int i,j;
	ifstream inputFile;

	inputFile.open (fileName);
	// read random matrix of size NxN
	for (i=0; i < size; i++)
		for (j=0; j < size; j++)
			inputFile >> data[i*size+j];
}


void matmult (int *dataC, int *dataA, int *dataB, int size)
{
	int i, j, k, sum;

	for (i=0; i < size; i++ ) {	

		#pragma omp parallel for private(j)	private(sum)
		for (j=0; j < size; j++ ) {
			sum = 0;

			#pragma omp parallel for private(k) reduction(+: sum)
			for (k=0; k < size; k++ ) {
				sum += dataA[i*size+k] * dataB[k*size+j];
			}
			dataC[i*size+j] = sum;
		}
	}
}


void print_matrix (int *data, int size)
{
	int i,j;
	ofstream outputFile;

	outputFile.open ("C.dat");
	for (i=0; i < size; i++)
		for (j=0; j < size; j++)
			outputFile << data[i*size+j] << "\t";
	outputFile.close();
}