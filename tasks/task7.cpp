#include<mpi.h>
#include<iostream>

using namespace std;

void RunTask7() {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N = 2;

    int *matrix = new int[N * N];
    int *invertedMatrix = new int[N * N];
    int *vector = new int[N];

    if (rank == 0) {
        cout << "Матрица:\n";
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                matrix[i * N + j] = rand() % 10;
                cout << matrix[i * N + j] << "\t";
            }
            cout << "\n";
        }

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                invertedMatrix[i * N + j] = matrix[j * N + i];
            }
        }

        cout << "Вектор:\n";
        for (int i = 0; i < N; i++) {
            vector[i] = rand() % 10;
            cout << vector[i] << "\t";
        }
        cout << "\n";
    }

    //Разделяем массив в руте, раздаем разделение по всем тредам
    int *matrixLengths = new int[size];
    int *matrixIndexes = new int[size];

    int *vectorLengths = new int[size];
    int *vectorIndexes = new int[size];

    int tagLen = 0, tagInd = 1;
    int vectorTagLen = 2, vectorTagInd = 3;
    MPI_Status status;
    if (rank == 0) {
        int leftCount = N;
        for (int i = 0; i < size; i++) {
            matrixLengths[i] = leftCount / (size - i) * N;
            matrixIndexes[i] = i > 0
                               ? matrixIndexes[i - 1] + matrixLengths[i - 1]
                               : 0;
            vectorLengths[i] = leftCount / (size - i);
            vectorIndexes[i] = i > 0
                               ? vectorIndexes[i - 1] + vectorLengths[i - 1]
                               : 0;
            leftCount -= matrixLengths[i] / N;
        }

        for (int id = 1; id < size; id++) {
            MPI_Send(matrixLengths, size, MPI_INT, id, tagLen, MPI_COMM_WORLD);
            MPI_Send(matrixIndexes, size, MPI_INT, id, tagInd, MPI_COMM_WORLD);
            MPI_Send(vectorLengths, size, MPI_INT, id, vectorTagLen, MPI_COMM_WORLD);
            MPI_Send(vectorIndexes, size, MPI_INT, id, vectorTagInd, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(matrixLengths, size, MPI_INT, 0, tagLen, MPI_COMM_WORLD, &status);
        MPI_Recv(matrixIndexes, size, MPI_INT, 0, tagInd, MPI_COMM_WORLD, &status);
        MPI_Recv(vectorLengths, size, MPI_INT, 0, vectorTagLen, MPI_COMM_WORLD, &status);
        MPI_Recv(vectorIndexes, size, MPI_INT, 0, vectorTagInd, MPI_COMM_WORLD, &status);
    }

    int currentMatrixCount = matrixLengths[rank];
    int *currentSubMatrix = new int[currentMatrixCount];

    int currentVectorCount = vectorLengths[rank];
    int *currentSubVector = new int[currentVectorCount];

    //Разделяет исходный буффер на несколько разного размера
    MPI_Scatterv(invertedMatrix, matrixLengths, matrixIndexes, MPI_INT, currentSubMatrix, currentMatrixCount,
                 MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(vector, vectorLengths, vectorIndexes, MPI_INT, currentSubVector, currentVectorCount,
                 MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int *curSums = new int[N];
    for (int i = 0; i < N; i++) {
        curSums[i] = 0;
    }

    for (int i = 0; i < currentVectorCount; i++) {
        for (int j = 0; j < N; j++) {
            curSums[j] += currentSubMatrix[i * N + j] * currentSubVector[i];
        }
    }
    int *result = new int[N];
    MPI_Reduce(curSums, result, N, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout<<"Результат умножения:\n";
        for (int i = 0; i < N; i++) {
            cout << result[i] << "\t";
        }
    }
}

	