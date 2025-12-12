#include "banner.hpp"

using namespace std;

Course::Course(){
    designation = "";
    name = "";
    credit = 0;
    grade = "";
}

void Course::setCourse(string i, string d, string n, int c, string g){
    identification = i;
    designation = d;
    name = n;
    credit = c;
    grade = g;
}

void Course::displayCourse(){
    cout << "Course Designation: " << designation << endl;
    cout << "Course Name: " << name << endl;
    cout << "Course Credit: " << credit << endl;
    cout << "Course Grade: " << grade << endl;
}

string Course::getIdentification() const{
    return identification;
}

string Course::getDesignation() const{
    return designation;
}

string Course::getName() const{
    return name;
}

int Course::getCredit() const{
    return credit;
}

string Course::getGrade() const{
    return grade;
}

Semester::Semester(){
    year = "";
    extension = "";
    semGPA = 0.0;
    NumberOn = true;
}

void Semester::setSemester(string y, string e){
    year = y;
    extension = e;
}

void Semester::createCourse(string input){
    string designation, name, grade;
    int credit;

    cout << "Input course designation: ";
    cin >> designation;
    cout << "Input course name: ";
    cin >> name;
    cout << "Input course credit: ";
    cin >> credit;
    cout << "Input course grade: ";
    cin >> grade;

    addToCourseList(input, designation, name, credit, grade);
}

void Semester::deleteCourse(string designation){
    courselist.erase(designation);
}

Course* Semester::returnCourse(string designation){
    auto it = courselist.find(designation);
    if (it != courselist.end())
        return &it->second;
    return nullptr;
}

void Semester::addToCourseList(string identifer, string designation, string name, int credit, string grade){
    courselist[identifer] = Course();
    courselist[identifer].setCourse(identifer, designation, name, credit, grade);
}

void Semester::displaySemester(){
    cout << "Semester Year: " << year << endl;
    cout << "Semester Extension: " << extension << endl;
    cout << "Courses in Semester: " << endl;

    if (NumberOn) {
        for (const auto& pair : courselist) {
            cout << "- " << pair.first << ", " << pair.second.getName() << endl;
        }
    }
    else {
        sortByGrade();
        for (const auto& pair : sortedCourses) {
            cout << "- " << pair.second.getGrade() << ", " << pair.second.getName() <<  endl;
        }
    }
}

void Semester::sortByGrade(){
    NumberOn = false;
    sortedCourses.clear();

    for (const auto& pair : courselist) {
        sortedCourses.push_back(pair);
    }

    auto gradeRank = [](const string& grade) {
        static map<string, int> rank = {
            {"A", 12}, {"A-", 11}, {"B+", 10}, 
            {"B", 9}, {"B-", 8}, {"C+", 7}, 
            {"C", 6}, {"C-", 5}, {"D+", 4}, 
            {"D", 3}, {"F", 1}
        };
        auto it = rank.find(grade);
        return it != rank.end() ? it->second : 0;
    };

    sort(sortedCourses.begin(), sortedCourses.end(),
        [&](const auto &a, const auto &b) {
            return gradeRank(a.second.getGrade()) > gradeRank(b.second.getGrade());
        });
}


void Semester::sortByNumber(){
    NumberOn = true;
}

void Semester::calcSemGPA(){
    semGPA = 0.0f;
    totalCredits = 0;

    static map<string, float> rank = {
        {"A", 4.0}, {"A-", 3.7}, {"B+", 3.3}, 
        {"B", 3.0}, {"B-", 2.7}, {"C+", 2.3}, 
        {"C", 2.0}, {"C-", 1.7}, {"D+", 1.3}, 
        {"D", 1.0},  {"F", 0.0}
    };

    for (const auto& pair : courselist) {
        string grade = pair.second.getGrade();
        auto it = rank.find(grade);
        totalCredits += pair.second.getCredit();
        semGPA += (it -> second * pair.second.getCredit());
    }
    
    semGPA /= totalCredits;
}

string Semester::getYear() const{
    return year;
}

string Semester::getExtension() const{
    return extension;
}

map<string, Course>& Semester::getCourseList(){
    return courselist;
}

vector<pair<string, Course>>& Semester::getSortedCourses(){
    return sortedCourses;
}
float Semester::getSemGPA () const{
    return semGPA;
}

int Semester::getTotalCredits() const{
    return totalCredits;
}
