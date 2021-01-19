#include<mpi.h>
#include<iostream>

using namespace std;

void RunTask2() {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N = 10;
    int elementsInArray = N / size;

    int max;
    int *array;
    if (rank == 0) {
        max = -1;

        array = new int[N];
        cout << "Входной массив:\n";
        for (int i = 0; i < N; i++) {
            array[i] = rand() % 1000;
            cout << array[i] << ' ';
        }
        cout << "\n";
    }

    int *subArray = new int[elementsInArray];

    MPI_Scatter(array, elementsInArray, MPI_INT, subArray, elementsInArray, MPI_INT, 0, MPI_COMM_WORLD);

    int currentMax = 0;
    for (int i = 0; i < elementsInArray; i++) {
        if (subArray[i] > currentMax) {
            currentMax = subArray[i];
        }
    }

    MPI_Reduce(&currentMax, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Максимум: " << max;
    }
}
	