#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <sstream>

using namespace std;

class Student
{
private:
    int id;
    string name;
    float grade;
    int attendance;
    int totalScore;
    int blueMarks, redMarks, blackMarks;

public:
    Student() : id(0), name(""), grade(0), attendance(0), totalScore(100), blueMarks(0), redMarks(0), blackMarks(0) {}

    Student(int id, const string &name, float grade, int attendance)
        : id(id), name(name), grade(grade), attendance(attendance), totalScore(100), blueMarks(0), redMarks(0), blackMarks(0) {}

    Student(int id, const string &name, float grade, int attendance, int totalScore, int blueMarks, int redMarks, int blackMarks)
        : id(id), name(name), grade(grade), attendance(attendance), totalScore(totalScore), blueMarks(blueMarks), redMarks(redMarks), blackMarks(blackMarks) {}

    int getId() const { return id; }
    const string &getName() const { return name; }
    float getGrade() const { return grade; }
    int getAttendance() const { return attendance; }
    int getTotalScore() const { return totalScore; }

    void updateDetails(const string &newName, float newGrade, int newAttendance)
    {
        name = newName;
        grade = newGrade;
        attendance = newAttendance;
    }

    void assignMark(char markType)
    {
        switch (markType)
        {
        case 'B':
            totalScore = max(0, totalScore - 5);
            ++blueMarks;
            break;
        case 'R':
            totalScore = max(0, totalScore - 10);
            ++redMarks;
            break;
        case 'K':
            totalScore = max(0, totalScore - 15);
            ++blackMarks;
            break;
        default:
            throw invalid_argument("Invalid mark type.");
        }

        if (blackMarks >= 3)
        {
            cout << "WARNING: Student " << name << " is marked as Bad Student!\n";
        }
    }

    void displayReport() const
    {
        cout << "ID: " << id << ", Name: " << name
             << ", Grade: " << grade
             << ", Attendance: " << attendance
             << ", Total Score: " << totalScore
             << ", Blue Marks: " << blueMarks
             << ", Red Marks: " << redMarks
             << ", Black Marks: " << blackMarks << '\n';
    }

    string serialize() const
    {
        ostringstream out;
        out << id << ',' << name << ',' << grade << ',' << attendance << ','
            << totalScore << ',' << blueMarks << ',' << redMarks << ',' << blackMarks;
        return out.str();
    }

    static Student deserialize(const string &line)
    {
        istringstream in(line);
        int id, attendance, totalScore, blueMarks, redMarks, blackMarks;
        float grade;
        string name, token;

        getline(in, token, ',');
        id = stoi(token);
        getline(in, name, ',');
        getline(in, token, ',');
        grade = stof(token);
        getline(in, token, ',');
        attendance = stoi(token);
        getline(in, token, ',');
        totalScore = stoi(token);
        getline(in, token, ',');
        blueMarks = stoi(token);
        getline(in, token, ',');
        redMarks = stoi(token);
        getline(in, token);
        blackMarks = stoi(token);

        return Student(id, name, grade, attendance, totalScore, blueMarks, redMarks, blackMarks);
    }
};

class StudentDatabase
{
private:
    unordered_map<int, Student> students;
    const string filename = "students.txt";

    void loadFromFile()
    {
        ifstream file(filename);
        if (!file.is_open())
            return;
        string line;
        while (getline(file, line))
        {
            try
            {
                Student student = Student::deserialize(line);
                students[student.getId()] = student;
            }
            catch (const exception &e)
            {
            }
        }
        file.close();
    }

    void saveToFile()
    {
        ofstream file(filename);
        if (!file)
            return;
        for (const auto &pair : students)
        {
            file << pair.second.serialize() << '\n';
        }
        file.close();
    }

public:
    StudentDatabase() { loadFromFile(); }

    ~StudentDatabase() { saveToFile(); }

    void addStudent(int id, const string &name, float grade, int attendance)
    {
        if (students.find(id) != students.end())
            throw invalid_argument("Student ID already exists.");
        students.emplace(id, Student(id, name, grade, attendance));
    }

    void updateStudent(int id, const string &name, float grade, int attendance)
    {
        auto it = students.find(id);
        if (it == students.end())
            throw invalid_argument("Student ID not found.");
        it->second.updateDetails(name, grade, attendance);
    }

    void deleteStudent(int id)
    {
        if (students.erase(id) == 0)
            throw invalid_argument("Student ID not found.");
    }

    void assignMark(int id, char markType)
    {
        auto it = students.find(id);
        if (it == students.end())
            throw invalid_argument("Student ID not found.");
        it->second.assignMark(markType);
    }

    void displayStudent(int id) const
    {
        auto it = students.find(id);
        if (it == students.end())
            throw invalid_argument("Student ID not found.");
        it->second.displayReport();
    }

    void sortAndDisplayAll(char sortType) const
    {
        vector<Student> sortedStudents;
        for (const auto &pair : students)
        {
            sortedStudents.push_back(pair.second);
        }

        if (sortType == 'I')
        {
            sort(sortedStudents.begin(), sortedStudents.end(), [](const Student &a, const Student &b)
                 { return a.getId() < b.getId(); });
        }
        else if (sortType == 'N')
        {
            sort(sortedStudents.begin(), sortedStudents.end(), [](const Student &a, const Student &b)
                 { return a.getName() < b.getName(); });
        }
        else if (sortType == 'G')
        {
            sort(sortedStudents.begin(), sortedStudents.end(), [](const Student &a, const Student &b)
                 { return a.getGrade() > b.getGrade(); });
        }
        else if (sortType == 'A')
        {
            sort(sortedStudents.begin(), sortedStudents.end(), [](const Student &a, const Student &b)
                 { return a.getAttendance() > b.getAttendance(); });
        }

        for (const auto &student : sortedStudents)
        {
            student.displayReport();
        }
    }
};

bool login()
{
    const string correctPassword = "password123";
    string enteredPassword;
    cout << "Enter the password to access the system: ";
    cin >> enteredPassword;
    return enteredPassword == correctPassword;
}

int main()
{
    if (!login())
    {
        cout << "Incorrect password. Exiting program.\n";
        return 0;
    }

    cout << "\nAccess Granted.\n";

    cout << "\nWelcome to Student Management System.\n";

    StudentDatabase db;

    while (true)
    {
        cout << "\n1. Add Student\n2. Update Student\n3. Delete Student\n"
             << "4. Assign Mark\n5. Display Student\n6. Display All Students\n"
             << "7. Exit\nChoose an option: ";
        int choice, id, attendance;
        string name;
        char markType, sortType;
        float grade;

        try
        {
            cin >> choice;
            switch (choice)
            {
            case 1:
                cout << "Enter ID: ";
                cin >> id;
                cout << "Enter Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Grade: ";
                cin >> grade;
                cout << "Enter Attendance: ";
                cin >> attendance;
                db.addStudent(id, name, grade, attendance);
                break;
            case 2:
                cout << "Enter ID: ";
                cin >> id;
                cout << "Enter New Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter New Grade: ";
                cin >> grade;
                cout << "Enter New Attendance: ";
                cin >> attendance;
                db.updateStudent(id, name, grade, attendance);
                break;
            case 3:
                cout << "Enter ID to delete: ";
                cin >> id;
                db.deleteStudent(id);
                break;
            case 4:
                cout << "Enter ID and Mark Type (B/R/K): ";
                cin >> id >> markType;
                db.assignMark(id, markType);
                break;
            case 5:
                cout << "Enter ID to display: ";
                cin >> id;
                db.displayStudent(id);
                break;
            case 6:
                cout << "Sort by: I (ID), N (Name), G (Grade), A (Attendance): ";
                cin >> sortType;
                db.sortAndDisplayAll(sortType);
                break;
            case 7:
                cout << "Exiting...";
                return 0;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        }
        catch (const exception &e)
        {
            cerr << "Error: " << e.what() << '\n';
        }
    }
}