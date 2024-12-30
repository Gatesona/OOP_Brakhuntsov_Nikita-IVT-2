#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>

// Интерфейс для расчета зарплаты
class ISalaryCalculation {
public:
    virtual double calculate() const = 0; // Чисто виртуальный метод
    virtual ~ISalaryCalculation() {}      // Виртуальный деструктор
};

// Базовый класс "Личность"
class Person {
protected:
    std::string fullName;
    std::string gender;
    int birthYear;

public:
    Person(const std::string& name, const std::string& gen, int birth)
        : fullName(name), gender(gen), birthYear(birth) {
    }

    virtual void print() const {
        std::cout << "Full Name: " << fullName << "\n"
            << "Gender: " << gender << "\n"
            << "Birth Year: " << birthYear << "\n";
    }

    virtual ~Person() {} // Виртуальный деструктор
};

// Класс "Студент"
class Student : public Person, public ISalaryCalculation {
private:
    int enrollmentYear;
    std::string recordBookNumber;
    double averageGrade;

public:
    Student(const std::string& name, const std::string& gen, int birth, int enrollment,
        const std::string& recordBook, double grade)
        : Person(name, gen, birth), enrollmentYear(enrollment),
        recordBookNumber(recordBook), averageGrade(grade) {
    }

    void recalculateAverageGrade(const std::vector<int>& grades) {
        int sum = 0;
        for (int grade : grades) {
            sum += grade;
        }
        averageGrade = static_cast<double>(sum) / grades.size();
    }

    double calculate() const override {
        return averageGrade > 4.5 ? 1000.0 : 700.0;
    }

    void print() const override {
        Person::print();
        std::cout << "Enrollment Year: " << enrollmentYear << "\n"
            << "Record Book Number: " << recordBookNumber << "\n"
            << "Average Grade: " << averageGrade << "\n"
            << "Monthly Payment: " << calculate() << "\n\n";
    }
};

// Класс "Преподаватель"
class Teacher : public Person, public ISalaryCalculation {
private:
    int experience;             // Стаж работы в годах
    int startYear;              // Год начала работы
    std::string position;       // Должность
    std::string academicDegree; // Ученая степень: "candidate" или "doctor"
    std::string academicTitle;  // Ученое звание: "associate professor" или "professor"

public:
    Teacher(const std::string& name, const std::string& gen, int birth, int exp, int start,
        const std::string& pos, const std::string& degree, const std::string& title)
        : Person(name, gen, birth), experience(exp), startYear(start),
        position(pos), academicDegree(degree), academicTitle(title) {
    }

    double calculate() const override {
        double salary = 5000;
        if (academicDegree == "candidate")
            salary += 700;
        else if (academicDegree == "doctor")
            salary += 1200;

        if (academicTitle == "associate professor")
            salary += 2200;
        else if (academicTitle == "professor")
            salary += 3500;

        salary += (experience / 5) * 700; // Доплата за каждые 5 лет стажа
        return salary;
    }

    void print() const override {
        Person::print();
        std::cout << "Position: " << position << "\n"
            << "Experience: " << experience << " years\n"
            << "Start Year: " << startYear << "\n"
            << "Academic Degree: " << academicDegree << "\n"
            << "Academic Title: " << academicTitle << "\n"
            << "Monthly Salary: " << calculate() << "\n\n";
    }
};

int main() {
    // Создание объектов студентов и преподавателей
    std::vector<ISalaryCalculation*> people;

    Student student1("Alice Smith", "Female", 2002, 2020, "AB123", 4.6);
    Student student2("Bob Johnson", "Male", 2001, 2019, "BC234", 4.3);

    Teacher teacher1("Dr. Carol White", "Female", 1980, 20, 2003, "Professor", "doctor", "professor");
    Teacher teacher2("Mr. David Brown", "Male", 1975, 15, 2008, "Lecturer", "candidate", "associate professor");

    // Добавление объектов в контейнер
    people.push_back(&student1);
    people.push_back(&student2);
    people.push_back(&teacher1);
    people.push_back(&teacher2);

    // Полиморфный вызов calculate() и RTTI
    for (const ISalaryCalculation* person : people) {
        if (const Student* student = dynamic_cast<const Student*>(person)) {
            std::cout << "Student Details:\n";
            student->print();
        }
        else if (const Teacher* teacher = dynamic_cast<const Teacher*>(person)) {
            std::cout << "Teacher Details:\n";
            teacher->print();
        }
    }

    return 0;
}