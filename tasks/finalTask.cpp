#include <mpi.h>
#include <iostream>

using namespace std;

void ShellSort(int *numbers, int array_size) {
    int i, j, increment, temp;
    increment = 3;
    while (increment > 0) {
        for (i = 0; i < array_size; i++) {
            j = i;
            temp = numbers[i];
            while ((j >= increment) && (numbers[j - increment] > temp)) {
                numbers[j] = numbers[j - increment];
                j = j - increment;
            }
            numbers[j] = temp;
        }
        if (increment / 2 != 0)
            increment = increment / 2;
        else if (increment == 1)
            increment = 0;
        else
            increment = 1;
    }
}

int *Merge(int *arr1, int count1, int *arr2, int count2) {
    int *result = new int[count1 + count2];

    int i = 0;
    int j = 0;

    while (i < count1 && j < count2)
        if (arr1[i] < arr2[j]) {
            result[i + j] = arr1[i];
            i++;
        } else {
            result[i + j] = arr2[j];
            j++;
        }

    if (i == count1) {
        while (j < count2) {
            result[i + j] = arr2[j];
            j++;
        }
    } else {
        while (i < count1) {
            result[i + j] = arr1[i];
            i++;
        }
    }
    return result;
}

void RunFinalTask() {
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const bool outputEnabled = true;
    const int N = 1200000;
    int *array = new int[N];

    //
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            array[i] = rand() % 10;
        }
        cout << "Входной массив:\n";
        if (outputEnabled) {
            for (int i = 0; i < N; i++) {
                cout << array[i] << " ";
            }
        }
        cout << "\n";
    }

    //Разделяем массив в руте, раздаем разделение по всем тредам
    int *len = new int[size];
    int *ind = new int[size];

    int tagLen = 0, tagInd = 1;
    MPI_Status status;
    if (rank == 0) {
        int leftCount = N;
        for (int i = 0; i < size; i++) {
            len[i] = leftCount / (size - i);
            ind[i] = i > 0
                     ? ind[i - 1] + len[i - 1]
                     : 0;
            leftCount -= len[i];
        }

        for (int id = 1; id < size; id++) {
            MPI_Send(len, size, MPI_INT, id, tagLen, MPI_COMM_WORLD);
            MPI_Send(ind, size, MPI_INT, id, tagInd, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(len, size, MPI_INT, 0, tagLen, MPI_COMM_WORLD, &status);
        MPI_Recv(ind, size, MPI_INT, 0, tagInd, MPI_COMM_WORLD, &status);
    }

    int currentCount = len[rank];
    int *currentSubArray = new int[currentCount];

    //Разделяет исходный буффер на несколько разного размера
    MPI_Scatterv(array, len, ind, MPI_INT, currentSubArray, currentCount,
                 MPI_INT, 0, MPI_COMM_WORLD);

    //Сортировка шеллом
    ShellSort(currentSubArray, currentCount);

    //Мерджим массивы
    int currentStep = 1;
    while (currentStep < size) {
        if (rank % (2 * currentStep) == 0) {
            int sender = rank + currentStep;
            if (sender < size) {
                //Получаем количество в другом треде
                int senderCount;
                MPI_Recv(&senderCount, 1, MPI_INT, sender, 0, MPI_COMM_WORLD, &status);
                //Получаем массив из другого треда
                int *senderArray = new int[senderCount];
                MPI_Recv(senderArray, senderCount, MPI_INT, sender, 0, MPI_COMM_WORLD, &status);
                //Мерджим массивы во временную переменную, удаляем временные, перезаписываем переменные
                int *tempMerge = Merge(currentSubArray, currentCount, senderArray, senderCount);
                delete[] currentSubArray;
                delete[] senderArray;
                currentSubArray = tempMerge;
                //Изменяем размер массива
                currentCount += senderCount;
            }
        } else {
            int receiver = rank - currentStep;
            //Отправляем размер массива
            MPI_Send(&currentCount, 1, MPI_INT, receiver, 0, MPI_COMM_WORLD);
            //Отправляем массив
            MPI_Send(currentSubArray, currentCount, MPI_INT, receiver, 0, MPI_COMM_WORLD);
            break;
        }
        currentStep *= 2;
    }
    //Результат сортировки в руте
    if (rank == 0) {
        cout << "Отсортированный массив:\n";
        if (outputEnabled) {
            for (int i = 0; i < currentCount; i++) {
                cout << currentSubArray[i] << ' ';
            }
        }
        cout << '\n';
    }
}