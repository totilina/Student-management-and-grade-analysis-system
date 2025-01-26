#include "mainwindow.h"
#include <QStringList>
#include <QHeaderView>
/* @author totilina */

void MainWindow::createdStudentTab(){
    studentTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();

    mainTabWidget->addTab(studentTab, "学生管理");
    studentTab->setLayout(layout);

    QGroupBox *inputGroupBox = new QGroupBox("学生信息");
    QGridLayout *inputLayout = new QGridLayout();

    inputLayout->addWidget(new QLabel("学号"), 0, 0);
    idEdit = new QLineEdit();
    inputLayout->addWidget(idEdit, 0, 1);

    inputLayout->addWidget(new QLabel("姓名"), 0, 2);
    nameEdit = new QLineEdit();
    inputLayout->addWidget(nameEdit, 0, 3);

    inputLayout->addWidget(new QLabel("专业"), 1, 0);
    majorEdit = new QLineEdit();
    inputLayout->addWidget(majorEdit, 1, 1);

    inputLayout->addWidget(new QLabel("年级"), 1, 2);
    gradeEdit = new QLineEdit();
    inputLayout->addWidget(gradeEdit, 1, 3);

    inputLayout->addWidget(new QLabel("班级"), 2, 0);
    classEdit = new QLineEdit();
    inputLayout->addWidget(classEdit, 2, 1);

    inputLayout->addWidget(new QLabel("目前已修学分"), 2, 2);
    creditLabel = new QLabel("0.0");
    inputLayout->addWidget(creditLabel, 2, 3);

    inputLayout->addWidget(new QLabel("加权平均成绩"), 3, 0);
    averageScoreLabel = new QLabel("0.0");
    inputLayout->addWidget(averageScoreLabel, 3, 1);

    inputLayout->addWidget(new QLabel("不及格科目数"), 3, 2);
    failedCourseCountLabel = new QLabel("0");
    inputLayout->addWidget(failedCourseCountLabel, 3, 3);

    inputGroupBox->setLayout(inputLayout);

    layout->addWidget(inputGroupBox);

    QHBoxLayout *buttonLayout= new QHBoxLayout();
    addButton = new QPushButton("添加");
    searchButton = new QPushButton("查询");
    deleteButton = new QPushButton("删除");
    updateButton = new QPushButton("更新");
    clearButton = new QPushButton("清空/刷新");

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(searchButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(clearButton);
    layout->addLayout(buttonLayout);

    QHBoxLayout *expendLayout= new QHBoxLayout();
    QPushButton *expendAllButton = new QPushButton("展开全部");
    QPushButton *collapseAllButton = new QPushButton("收起全部");
    expendLayout->addStretch();
    expendLayout->addWidget(expendAllButton);
    expendLayout->addWidget(collapseAllButton);


    layout->addLayout(expendLayout);


    studentTreeView = new QTreeView();
    studentTreeView->setAnimated(true);
    studentTreeView->setSortingEnabled(true);
    studentTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers); //将表格设为不可编辑
    studentModel = new QStandardItemModel();
    studentTreeView->setModel(studentModel);



    studentTreeView->setStyleSheet(
        // 全局基础样式
        "QTreeView {"
        "   background-color: #FFFFFF;"           // 表格背景为白色
        "   alternate-background-color: #FAFAFA;" // 交替行颜色（更浅的灰色）
        "   border: none;"                        // 去除边框
        "}"

        // 默认项样式（子节点）
        "QTreeView::item {"
        "   color: #212121;"                     // 文字颜色（深灰色）
        "   background-color: #F5F5F5;"          // 子节点默认背景颜色（浅灰色）
        "   border-bottom: 1px solid #E0E0E0;"   // 底部边框（浅灰色）
        "   padding: 4px;"                       // 内边距
        "}"

        // 父节点样式
        "QTreeView::item:has-children {"
        "   background-color: #E3F2FD;"          // 父节点背景颜色（柔和的蓝色）
        "   color: black;"                       // 文字颜色（黑色）
        "   font-weight: bold;"                  // 父节点文字加粗
        "}"

        // 选中项样式（所有行通用）
        "QTreeView::item:selected {"
        "   background-color: #BBDEFB;"          // 选中背景颜色（深蓝色）
        "   color: black;"                       // 选中文字颜色（黑色）
        "   border: none;"                       // 选中边框
        "}"

        // 悬停效果
        "QTreeView::item:hover {"
        "   background-color: #BBDEFB;"          // 悬停背景颜色（浅蓝色）
        "}"
        );



    layout->addWidget(studentTreeView);
    refreshStudentTree(studentmanager->students);


    connect(addButton, &QPushButton::clicked, this, &MainWindow::addStudent);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchStudent);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteStudent);
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::updateStudent);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearInputEdits);
    connect(expendAllButton, &QPushButton::clicked, studentTreeView, &QTreeView::expandAll);
    connect(collapseAllButton, &QPushButton::clicked, studentTreeView, &QTreeView::collapseAll);

    connect(studentTreeView, &QTreeView::clicked, this, &MainWindow::onStudentSelected);

    // connect(studentModel, &QStandardItemModel::itemChanged, this, &MainWindow::treeDataChanged);



}


void MainWindow::addStudent(){
    if(idEdit->text().isEmpty() || nameEdit->text().isEmpty() ||
        majorEdit->text().isEmpty() || gradeEdit->text().isEmpty() ||
        classEdit->text().isEmpty()) {
        QMessageBox::warning(this, "警告", "请填写所有必要信息");
        return;
    }

    Student stu(idEdit->text(),
                nameEdit->text(),
                majorEdit->text(),
                gradeEdit->text(),
                classEdit->text());

    if(!studentmanager->addStudent(stu)){
        QMessageBox::warning(this, "警告", "学号重复，添加失败");
    }else {
        clearInputEdits();
        qDebug()<<"添加学生完成";
        return ;
    }
    refreshStudentTree(studentmanager->students);  // 刷新显示
    qDebug()<<"添加学生完成";
}

void MainWindow::updateStudent(){
    if(idEdit->text().isEmpty()){
        QMessageBox::warning(this, "警告", "请先填写学生学号");
        return ;
    }
    if(!studentmanager->findStudentById(idEdit->text()).id.isEmpty()){
        Student oldStu =  studentmanager->findStudentById(idEdit->text());

        if(idEdit->text().isEmpty() || nameEdit->text().isEmpty() ||
            majorEdit->text().isEmpty() || gradeEdit->text().isEmpty() ||
            classEdit->text().isEmpty()) {
            QMessageBox::warning(this, "警告", "请填写所有必要信息");
            return;
        }

        Student newStu(idEdit->text(),
                       nameEdit->text(),
                       majorEdit->text(),
                       gradeEdit->text(),
                       classEdit->text()
                       );
        newStu.scores = oldStu.scores;
        if(!studentmanager->updateStudent(oldStu,newStu)){
            QMessageBox::warning(this, "警告", "学生信息更新失败！");
        }
        QVector<Student> stus;
        stus.append(newStu);
        refreshStudentTree(stus);  // 刷新显示
        return ;
    }
    else{
        QMessageBox::information(this, "通知", "该学号不存在");
    }
}

void MainWindow::deleteStudent(){
    if(idEdit->text().isEmpty()){
        QMessageBox::warning(this, "警告", "请先填写学生学号");
        return ;
    }
    if(!studentmanager->findStudentById(idEdit->text()).id.isEmpty()){
        Student stu(idEdit->text(),
                    nameEdit->text(),
                    majorEdit->text(),
                    gradeEdit->text(),
                    classEdit->text());
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "问题",
                                      "您确定要删除这个学生吗？",
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                      QMessageBox::No);
        if(reply == QMessageBox::Yes){
            if(studentmanager->deleteStudent(stu)){
                clearInputEdits();
                refreshStudentTree(studentmanager->students);
            }
        }
    }else {
        QMessageBox::information(this, "通知", "该学号不存在");
    }
}

void MainWindow::searchStudent(){
    if(idEdit->text().isEmpty() &&
        nameEdit->text().isEmpty() &&
        majorEdit->text().isEmpty() &&
        gradeEdit->text().isEmpty() &&
        classEdit->text().isEmpty()
        ) {
        QMessageBox::warning(this, "警告", "请先填写必要信息");
        return ;
    }

    if(!idEdit->text().isEmpty()){
        Student stu =  studentmanager->findStudentById(idEdit->text());
        if(stu.id.isEmpty()){
            QMessageBox::information(this, "通知", "该学号不存在");
            return ;
        }
        nameEdit->setText(stu.name);
        majorEdit->setText(stu.major);
        gradeEdit->setText(stu.grade);
        classEdit->setText(stu.className);
        creditLabel->setText(QString::number(stu.getCredit(), 'f', 1));
        averageScoreLabel->setText(QString::number(stu.getAverageScore(), 'f', 1));
        failedCourseCountLabel->setText(QString::number(stu.getFailedCoursesCount()));
        QVector<Student> stus;
        stus.append(stu);
        refreshStudentTree(stus);
        return ;
    }

    if(!nameEdit->text().isEmpty()){
        QVector<Student> stus =  studentmanager->findStudentsByName(nameEdit->text());
        if(stus.isEmpty()){
            refreshStudentTree(stus);
            return ;
        }
        idEdit->setText(stus.first().id);
        majorEdit->setText(stus.first().major);
        gradeEdit->setText(stus.first().grade);
        classEdit->setText(stus.first().className);
        creditLabel->setText(QString::number(stus.first().getCredit(), 'f', 1));
        averageScoreLabel->setText(QString::number(stus.first().getAverageScore(), 'f', 1));
        failedCourseCountLabel->setText(QString::number(stus.constFirst().getFailedCoursesCount()));
        refreshStudentTree(stus);  // 刷新显示
        return ;
    }

    if(!majorEdit->text().isEmpty()){
        QVector<Student> stus =  studentmanager->findStudentsByMajor(majorEdit->text());
        refreshStudentTree(stus);
        return ;
    }

    if(!gradeEdit->text().isEmpty()){
        QVector<Student> stus =  studentmanager->findStudentsByGrade(gradeEdit->text());
        refreshStudentTree(stus);
        return ;
    }

    if(!classEdit->text().isEmpty()){
        QVector<Student> stus =  studentmanager->findStudentsByClass(classEdit->text());
        refreshStudentTree(stus);
        return ;
    }
}

void MainWindow::clearInputEdits(){
    qDebug()<<"开始清空文本";
    idEdit->clear();
    nameEdit->clear();
    majorEdit->clear();
    gradeEdit->clear();
    classEdit->clear();
    creditLabel->setText("0.0");
    averageScoreLabel->setText("0.0");
    failedCourseCountLabel->setText("0");
    qDebug()<<"清空文本完成";
    refreshStudentTree(studentmanager->students);
}

void MainWindow::refreshStudentTree(const QVector<Student> &students){
    qDebug()<<"开始刷新表格";
    studentModel->clear();

    //表头
    studentModel->setHorizontalHeaderLabels(
        QStringList()<< "学号" << "姓名" << "专业" << "年级" << "班级"<<"目前已修学分"<<"加权平均成绩"<<"不及格科目数"
    );
    for(const Student& student : students){
        QList<QStandardItem*> studentRow;
        studentRow << new QStandardItem(student.id)
                   << new QStandardItem(student.name)
                   << new QStandardItem(student.major)
                   << new QStandardItem(student.grade)
                   << new QStandardItem(student.className)
                   << new QStandardItem(QString::number(student.getCredit(), 'f', 1))
                   << new QStandardItem(QString::number(student.getAverageScore(), 'f', 2))
                   << new QStandardItem(QString::number(student.getFailedCoursesCount()));

        studentModel->appendRow(studentRow);

        // 成绩子表
        QStandardItem* gradesParent = studentRow[0];
        QList<QStandardItem*> headerRow;
        headerRow << new QStandardItem("")
                  << new QStandardItem("课程名称")
                  << new QStandardItem("修读学期")
                  << new QStandardItem("课程学分")
                  << new QStandardItem("成绩")
                  << new QStandardItem("");
        gradesParent->appendRow(headerRow);

        for (auto it = student.scores.begin(); it != student.scores.end(); ++it) {
            QList<QStandardItem*> gradeRow;
            gradeRow << new QStandardItem("")
                     << new QStandardItem(it->name)
                     << new QStandardItem(it->semester)
                     << new QStandardItem(QString::number(it->credit))
                     << new QStandardItem(QString::number(it->score))
                     << new QStandardItem("")
                     << new QStandardItem("");
            gradesParent->appendRow(gradeRow);
        }
    }
    studentTreeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    studentTreeView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    qDebug()<<"刷新表格完成";
}

void MainWindow::onStudentSelected(const QModelIndex &index){
    QModelIndex ind = index;
    if(ind.parent().isValid()){
        ind = index.parent();
    }
    idEdit->setText(studentModel->data(studentModel->index(ind.row(), 0)).toString());
    nameEdit->setText(studentModel->data(studentModel->index(ind.row(), 1)).toString());
    majorEdit->setText(studentModel->data(studentModel->index(ind.row(), 2)).toString());
    gradeEdit->setText(studentModel->data(studentModel->index(ind.row(), 3)).toString());
    classEdit->setText(studentModel->data(studentModel->index(ind.row(), 4)).toString());
    creditLabel->setText(studentModel->data(studentModel->index(ind.row(), 5)).toString());
    averageScoreLabel->setText(studentModel->data(studentModel->index(ind.row(), 6)).toString());
    failedCourseCountLabel->setText(studentModel->data(studentModel->index(ind.row(), 7)).toString());
}

/* 废弃代码

// 设置表格编辑
// void MainWindow::treeDataChanged(QStandardItem *item){
//     QModelIndex index = item->index();
//     QModelIndex parentIndex = index.parent();

//     if(parentIndex.isValid()){
//         QMessageBox::warning(this, "警告", "本界面不可以修改课程信息和学生成绩，如需修改请到成绩管理界面修改");
//     }
//     else{
//         QString studentId = studentModel->data(index.sibling(index.row(), 0)).toString();
//         Student oldStudent = studentmanager->findStudentById(studentId);
//         if(!oldStudent.id.isEmpty()){
//             Student newStudent(studentModel->data(studentModel->index(index.row(), 0)).toString(),
//                                studentModel->data(studentModel->index(index.row(), 1)).toString(),
//                                studentModel->data(studentModel->index(index.row(), 2)).toString(),
//                                studentModel->data(studentModel->index(index.row(), 3)).toString(),
//                                studentModel->data(studentModel->index(index.row(), 4)).toString()
//             );
//             newStudent.scores = oldStudent.scores;
//             if(!studentmanager->updateStudent(oldStudent, newStudent)){
//                 QMessageBox::warning(this, "警告", "更新失败");
//             }
//             refreshStudentTree(studentmanager->students);
//         }
//     }

// }

废弃代码
*/
