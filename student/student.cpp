#include "student.h"
/* @author totilina */

Student::Student(const QString& id,
                 const QString& name,
                 const QString& major,
                 const QString& grade,
                 const QString& className){
    this->id = id;
    this->name = name;
    this->major = major;
    this->grade = grade;
    this->className = className;
}

Student::Student(){
}

Student::~Student(){
}

Course::Course(){
}


bool Student::isEqual(const Student& student) const{
    if(this->id == student.id &&
        this->name == student.name &&
        this->major == student.major &&
        this->grade == student.grade){
        return true;
    }
    return false;
}

double Student::getCredit() const{
    double credit = 0.0;
    for(auto it = scores.begin(); it != scores.end(); ++it){
        credit += it->credit;
    }
    return credit;
}

double Student::getAverageScore() const{
    // averageGrade = 0.0;
    double sum = 0.0;
    for(auto it = scores.begin(); it != scores.end(); ++it){
        sum += it->credit*it->score;
    }
    double credit = getCredit();
    if(credit) return sum/credit;
    return 0.0;
    // return averageGrade;
}

double Student::getFailedCoursesCount() const{
    int count = 0;
    for(auto it = scores.begin(); it != scores.end(); ++it){
        if(it->score<60){
            count++;
        }
    }
    return count;
}

double Student::getCourseScoreByName(const QString courseName) const{
    if(courseName == "加权平均") return getAverageScore();
    for(const Course& cour : scores){
        if(cour.name == courseName){
            return cour.score;
        }
    }
    return 0.0;
}
