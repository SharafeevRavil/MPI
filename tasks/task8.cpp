#include<mpi.h>
#include<iostream>

using namespace std;

void RunTask8() {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int N = 10;

    int step = N / size;
    int remainder = N % size;

    int *currentArray;
    int currentArraySize;

    int tag = 0;
    MPI_Status Status;
    if (rank == 0) {
        int *array = new int[N];
        cout << "Входной массив:\n";
        for (int i = 0; i < N; i++) {
            array[i] = rand() % 1000;
            cout << array[i] << " ";
        }
        cout << "\n";

        currentArraySize = step + remainder;
        currentArray = new int[currentArraySize];
        for (int i = 0; i < currentArraySize; i++) {
            currentArray[i] = array[i];
        }

        int index = remainder + step;
        for (int id = 1; id < size; id++) {
            MPI_Send(&array[index], step, MPI_INT, id, tag, MPI_COMM_WORLD);
            index += step;
        }
    } else {
        currentArraySize = step;
        currentArray = new int[step];
        MPI_Recv(currentArray, step, MPI_INT, 0, tag, MPI_COMM_WORLD, &Status);
    }

    cout << "Поток #" << rank << ", массив: ";
    for (int i = 0; i < currentArraySize; i++) {
        cout << currentArray[i] << " ";
    }
    cout << "\n";

    //Сборка

    if (rank == 0) {
        int *resultArray = new int[N];
        for (int i = 0; i < currentArraySize; i++) {
            resultArray[i] = currentArray[i];
        }

        int index = step + remainder;
        for (int id = 1; id < size; id++) {
            MPI_Recv(&resultArray[index], step, MPI_INT, id, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
            index += step;
        }

        cout << "Поток #" << rank << ", итоговый массив: ";
        for (int i = 0; i < N; i++) {
            cout << resultArray[i] << " ";
        }
        cout << "\n";
    } else {
        MPI_Send(currentArray, step, MPI_INT, 0, rank, MPI_COMM_WORLD);
    }
}
	