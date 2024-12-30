#include <iostream>
#include <vector>
#include <cstdlib>  // для rand() и srand()
#include <ctime>  // для time()
#include <locale>
// 2 задача 
#include <cmath>   // использования sin и M_PI
#include <iomanip> // для вывода 


void processArray(double* arr1D, double (*arr2D)[5], int size) {
    int rows = 5, cols = 5;

    // Инициализация одномерного массива по формуле x[n] = n * sin(πn / 25)
    for (int n = 0; n < size; ++n)
    {
        *(arr1D + n) = n * sin(3.14 * n / 25);
    }

    // Разложение одномерного массива в двумерный 5x5
    for (int i = 0; i < rows; ++i) {
        double sum = 0;
        for (int j = 1; j < cols; ++j) {
            *(*(arr2D + i) + j) = *(arr1D + i * cols + j);
            sum += *(*(arr2D + i) + j);
        }
        *(*(arr2D + i) + 0) = sum;
    }
}

int main() {
    using std::cout;
    using std::endl;

    const int rows = 5, cols = 5;

    double arr1D[rows * cols];
    double arr2D[rows][cols];

    processArray(arr1D, arr2D, rows * cols);

    // Вывод двумерного массива с шириной каждого элемента 10 позиций
    cout << std::fixed << std::setprecision(4);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << std::setw(10) << *(*(arr2D + i) + j);  // Выводим элементы через указатели
        }
        cout << endl;
    }

    return 0;
}