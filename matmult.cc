// matmult.cc: implements integer matrix multiplication C = A x B
// for simplication, assumes A and B are both n x n matrices
//
// gw

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <omp.h>
#include <time.h>
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

	//SETUP TIMER FOR FILE
    struct timespec begin, end;
    clock_gettime(CLOCK_REALTIME, &begin);
	// input A and B matrices
	read_matrix(A, dim, argv[2]);
	read_matrix(B, dim, argv[3]);

	//END CLOCK AND GET TIME
    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;

    printf("sequential time taken %f\n",elapsed);

	//SETUP TIMER FOR FILE
    struct timespec begin2, end2;
    clock_gettime(CLOCK_REALTIME, &begin2);
	// compute product using straightforward algorithm
	matmult (C, A, B, dim);

	//END CLOCK AND GET TIME
    clock_gettime(CLOCK_REALTIME, &end2);
    long seconds2 = end2.tv_sec - begin2.tv_sec;
    long nanoseconds2 = end2.tv_nsec - begin2.tv_nsec;
    double elapsed2 = seconds2 + nanoseconds2*1e-9;

    printf("parallel time taken %f\n",elapsed2);

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