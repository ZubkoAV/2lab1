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
    int grades[4];  // ������ �� 4 ��������

    // ��� ���������� ������� "������"
    int countExcellent() const {
        int count = 0;
        for (int grade : grades) {
            if (grade == 5) count++;
        }
        return count;
    }
};

// ������� ��� ������ �������� � ���������� ����
void RW(std::vector<Student>& students, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "������� �������� ����� ��� ������.\n";
        return;
    }
    for (const auto& student : students) {
        file.write(reinterpret_cast<const char*>(&student), sizeof(Student));
    }
    file.close();
}

// ������� ��� ������� �������� �� ����������� ����� � ����������
void RD(std::vector<Student>& students, const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "������� �������� ����� ��� �������.\n";
        return;
    }
    Student student;
    while (file.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
        students.push_back(student);
    }
    file.close();

    // ���������� �� ��������
    std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.lastName < b.lastName;
        });

    // ��������� ����������
    for (const auto& s : students) {
        std::cout << "Surname: " << s.lastName << ", Name: " << s.firstName
            << ", Course: " << s.course << ", Group: " << s.group << '\n';
    }
}

// ������� ��� ������ �������� � ��������� ����
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

// ������� ��� ������� �������� �� ���������� ����� �� ���������� �� ������� "������"
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

    // Գ�������� �� ������
    std::vector<Student> filteredStudents;
    for (const auto& s : students) {
        if (s.course == courseFilter) {
            filteredStudents.push_back(s);
        }
    }

    // ���������� �� ������� "������"
    std::sort(filteredStudents.begin(), filteredStudents.end(), [](const Student& a, const Student& b) {
        return a.countExcellent() > b.countExcellent();
        });

    // ��������� ����������
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

    // ����� � ������� ����
    RW(students, "INFO.DAT");

    // ������� �� ���������� � �������� �����
    std::vector<Student> binaryStudents;
    RD(binaryStudents, "INFO.DAT");

    // ����� � ��������� ����
    RW(students, "INFO.TXT", true);

    // ������� � ���������� ����� � �������� �� �����
    int courseFilter = 2;
    RD("INFO.TXT", courseFilter);
    courseFilter = 1;
    RD("INFO.TXT", courseFilter);
    return 0;
}
