#include <iostream>

using namespace std;

void FillStart(double **temperature, int xCount, int yCount) {
    for (int i = 1; i < xCount - 1; i++) {
        for (int j = 1; j < yCount - 1; j++) {
            temperature[i][j] = 300;
        }
    }
}

void FillEdges(double **temperature, int xCount, int yCount, double xStart, double xStep) {
    //y
    for (int j = 0; j < yCount; j++) {
        //x == 0
        temperature[0][j] = 600;
        //x == 1
        temperature[xCount - 1][j] = 1200;
    }
    //x
    double x = xStart;
    for (int i = 0; i < xCount; i++) {
        //y == 0
        temperature[i][0] = 600 * (1 + x);
        //y = 0.5
        temperature[i][yCount - 1] = 600 * (1 + x * x * x);
        x += xStep;
    }
}

double* GetXStartArray(int yCount){
    double* array = new double [yCount];
    for (int j = 0; j < yCount; j++) {
        array[j] = 600;
    }
    return array;
}

double* GetXEndArray(int yCount){
    double* array = new double [yCount];
    for (int j = 0; j < yCount; j++) {
        array[j] = 1200;
    }
    return array;
}

void PrintMatrix(double **temperature, int xCount, int yCount) {
    for (int j = yCount - 1; j >= 0; j--) {
        for (int i = 0; i < xCount; i++) {
            cout << temperature[i][j] << " ";
        }
        cout << "\n";
    }
}

void RunFinal86Task() {
    //input
    double xStart = 0;
    double xEnd = 1;
    double yStart = 0;
    double yEnd = 0.5;
    double tStart = 0;
    double tEnd = 10;

    int xCount = 200;
    int yCount = 100;
    double xStep = (xEnd - xStart) / xCount;//h(x)
    double yStep = (yEnd - yStart) / yCount;//h(y)
    double tStep = min(xStep, yStep);//tau
    xCount++;
    yCount++;

    //program start

    double* xStartArray = GetXStartArray(yCount);
    double* xEndArray = GetXEndArray(yCount);

    //fill matrix
    double **temperature = new double *[xCount]; // NOLINT(modernize-use-auto)
    for (int i = 0; i < xCount; i++) {
        temperature[i] = new double[yCount];
    }
    FillStart(temperature, xCount, yCount);
    FillEdges(temperature, xCount, yCount, xStart, xStep);

    //PrintMatrix(temperature, xCount, yCount);

    //calculate
    double currentTime = tStart;
    while(currentTime < tEnd){
        currentTime+= tStep;
        //в направлении x
        for (int j = 0; j < yCount; j++) {

        }
        //в направлении y
    }
}