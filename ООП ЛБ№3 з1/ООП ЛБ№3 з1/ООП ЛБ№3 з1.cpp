#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <typeinfo>
#include <cmath>

// Интерфейс ILoggable
class ILoggable {
public:
    virtual void logToScreen() const = 0;
    virtual void logToFile(const std::string& filename) const = 0;
    virtual ~ILoggable() = default;
};

// Интерфейс IShuffle
class IShuffle {
public:
    virtual void shuffle() = 0;
    virtual void shuffle(size_t i, size_t j) = 0;
    virtual ~IShuffle() = default;
};

// Абстрактный класс ExpressionEvaluator
class ExpressionEvaluator : public ILoggable {
protected:
    size_t operandCount;
    double* operands;

public:
    ExpressionEvaluator(size_t count = 20) : operandCount(count), operands(new double[count]()) {}

    virtual ~ExpressionEvaluator() { delete[] operands; }

    void setOperand(size_t pos, double value) {
        if (pos < operandCount) operands[pos] = value;
    }

    void setOperands(double ops[], size_t n) {
        for (size_t i = 0; i < std::min(n, operandCount); ++i) {
            operands[i] = ops[i];
        }
    }

    virtual double calculate() const = 0;

    virtual void logToScreen() const override {
        for (size_t i = 0; i < operandCount; ++i) {
            if (operands[i] < 0) std::cout << "(" << operands[i] << ")";
            else std::cout << operands[i];
            if (i != operandCount - 1) std::cout << " ";
        }
        std::cout << std::endl;
    }

    virtual void logToFile(const std::string& filename) const override {
        std::ofstream file(filename, std::ios::app);
        for (size_t i = 0; i < operandCount; ++i) {
            if (operands[i] < 0) file << "(" << operands[i] << ")";
            else file << operands[i];
            if (i != operandCount - 1) file << " ";
        }
        file << std::endl;
        file.close();
    }
};

// Класс Summator
class Summator : public ExpressionEvaluator {
public:
    using ExpressionEvaluator::ExpressionEvaluator;

    double calculate() const override {
        double sum = 0.0;
        for (size_t i = 0; i < operandCount; ++i) {
            sum += operands[i];
        }
        return sum;
    }
};

// Класс Subtractor
class Subtractor : public ExpressionEvaluator {
public:
    using ExpressionEvaluator::ExpressionEvaluator;

    double calculate() const override {
        double result = operands[0];
        for (size_t i = 1; i < operandCount; ++i) {
            result -= operands[i];
        }
        return result;
    }
};

// Класс Multiplier
class Multiplier : public ExpressionEvaluator, public IShuffle {
public:
    using ExpressionEvaluator::ExpressionEvaluator;

    double calculate() const override {
        double product = 1.0;
        for (size_t i = 0; i < operandCount; ++i) {
            product *= operands[i];
        }
        return product;
    }

    void shuffle() override {
        std::swap(operands[0], operands[operandCount - 1]);
        operands[0] *= -1;
        operands[operandCount - 1] *= -1;
    }

    void shuffle(size_t i, size_t j) override {
        if (i < operandCount && j < operandCount) {
            std::swap(operands[i], operands[j]);
            operands[i] *= -1;
            operands[j] *= -1;
        }
    }
};

// Класс CustomExpressionEvaluator
class CustomExpressionEvaluator : public ExpressionEvaluator {
public:
    using ExpressionEvaluator::ExpressionEvaluator;

    double calculate() const override {
        double result = 0.0;
        for (size_t i = 0; i < operandCount; ++i) {
            result += operands[i] / (i + 1);
        }
        return result;
    }
};

// Функция main()
int main() {
    ExpressionEvaluator* expressions[3];

    // CustomExpressionEvaluator
    double customOperands[] = { 5, 16, -3, 10, 12, 2 };
    expressions[0] = new CustomExpressionEvaluator(6);
    expressions[0]->setOperands(customOperands, 6);

    // Subtractor
    double subtractorOperands[] = { 5.6, -2.1, 3.2, 1.5 };
    expressions[1] = new Subtractor(4);
    expressions[1]->setOperands(subtractorOperands, 4);

    // Multiplier
    expressions[2] = new Multiplier(3);
    expressions[2]->setOperand(0, 1.5);
    expressions[2]->setOperand(1, -8);
    expressions[2]->setOperand(2, 2.5);

    // Демонстрация работы
    for (int i = 0; i < 3; ++i) {
        expressions[i]->logToScreen();
        expressions[i]->logToFile("log.txt");
        std::cout << "Result = " << expressions[i]->calculate() << std::endl;

        // Проверка на IShuffle
        if (auto shuffleObj = dynamic_cast<IShuffle*>(expressions[i])) {
            shuffleObj->shuffle();
            expressions[i]->logToScreen();
            std::cout << "After shuffle, Result = " << expressions[i]->calculate() << std::endl;
        }
        std::cout << "--------------------------" << std::endl;
    }

    // Очистка памяти
    for (int i = 0; i < 3; ++i) {
        delete expressions[i];
    }

    return 0;
}