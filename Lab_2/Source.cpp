#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <fstream>
#include <cmath>
#include <map>
#include <random>

// Структура уравнения: ax^2 + bx + c = 0
struct Equation {
    double a, b, c;
    std::string raw;
};

// Ответ студента
struct Answer {
    std::vector<double> roots;
};

// Запись в очереди преподавателя
struct Submission {
    Equation eq;
    Answer ans;
    std::string studentName;
};

// Базовый класс студента
class Student {
protected:
    std::string name;
public:
    Student(std::string n) : name(n) {}
    virtual Answer solve(Equation eq) = 0;
    std::string getName() { return name; }
    virtual ~Student() = default;
};

// Хороший студент: решает всегда верно
class GoodStudent : public Student {
public:
    using Student::Student;
    Answer solve(Equation eq) override {
        double d = eq.b * eq.b - 4 * eq.a * eq.c;
        if (d < 0) return {};
        if (d == 0) return { {-eq.b / (2 * eq.a)} };
        return { {(-eq.b + sqrt(d)) / (2 * eq.a), (-eq.b - sqrt(d)) / (2 * eq.a)} };
    }
};

// Плохой студент: всегда корень 0
class BadStudent : public Student {
public:
    using Student::Student;
    Answer solve(Equation eq) override {
        return { {0.0} };
    }
};

// Средний студент: ошибается в 50% случаев
class AverageStudent : public Student {
public:
    using Student::Student;
    Answer solve(Equation eq) override {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);

        if (dis(gen)) { // Решил верно
            double d = eq.b * eq.b - 4 * eq.a * eq.c;
            if (d < 0) return {};
            if (d == 0) return { {-eq.b / (2 * eq.a)} };
            return { {(-eq.b + sqrt(d)) / (2 * eq.a), (-eq.b - sqrt(d)) / (2 * eq.a)} };
        }
        return { {999.99} }; // Ошибка
    }
};

class Teacher {
private:
    std::queue<Submission> mailbox;
    std::map<std::string, int> gradebook;

    bool check(Equation eq, Answer ans) {

        double d = eq.b * eq.b - 4 * eq.a * eq.c;
        std::vector<double> correct;
        if (d >= 0) {
            if (d == 0) correct.push_back(-eq.b / (2 * eq.a));
            else {
                correct.push_back((-eq.b + sqrt(d)) / (2 * eq.a));
                correct.push_back((-eq.b - sqrt(d)) / (2 * eq.a));
            }
        }

        if (correct.size() != ans.roots.size()) return false;
        for (auto r : ans.roots) {
            bool found = false;
            for (auto c : correct) if (std::abs(r - c) < 1e-5) found = true;
            if (!found) return false;
        }
        return true;
    }

public:
    void receiveSubmission(Submission s) {
        mailbox.push(s);
    }

    void processQueue() {
        std::cout << "--- checking... ---" << std::endl;
        while (!mailbox.empty()) {
            Submission s = mailbox.front();
            if (check(s.eq, s.ans)) {
                gradebook[s.studentName]++;
            }
            else if (gradebook.find(s.studentName) == gradebook.end()) {
                gradebook[s.studentName] = 0;
            }
            mailbox.pop();
        }
    }

    void publishGrades() {
        std::cout << "\nresults:\n" << std::endl;
        std::cout << "name\t\thow much solved" << std::endl;

        for (auto const& pair : gradebook) {
            std::cout << pair.first << "\t\t" << pair.second << std::endl;
        }
    }
};

int main() {

    std::vector<Equation> tasks = {
        {1, -3, 2, "x^2 - 3x + 2 = 0"},
        {1, 2, 1, "x^2 + 2x + 1 = 0"},
        {1, 0, -4, "x^2 - 4 = 0"}
    };

    Teacher teacher;


    std::vector<Student*> students;
    students.push_back(new GoodStudent("Kozhin (G)"));
    students.push_back(new AverageStudent("Krotov (A)"));
    students.push_back(new BadStudent("Chechelev (B)"));

    for (auto s : students) {
        for (const auto& task : tasks) {
            teacher.receiveSubmission({ task, s->solve(task), s->getName() });
        }
    }

    teacher.processQueue();
    teacher.publishGrades();

    for (auto s : students) delete s;

    return 0;
}
