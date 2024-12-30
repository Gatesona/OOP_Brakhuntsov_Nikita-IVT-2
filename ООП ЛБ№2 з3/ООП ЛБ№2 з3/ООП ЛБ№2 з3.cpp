#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>

class Student {
private:
    std::string fullName;
    std::string gender;
    int birthYear;
    int enrollmentYear;
    std::string recordBookNumber;
    double averageGrade;

public:
    // Конструктор по умолчанию
    Student()
        : fullName(""), gender(""), birthYear(0), enrollmentYear(0), recordBookNumber(""), averageGrade(0.0) {
    }

    // Конструктор с параметрами
    Student(const std::string& name, const std::string& gen, int birth, int enrollment, const std::string& recordBook, double grade)
        : fullName(name), gender(gen), birthYear(birth), enrollmentYear(enrollment), recordBookNumber(recordBook), averageGrade(grade) {
    }

    // Сеттеры
    void setFullName(const std::string& name) { fullName = name; }
    void setGender(const std::string& gen) { gender = gen; }
    void setBirthYear(int birth) { birthYear = birth; }
    void setEnrollmentYear(int enrollment) { enrollmentYear = enrollment; }
    void setRecordBookNumber(const std::string& recordBook) { recordBookNumber = recordBook; }
    void setAverageGrade(double grade) { averageGrade = grade; }

    // Геттеры
    std::string getFullName() const { return fullName; }
    std::string getGender() const { return gender; }
    int getBirthYear() const { return birthYear; }
    int getEnrollmentYear() const { return enrollmentYear; }
    std::string getRecordBookNumber() const { return recordBookNumber; }
    double getAverageGrade() const { return averageGrade; }

    // Метод для пересчета среднего балла
    void recalculateAverageGrade(const std::vector<int>& grades) {
        int sum = 0;
        for (int grade : grades) {
            sum += grade;
        }
        averageGrade = static_cast<double>(sum) / grades.size();
    }

    // Методы сериализации
    void serialize() const {
        serialize("student_data.txt");
    }

    void serialize(const std::string& filename) const {
        std::ofstream outFile(filename, std::ios::app);
        if (outFile.is_open()) {
            outFile << fullName << "\n";
            outFile << gender << "\n";
            outFile << birthYear << "\n";
            outFile << enrollmentYear << "\n";
            outFile << recordBookNumber << "\n";
            outFile << averageGrade << "\n";
            outFile.close();
        }
    }

    // Методы десериализации
    void deserialize() {
        deserialize("student_data.txt");
    }

    void deserialize(const std::string& filename) {
        std::ifstream inFile(filename);
        if (inFile.is_open()) {
            std::getline(inFile, fullName);
            std::getline(inFile, gender);
            inFile >> birthYear;
            inFile >> enrollmentYear;
            inFile.ignore(); // Пропуск символа новой строки
            std::getline(inFile, recordBookNumber);
            inFile >> averageGrade;
            inFile.close();
        }
    }

    // Метод для вывода данных студента
    void print() const {
        std::cout << "Full Name: " << fullName << "\n"
            << "Gender: " << gender << "\n"
            << "Birth Year: " << birthYear << "\n"
            << "Enrollment Year: " << enrollmentYear << "\n"
            << "Record Book Number: " << recordBookNumber << "\n"
            << "Average Grade: " << averageGrade << "\n\n";
    }
};

int main() {
    // Массив из трех студентов
    std::vector<Student> students(3);

    // Установка данных через сеттеры
    students[0].setFullName("Alice Smith");
    students[0].setGender("Female");
    students[0].setBirthYear(2002);
    students[0].setEnrollmentYear(2020);
    students[0].setRecordBookNumber("AB123");
    students[0].setAverageGrade(4.5);

    students[1].setFullName("Bob Johnson");
    students[1].setGender("Male");
    students[1].setBirthYear(2001);
    students[1].setEnrollmentYear(2019);
    students[1].setRecordBookNumber("BC234");
    students[1].setAverageGrade(3.8);

    students[2].setFullName("Carol White");
    students[2].setGender("Female");
    students[2].setBirthYear(2000);
    students[2].setEnrollmentYear(2018);
    students[2].setRecordBookNumber("CD345");
    students[2].setAverageGrade(4.2);

    // Студент в куче
    Student* dynamicStudent = new Student("David Brown", "Male", 2003, 2021, "DE456", 4.7);

    // Имитация трех сессий
    srand(time(nullptr));
    for (Student& student : students) {
        for (int i = 0; i < 3; ++i) {
            std::vector<int> grades(4);
            for (int& grade : grades) {
                grade = rand() % 6 + 1; // Оценки от 1 до 6
            }
            student.recalculateAverageGrade(grades);
        }
    }

    // Сортировка студентов по убыванию среднего балла
    std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.getAverageGrade() > b.getAverageGrade();
        });

    // Вывод результатов
    for (const Student& student : students) {
        student.print();
    }

    // Вывод динамического студента
    dynamicStudent->print();

    // Освобождение памяти
    delete dynamicStudent;

    return 0;
}
