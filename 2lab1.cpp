#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Student {
    std::string lastName;
    std::string firstName;
    char gender;
    int course;
    int group;
    int grades[4];  // Оцінки за 4 предмети

    // Для обчислення кількості "відмінно"
    int countExcellent() const {
        int count = 0;
        for (int grade : grades) {
            if (grade == 5) count++;
        }
        return count;
    }
};

// Функція для запису студентів у типізований файл
void RW(std::vector<Student>& students, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Помилка відкриття файлу для запису.\n";
        return;
    }
    for (const auto& student : students) {
        file.write(reinterpret_cast<const char*>(&student), sizeof(Student));
    }
    file.close();
}

// Функція для читання студентів із типізованого файлу і сортування
void RD(std::vector<Student>& students, const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Помилка відкриття файлу для читання.\n";
        return;
    }
    Student student;
    while (file.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
        students.push_back(student);
    }
    file.close();

    // Сортування за прізвищем
    std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.lastName < b.lastName;
        });

    // Виведення результату
    for (const auto& s : students) {
        std::cout << "Surname: " << s.lastName << ", Name: " << s.firstName
            << ", Course: " << s.course << ", Group: " << s.group << '\n';
    }
}

// Функція для запису студентів у текстовий файл
void RW(const std::vector<Student>& students, const std::string& filename, bool isText) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error\n";
        return;
    }

    for (const auto& student : students) {
        file << student.lastName << " " << student.firstName << " "
            << student.course << " " << student.group << " ";
        for (int grade : student.grades) {
            file << grade << " ";
        }
        file << '\n';
    }
    file.close();
}

// Функція для читання студентів із текстового файлу та сортування за кількістю "відмінно"
void RD(const std::string& filename, int courseFilter) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error\n";
        return;
    }

    std::vector<Student> students;
    Student student;

    while (file >> student.lastName >> student.firstName >> student.course >> student.group) {
        for (int& grade : student.grades) {
            file >> grade;
        }
        students.push_back(student);
    }
    file.close();

    // Фільтрація за курсом
    std::vector<Student> filteredStudents;
    for (const auto& s : students) {
        if (s.course == courseFilter) {
            filteredStudents.push_back(s);
        }
    }

    // Сортування за кількістю "відмінно"
    std::sort(filteredStudents.begin(), filteredStudents.end(), [](const Student& a, const Student& b) {
        return a.countExcellent() > b.countExcellent();
        });

    // Виведення результату
    for (const auto& s : filteredStudents) {
        std::cout << "Surname: " << s.lastName << ", Name: " << s.firstName
            << ", Course: " << s.course << ", Group: " << s.group
            << ", Count excellent: " << s.countExcellent() << '\n';
    }
}

int main() {
    std::vector<Student> students = {
        {"Syn", "Ivan", 'M', 2, 101, {5, 4, 3, 5}},
        {"Melnyk", "Olga", 'F', 1, 102, {5, 5, 5, 4}},
        {"Vozhga", "Vlad", 'M', 2, 101, {4, 5, 5, 5}}
    };

    // Запис у бінарний файл
    RW(students, "INFO.DAT");

    // Читання та сортування з бінарного файлу
    std::vector<Student> binaryStudents;
    RD(binaryStudents, "INFO.DAT");

    // Запис у текстовий файл
    RW(students, "INFO.TXT", true);

    // Читання з текстового файлу з фільтром по курсу
    int courseFilter = 2;
    RD("INFO.TXT", courseFilter);
    courseFilter = 1;
    RD("INFO.TXT", courseFilter);
    return 0;
}
