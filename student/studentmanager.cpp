#include "studentmanager.h"
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
/* @author totilina */

StudentManager::StudentManager(){
    dataFilePath = "data.json";
}

StudentManager::~StudentManager(){

}


bool StudentManager::loadData(){
    QFile file(dataFilePath);
    if(!file.exists()){
        saveData();
        return loadData();
    }
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<"无法打开数据文件"<<dataFilePath;
        dataImportSucceeded = false;
        return false;
    }
    QJsonDocument doc =QJsonDocument::fromJson(file.readAll());
    if(doc.isNull()){
        qDebug() << "JSon 解析失败";
        dataImportSucceeded = false;
        return false;
    }

    QJsonObject root = doc.object();

    QJsonArray coursesJson = root["课程信息"].toArray();

    QJsonArray studentsJson = root["学生信息"].toArray();
    //导入课程信息
    for(const auto& courJson : coursesJson){
        QJsonObject json = courJson.toObject();
        Course cour;
        cour.id = json["课程代码"].toString();
        cour.name = json["课程名称"].toString();
        cour.credit = json["课程学分"].toDouble();
        cour.semester = json["修读学期"].toString();
        this->courses.append(cour);
    }

    //导入学生信息
    for(const auto& stuJson : studentsJson){
        QJsonObject json = stuJson.toObject();
        Student stu(json["学号"].toString(),
                    json["姓名"].toString(),
                    json["专业"].toString(),
                    json["年级"].toString(),
                    json["班级"].toString()
        );
        QJsonObject scores = json["各科分数"].toObject();
        for(auto it = scores.begin(); it != scores.end(); ++it){
            Course cour = findCourseByName(it.key());
            if(cour.id.isEmpty()){

            }
            cour.score = it.value().toDouble();
            stu.scores.append(cour);
        }
        students.append(stu);
    }
    file.close();

    return true;
}

bool StudentManager::saveData(){
    if(!dataImportSucceeded){
        qDebug()<<"数据导入失败，不保存本次对数据信息的修改";
        return false;
    }
    QJsonObject root;

    // 数据信息
    QJsonObject metadata;
    metadata["version"] = "1.0";
    metadata["lastModified"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    metadata["school"] = "计算机学院";

    // 保存课程信息
    QJsonArray coursesJson;

    for(const Course& cour : courses){
        QJsonObject json;
        json["课程代码"] = cour.id;
        json["课程名称"] = cour.name;
        json["课程学分"] = cour.credit;
        json["修读学期"] = cour.semester;
        coursesJson.append(json);
    }

    // 保存学生信息
    QJsonArray studentsJson;
    for(const Student& stu : students){
        QJsonObject json;
        json["学号"] = stu.id;
        json["姓名"] = stu.name;
        json["专业"] = stu.major;
        json["年级"] = stu.grade;
        json["班级"] = stu.className;
        QJsonObject scores;
        for(auto it = stu.scores.begin(); it != stu.scores.end(); ++it){
            scores.insert(it->name, it->score);
        }
        json["各科分数"] = scores;
        studentsJson.append(json);
    }

    root["a_metadata"] = metadata;
    root["课程信息"] = coursesJson;
    root["学生信息"] = studentsJson;

    // 写入文件
    QJsonDocument doc(root);
    QFile file(dataFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    file.write(doc.toJson(QJsonDocument::Indented));

    return true;
}

Course StudentManager::findCourseByName(const QString& courseName) const{
    for(const Course& cour : courses){
        if(cour.name == courseName){
            return cour;
        }
    }
    return Course();
}

bool StudentManager::addStudent(const Student& student){
    if(findStudentById(student.id).id.isEmpty()) students.append(student);
    else return false;
    saveData();
    return true;
}

Student StudentManager::findStudentById(const QString& studentId) const{
    for(const Student &stu : students){
        if(stu.id == studentId){
            return stu;
        }
    }
    return Student();
}

QVector<Student> StudentManager::findStudentsByName(const QString& studentName) const{
    QVector<Student> stus;
    for(const Student &stu : students){
        if(stu.name == studentName){
            stus.append(stu);
        }
    }
    return stus;
}

QVector<Student> StudentManager::findStudentsByMajor(const QString& major) const{
    QVector<Student> stus;
    for(const Student &stu : students){
        if(stu.major == major){
            stus.append(stu);
        }
    }
    return stus;
}

QVector<Student> StudentManager::findStudentsByGrade(const QString& grade) const{
    QVector<Student> stus;
    for(const Student &stu : students){
        if(stu.grade == grade){
            stus.append(stu);
        }
    }
    return stus;
}

QVector<Student> StudentManager::findStudentsByClass(const QString& className) const{
    QVector<Student> stus;
    for(const Student &stu : students){
        if(stu.className == className){
            stus.append(stu);
        }
    }
    return stus;
}

bool StudentManager::updateStudent(const Student& oldStudent, const Student& newStudent){
    for(QVector<Student>::Iterator it = students.begin(); it != students.end(); ++it){
        if(it->id == oldStudent.id){
            *it = newStudent;
            return true;
        }
    }
    return false;
}

bool StudentManager::deleteStudent(const Student& student){
    for(QVector<Student>::Iterator it = students.begin(); it != students.end(); ++it){
        if(it->id == student.id){
            students.erase(it); // 一个无关要紧的警告，不用在意
            return true;
        }
    }
    return false;
}
