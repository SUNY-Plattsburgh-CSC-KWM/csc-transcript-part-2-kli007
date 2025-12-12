#ifndef BANNER_HPP
#define BANNER_HPP

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Course{
    private:
        string identification, designation, name, grade;
        int credit;

    public:
        Course();

        void setCourse(string i, string d, string n, int c, string g);

        void displayCourse();

        string getIdentification() const;

        string getDesignation() const;

        string getName() const;

        int getCredit() const;

        string getGrade() const;
};

class Semester{
    private:
        string year, extension;
        map<string, Course> courselist;
        vector<pair<string, Course>> sortedCourses;
        float semGPA;
        int totalCredits;
        bool NumberOn;

    public:
        Semester();

        void setSemester(string y, string e);

        void createCourse(string input);

        void deleteCourse(string designation); 

        Course* returnCourse(string designation);

        void addToCourseList(string identifier, string designation, string name, int credit, string grade);

        void displaySemester();

        void sortByGrade();

        void sortByNumber();

        void calcSemGPA();

        string getYear() const;

        string getExtension() const;

        map<string, Course>& getCourseList();

        vector<pair<string, Course>>& getSortedCourses();

        float getSemGPA () const;

        int getTotalCredits() const;
};

class Transcript{
    private:
        string studentName;
        map<string, Semester> semList;
        map<string, pair<string, int>> gradeList; //course number, grade, credits
        float GPA;

    public:
        Transcript();

        void createSemester(string input);

        void deleteSemester(string input);//call find course for all course in semester

        Semester* returnSemester(string input);
 
        void displaySummary();

//match each semester's course and grade with the list, if not equal ie the most recent ignore
        void calculateGPA();

        void addToSemesterList(string identifier, string year, string extension);

        void storeData();

        void readData();

        void addGrade();

        string getSemester();

        string getCourse();

        void setStudentName(string name);

        string getStudentName() const;

        float getGPA() const;

        map<string, Semester>& getSemList();

        bool checkSPointer(Semester* Sptr);

        bool checkCPointer(Course* Cptr);
};
#endif