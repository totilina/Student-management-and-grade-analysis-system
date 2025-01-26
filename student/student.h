#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QMap>
#include <QVector>

class Course{
public:
    QString name;
    QString id;
    QString major;
    QString semester;
    double score;
    double credit;

    Course();
};

class Student{
public:
    QString name;
    QString id;
    QString major;
    QString grade;
    QString className;
    double averageGrade;
    double failedCourseCount;

    QVector<Course> scores;

    Student(const QString& id,
            const QString& name,
            const QString& major,
            const QString& grade,
            const QString& className);

    Student();
    ~Student();

    // 这些函数看名字应该就知道是什么作用了吧
    bool isEqual(const Student& student) const;
    double getAverageScore() const;
    double getCourseScoreByName(const QString courseName) const;
    double getFailedCoursesCount() const;
    double getCredit() const;


};

#endif // STUDENT_H
