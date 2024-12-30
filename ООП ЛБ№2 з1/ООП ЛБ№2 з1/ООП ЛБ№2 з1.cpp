#include <iostream>
#include <iomanip>
#include <stdexcept>

// Класс Vector для работы с одномерными массивами
class Vector {
private:
    int* data;
    size_t size;

public:
    // Конструктор
    Vector(size_t size) : size(size) {
        data = new int[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = 0;
        }
    }

    // Деструктор
    ~Vector() {
        delete[] data;
    }

    // Перегрузка оператора индексации
    int& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const int& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    // Перегрузка операторов инкремента и декремента
    Vector& operator++() { // Префиксный инкремент
        for (size_t i = 0; i < size; ++i) {
            ++data[i];
        }
        return *this;
    }

    Vector operator++(int) { // Постфиксный инкремент
        Vector temp = *this;
        ++(*this);
        return temp;
    }

    Vector& operator--() { // Префиксный декремент
        for (size_t i = 0; i < size; ++i) {
            --data[i];
        }
        return *this;
    }

    Vector operator--(int) { // Постфиксный декремент
        Vector temp = *this;
        --(*this);
        return temp;
    }

    // Вывод массива
    void print() const {
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }

    size_t getSize() const {
        return size;
    }
};

// Класс Matrix для работы с двумерными массивами
class Matrix {
private:
    int** data;
    size_t rows;
    size_t cols;

public:
    // Конструктор
    Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
        data = new int* [rows];
        for (size_t i = 0; i < rows; ++i) {
            data[i] = new int[cols];
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] = 0;
            }
        }
    }

    // Деструктор
    ~Matrix() {
        for (size_t i = 0; i < rows; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }

    // Методы at и setAt
    int at(size_t i, size_t j) const {
        if (i >= rows || j >= cols) {
            throw std::out_of_range("Index out of range");
        }
        return data[i][j];
    }

    void setAt(size_t i, size_t j, int val) {
        if (i >= rows || j >= cols) {
            throw std::out_of_range("Index out of range");
        }
        data[i][j] = val;
    }

    // Перегрузка операторов инкремента и декремента
    Matrix& operator++() { // Префиксный инкремент
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                ++data[i][j];
            }
        }
        return *this;
    }

    Matrix operator++(int) { // Постфиксный инкремент
        Matrix temp = *this;
        ++(*this);
        return temp;
    }

    Matrix& operator--() { // Префиксный декремент
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                --data[i][j];
            }
        }
        return *this;
    }

    Matrix operator--(int) { // Постфиксный декремент
        Matrix temp = *this;
        --(*this);
        return temp;
    }

    // Вывод массива
    void print() const {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                std::cout << std::setw(4) << data[i][j];
            }
            std::cout << std::endl;
        }
    }

    size_t getRows() const {
        return rows;
    }

    size_t getCols() const {
        return cols;
    }
};

// Функция преобразования из Vector в Matrix
Matrix convertToMatrix(const Vector& vec, size_t rows, size_t cols) {
    if (vec.getSize() != rows * cols) {
        throw std::invalid_argument("Size mismatch between vector and matrix");
    }

    Matrix mat(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            mat.setAt(i, j, vec[i * cols + j]);
        }
    }
    return mat;
}

// Функция преобразования из Matrix в Vector
Vector convertToVector(const Matrix& mat) {
    size_t rows = mat.getRows();
    size_t cols = mat.getCols();

    Vector vec(rows * cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            vec[i * cols + j] = mat.at(i, j);
        }
    }
    return vec;
}

int main() {
    try {
        // Создание и инициализация вектора
        Vector vec(6);
        for (size_t i = 0; i < vec.getSize(); ++i) {
            vec[i] = static_cast<int>(i + 1);
        }

        std::cout << "Original Vector:" << std::endl;
        vec.print();

        // Преобразование вектора в матрицу
        Matrix mat = convertToMatrix(vec, 2, 3);

        std::cout << "Converted Matrix:" << std::endl;
        mat.print();

        // Преобразование матрицы обратно в вектор
        Vector newVec = convertToVector(mat);

        std::cout << "Converted Back to Vector:" << std::endl;
        newVec.print();

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}