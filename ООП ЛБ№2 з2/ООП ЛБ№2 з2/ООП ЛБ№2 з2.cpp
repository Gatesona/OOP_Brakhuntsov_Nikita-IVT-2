#include <iostream>
#include <cmath>
#include <cstring>
#include <sstream>

class Fraction {
private:
    int numerator;
    int denominator;
    static int instance_count;

    void normalize() {
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
    }

public:
    Fraction(int num = 0, int denom = 1) : numerator(num), denominator(denom) {
        if (denom == 0) {
            throw std::invalid_argument("Denominator cannot be zero");
        }
        reduce();
        ++instance_count;
    }

    ~Fraction() {
        --instance_count;
    }

    static int gcd(int n, int m) {
        n = std::abs(n);
        m = std::abs(m);
        while (m != 0) {
            int temp = m;
            m = n % m;
            n = temp;
        }
        return n;
    }

    void reduce() {
        int divisor = gcd(numerator, denominator);
        numerator /= divisor;
        denominator /= divisor;
        normalize();
    }

    Fraction operator+(const Fraction& other) const {
        int num = numerator * other.denominator + other.numerator * denominator;
        int denom = denominator * other.denominator;
        return Fraction(num, denom);
    }

    Fraction operator-(const Fraction& other) const {
        int num = numerator * other.denominator - other.numerator * denominator;
        int denom = denominator * other.denominator;
        return Fraction(num, denom);
    }

    Fraction operator*(const Fraction& other) const {
        int num = numerator * other.numerator;
        int denom = denominator * other.denominator;
        return Fraction(num, denom);
    }

    Fraction operator/(const Fraction& other) const {
        if (other.numerator == 0) {
            throw std::invalid_argument("Division by zero");
        }
        int num = numerator * other.denominator;
        int denom = denominator * other.numerator;
        return Fraction(num, denom);
    }

    static void printAsFraction(double decimal_fraction) {
        const int precision = 1000000; // Precision for conversion
        int denom = precision;
        int num = static_cast<int>(std::round(decimal_fraction * precision));
        Fraction frac(num, denom);
        frac.reduce();
        std::cout << frac << std::endl;
    }

    static void printAsFraction(const char* decimal_fraction) {
        double value = std::stod(decimal_fraction);
        printAsFraction(value);
    }

    static int getInstanceCount() {
        return instance_count;
    }

    friend std::ostream& operator<<(std::ostream& os, const Fraction& fraction) {
        os << fraction.numerator << "/" << fraction.denominator;
        return os;
    }
};

int Fraction::instance_count = 0;

int main() {
    try {
        Fraction a(1, 2);
        Fraction b(3, 4);

        std::cout << "Fraction a: " << a << std::endl;
        std::cout << "Fraction b: " << b << std::endl;

        std::cout << "a + b = " << (a + b) << std::endl;
        std::cout << "a - b = " << (a - b) << std::endl;
        std::cout << "a * b = " << (a * b) << std::endl;
        std::cout << "a / b = " << (a / b) << std::endl;

        std::cout << "Decimal 0.25 as fraction: ";
        Fraction::printAsFraction(0.25);

        std::cout << "String \"0.43\" as fraction: ";
        Fraction::printAsFraction("0.43");

        std::cout << "Current number of Fraction instances: " << Fraction::getInstanceCount() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
