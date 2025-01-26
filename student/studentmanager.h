#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H

#include "student.h"
#include <QVector>



class StudentManager{
public:
    StudentManager();
    ~StudentManager();
    QVector<Student> students;
    QVector<Course> courses;

    bool dataImportSucceeded = true;
    bool studentDataImportSucceeded = true;
    bool courseDataImportSucceeded = true;

    bool loadData();
    bool saveData();

    bool addStudent(const Student& student);
    bool updateStudent(const Student& oldStudent, const Student& newStudent);
    bool deleteStudent(const Student& student);

    Student findStudentById(const QString& studentId) const;
    QVector<Student> findStudentsByName(const QString& studnetName) const;
    QVector<Student> findStudentsByMajor(const QString& major) const;
    QVector<Student> findStudentsByGrade(const QString& grade) const;
    QVector<Student> findStudentsByClass(const QString& className) const;

    Course findCourseByName(const QString& courseName) const;

private:
    QString dataFilePath = "data.json";
};

#endif // STUDENTMANAGER_H
