
#include "banner.hpp"

using namespace std;


Transcript::Transcript(){
    studentName = "";
    GPA = 0.0;
}

void Transcript::createSemester(string input){
    string year, extension;
    cout << "Input semester year: ";
    cin >> year;
    cout << "Input semester extension: ";
    cin >> extension;

    addToSemesterList(input, year, extension);
}

void Transcript::deleteSemester(string input){
    semList.erase(input);
}

void Transcript::displaySummary(){
    calculateGPA();
    cout << "Transcript Summary for " << studentName << endl;
    cout << "Overall GPA: " << GPA << endl;
    cout << "Semesters:" << endl;
    for (auto& pair : semList) {
        pair.second.calcSemGPA();
        cout << "- Semester: " << pair.first << " (Year: " << pair.second.getYear() 
             << ", Extension: " << pair.second.getExtension() 
             << ", Semester GPA: "  << pair.second.getSemGPA() << ")" << endl;
    }
}

void Transcript::calculateGPA(){
    GPA = 0.0f;
    addGrade();
    static map<string, float> rank = {
        {"A", 4.0}, {"A-", 3.7}, {"B+", 3.3}, 
        {"B", 3.0}, {"B-", 2.7}, {"C+", 2.3}, 
        {"C", 2.0}, {"C-", 1.7}, {"D+", 1.3}, 
        {"D", 1.0},  {"F", 0.0}
    };
    int totalCredits = 0;

    for (auto& pair: gradeList) {
        string grade = pair.second.first;
        int credits = pair.second.second;

        GPA += rank[grade] * credits;
        totalCredits += credits;
    }
    if (totalCredits > 0)
        GPA /= totalCredits;
    else
        GPA = 0.0;
}

Semester* Transcript::returnSemester(string input){
    auto it = semList.find(input);
    if (it != semList.end())
        return &it->second;
    return nullptr;
}


void Transcript::addToSemesterList(string identifier, string year, string extension){
    semList[identifier] = Semester();
    semList[identifier].setSemester(year, extension);
}

void Transcript::storeData(){
    ofstream outFile("transcript_data.txt");

    if (!outFile) {
        return;
    }

    outFile << studentName << endl;
    outFile << GPA << endl;
    outFile << "Semesters: " << semList.size() << endl << endl;

    for (auto& semPair : semList) {
        Semester& sem = semPair.second;
        outFile << "[Semester]" << endl;
        outFile << "Year: " << sem.getYear() << endl;
        outFile << "Extension: " << sem.getExtension() << endl;
        outFile << "GPA: " << sem.getSemGPA() << endl;
        outFile << "CourseCount: " << sem.getCourseList().size() << endl;
        outFile << "Courses: " << endl;

        for (auto& coursePair : sem.getCourseList()) {
            Course& c = coursePair.second;
            outFile << coursePair.first << "|"
                    << c.getDesignation() << "|" 
                    << c.getName() << "|" 
                    << c.getCredit() << "|" 
                    << c.getGrade() << endl;
        }
        outFile << endl;
    }

    outFile.close();
}
void Transcript::readData(){
    ifstream inFile("transcript_data.txt");
    if (!inFile.is_open()) {
        return;
    }

    semList.clear(); // clear any existing data
    string line;

    while (getline(inFile, line)) {
        if (line == "[Semester]") {
            string year, extension;
            float gpa = 0.0;
            int courseCount = 0;

            getline(inFile, line);  year = line.substr(6);           // skip "Year: "
            getline(inFile, line);  extension = line.substr(11);     // skip "Extension: "
            getline(inFile, line);  gpa = stof(line.substr(5));      // skip "GPA: "
            getline(inFile, line);  courseCount = stoi(line.substr(13)); // skip "CourseCount: "

            addToSemesterList(year + extension, year, extension);

            Semester& newSem = semList[year + extension];
            getline(inFile, line);
            if (line.find("Courses:") != string::npos) {
                for (int i = 0; i < courseCount; i++) {
                    getline(inFile, line);
                    if (line.empty()) break;

                    // Split the line by '|'
                    vector<string> fields;
                    string field;
                    stringstream ss(line);

                    while (getline(ss, field, '|')) {
                        fields.push_back(field);
                    }

                    // fields = [ID, DESIGNATION, NAME WITH SPACES, CREDIT, GRADE]
                    if (fields.size() == 5) {
                        string identifier  = fields[0];
                        string designation = fields[1];
                        string name        = fields[2];  // THIS NOW ALLOWS SPACES
                        int credit         = stoi(fields[3]);
                        string grade       = fields[4];

                        newSem.addToCourseList(identifier, designation, name, credit, grade);
                    }
                }
            } 
        }
    }

    inFile.close();
}

void Transcript::addGrade(){
    for (auto& semPair : semList) {
        Semester& sem = semPair.second;
        auto& courseList = sem.getCourseList();
        for (auto& coursePair : courseList) {
            auto &identifier = coursePair.first;
            string grade = coursePair.second.getGrade();
            int credits = coursePair.second.getCredit();
            gradeList[identifier] = {grade, credits};
        }
        
    }
}

string Transcript::getSemester(){
    string temp;
    cout << "Input semester identifier: ";
    cin >> temp;
    return temp;
}

string Transcript::getCourse(){
    string temp;
    cout << "Input course identifier: ";
    cin >> temp;
    return temp;
}

void Transcript::setStudentName(string name){
    studentName = name;
}

string Transcript::getStudentName() const{
    return studentName;
}

float Transcript::getGPA() const{
    return GPA;
}

map<string, Semester>& Transcript::getSemList(){
    return semList;
}

bool Transcript::checkSPointer(Semester* Sptr){
    if (Sptr == nullptr){
        cout << "Semester not found." << endl; 
        return true;
    }
    else return false;
}

bool Transcript::checkCPointer(Course* Cptr){
    if (Cptr == nullptr){
        cout << "Course not found." << endl;
        return true;
    }
    else return false;
}
