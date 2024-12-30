#include <iostream>
#include <vector>
#include <cstdlib>  // для rand() и srand()
#include <ctime>  // для time()
#include <locale>


int processArray(double arr[], int size, int a, int b, std::vector<double>& nElements) {
    srand(time(0));

    int nCount = 0;

    // Проходим по массиву и домножаем элементы с четными индексами
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            double random = a + (rand() % (b - a + 1));
            arr[i] *= random;
        }

        if (arr[i] < 0) {
            nElements.push_back(arr[i]);
            nCount++;
        }
    }

    return nCount;
}

int main() {
    std::setlocale(LC_ALL, "Russian"); // Установка локали на русскую

    const int n = 15;
    double arr[n];

    for (int i = 0; i < n; ++i) {
        arr[i] = 1.0;
    }

    int a, b;

    // Диапазон для случайных чисел
    std::cout << "Введите отрицательное число a: ";
    std::cin >> a;
    if (a >= 0) {
        std::cout << "Ошибка! а должно быть меньше 0." << std::endl;
        return 1;
    }

    std::cout << "Введите число b: ";
    std::cin >> b;

    std::vector<double> nElements;  // Динамический массив для хранения отрицательных элементов
    int nCount = processArray(arr, n, a, b, nElements);

    std::cout << "Исходный массив: ";
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Массив отрицательных элементов: ";
    for (double i : nElements) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "Количество отрицательных элементов: " << nCount << std::endl;

    return 0;
}