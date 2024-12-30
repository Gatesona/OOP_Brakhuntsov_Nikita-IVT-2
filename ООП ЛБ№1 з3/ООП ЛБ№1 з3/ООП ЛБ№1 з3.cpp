#include <iostream>
#include <vector>
#include <cstdlib>  // для rand() и srand()
#include <ctime>  // для time()
#include <locale>

char* my_strchr(char* s, int c) {

    while (*s != '\0') {
        if (*s == (char)(c)) {
            return s;
        }
        s++;
    }
    return nullptr;
}

int main()
{
    std::setlocale(LC_ALL, "Russian"); // Установка локали на русскую
    char str[] = "Hello, World!";
    char ch = 'W';

    char* result = my_strchr(str, ch);

    if (result != nullptr) {
        std::cout << "Подстрока, начиная с символа '" << ch << "': " << result << std::endl;
    }
    else {
        std::cout << "Символ '" << ch << "' не найден." << std::endl;
    }

    return 0;
}