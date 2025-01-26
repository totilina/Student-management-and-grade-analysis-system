#include "mainwindow.h"
#include <QStringList>
#include <QHeaderView>
#include <algorithm>
#include <QTableView>

void MainWindow::createdScoreManagerTab(){
    scoreManagerTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    mainTabWidget->addTab(scoreManagerTab,"成绩管理/导出");

    QGroupBox *filteredGroupBox = new QGroupBox("筛选信息");
    QHBoxLayout *ComboxLayout = new QHBoxLayout();

    courseComboBox = new QComboBox(this);
    courseComboBox->setEditable(true);
    courseComboBox->addItem("加权平均");
    courseComboBox->setPlaceholderText("选择比较成绩");

    majorComboBox = new QComboBox(this);
    majorComboBox->setEditable(true);
    majorComboBox->addItem("所有专业");
    majorComboBox->setPlaceholderText("选择专业");

    gradeComboBox = new QComboBox(this);
    gradeComboBox->setEditable(true);
    gradeComboBox->addItem("所有年级");
    gradeComboBox->setPlaceholderText("选择年级");

    classComboBox = new QComboBox(this);
    classComboBox->setEditable(true);
    classComboBox->addItem("所有班级");
    classComboBox->setPlaceholderText("选择班级");


    ComboxLayout->addWidget(new QLabel("成绩:"));
    ComboxLayout->addWidget(courseComboBox);
    ComboxLayout->addWidget(new QLabel("专业:"));
    ComboxLayout->addWidget(majorComboBox);
    ComboxLayout->addWidget(new QLabel("年级:"));
    ComboxLayout->addWidget(gradeComboBox);
    ComboxLayout->addWidget(new QLabel("班级:"));
    ComboxLayout->addWidget(classComboBox);

    filteredGroupBox->setLayout(ComboxLayout);
    layout->addWidget(filteredGroupBox);

    QGroupBox *buttonGroupBox = new QGroupBox("操作");
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *addCourseButton = new QPushButton("添加课程（待实现）");
    QPushButton *ascendingSortButton = new QPushButton("按成绩升序排序");
    QPushButton *descendingSortButton = new QPushButton("按成绩降序排序");
    QPushButton *exportCsvButton = new QPushButton("将当前表格导出为CSV文件");
    QPushButton *batchExportButton = new QPushButton("批量导出数据");

    buttonLayout->addWidget(addCourseButton);
    buttonLayout->addWidget(ascendingSortButton);
    buttonLayout->addWidget(descendingSortButton);
    buttonLayout->addWidget(exportCsvButton);
    buttonLayout->addWidget(batchExportButton);
    buttonGroupBox->setLayout(buttonLayout);
    layout->addWidget(buttonGroupBox);

    scoresTreeView = new QTreeView();
    scoresTreeView->setAnimated(true);
    // scoresTreeView->setSortingEnabled(true);
    scoresTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers); //将表格设为不可编辑
    scoresModel = new QStandardItemModel();
    scoresTreeView->setModel(scoresModel);

    QVector<Student> stus = studentmanager->students;

    std::sort(stus.begin(), stus.end(), [](const Student a, const Student b){
        return a.getAverageScore() >b.getAverageScore();
    });

    layout->addWidget(scoresTreeView);
    updateFileredOptios(studentmanager->students);


    connect(courseComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onFilterChanged);
    connect(majorComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onFilterChanged);
    connect(gradeComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onFilterChanged);
    connect(classComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onFilterChanged);

    connect(ascendingSortButton, &QPushButton::clicked, this, &MainWindow::ascendingSort);
    connect(descendingSortButton, &QPushButton::clicked, this, &MainWindow::descendingSort);
    connect(exportCsvButton, &QPushButton::clicked, this, [this](){
        if(exportCsvFile(fileredStudents , courseComboBox->currentText())){
            QMessageBox::information(this, "成功", "CSV文件导出成功");
        }else{
            QMessageBox::warning(this, "错误", "导出失败");
        }
    });
    connect(batchExportButton, &QPushButton::clicked, this, &MainWindow::batchExportData);



    refreshScoresTree(stus);

    scoreManagerTab->setLayout(layout);
}

void MainWindow::refreshScoresTree(const QVector<Student> &students){
    qDebug()<<"开始刷新表格";
    scoresModel->clear();

    scoresModel->setHorizontalHeaderLabels(
        QStringList()<< "学号" << "姓名" << "专业" << "年级" << "班级"<<courseComboBox->currentText()+"成绩"
        );
    for(const Student& student : students){
        QList<QStandardItem*> studentRow;
        studentRow << new QStandardItem(student.id)
                   << new QStandardItem(student.name)
                   << new QStandardItem(student.major)
                   << new QStandardItem(student.grade)
                   << new QStandardItem(student.className)
                   << new QStandardItem(QString::number(student.getCourseScoreByName(courseComboBox->currentText()), 'f', 2));


        scoresModel->appendRow(studentRow);
    }


    // studentTreeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    // studentTreeView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    qDebug()<<"刷新表格完成";
}

void MainWindow::updateFileredOptios(const QVector<Student> &students){
    courseComboBox->clear();
    majorComboBox->clear();
    gradeComboBox->clear();
    classComboBox->clear();

    courseComboBox->addItem("加权平均");
    majorComboBox->addItem("所有专业");
    gradeComboBox->addItem("所有年级");
    classComboBox->addItem("所有班级");

    courseComboBox->setCurrentText("加权平均");
    majorComboBox->setCurrentText("所有专业");
    gradeComboBox->setCurrentText("所有年级");
    classComboBox->setCurrentText("所有班级");


    for(const Course& cour : studentmanager->courses){
        if(!courses.contains(cour.name)){
            courses.append(cour.name);
        }
        courseComboBox->addItem(cour.name);
    }
    for(const Student& stu : students){
        if(!majors.contains(stu.major)){
            majors.append(stu.major);
        }
        if(!grades.contains(stu.grade)){
            grades.append(stu.grade);
        }
        if(!classes.contains(stu.className)){
            classes.append(stu.className);
        }

    }

    for(const QString& major : majors){
        majorComboBox->addItem(major);
    }

    for(const QString& grade : grades){
        gradeComboBox->addItem(grade);
    }
    for(const QString& className : classes){
        classComboBox->addItem(className);
    }
}


void MainWindow::onFilterChanged(){
    fileredStudents.clear();
    QString selectedCourse = courseComboBox->currentText();
    QString selectedMajor = majorComboBox->currentText();
    QString selectedGrade = gradeComboBox->currentText();
    QString selectedClass = classComboBox->currentText();

    for(const Student& stu : studentmanager->students){
        // bool courseMatch = ((selectedCourse == "加权平均成绩")||(!studentmanager->findCourseByName(selectedCourse).name.isEmpty()));
        bool majorMatch = ((selectedMajor == "所有专业")||(stu.major == selectedMajor));
        bool gradeMatch = ((selectedGrade == "所有年级")||(stu.grade == selectedGrade));
        bool classMatch = ((selectedClass == "所有班级")||(stu.className == selectedClass));
        qDebug()<<majorMatch<<" "<<gradeMatch<<" "<<classMatch;
        if(majorMatch && gradeMatch && classMatch){
            fileredStudents.append(stu);
        }

    }
    std::sort(fileredStudents.begin(),fileredStudents.end(),[selectedCourse](const Student& a, const Student& b){
        return a.getCourseScoreByName(selectedCourse) > b.getCourseScoreByName(selectedCourse);
    });
    // updateFileredOptios(fileredStudents);
    refreshScoresTree(fileredStudents);
    // qDebug()<<fileredStudents.first().name;
}

void MainWindow::ascendingSort() {
    QString selectedCourse = courseComboBox->currentText();
    std::sort(fileredStudents.begin(),fileredStudents.end(),[selectedCourse](const Student& a, const Student& b){
        return a.getCourseScoreByName(selectedCourse) < b.getCourseScoreByName(selectedCourse);
    });
    refreshScoresTree(fileredStudents);
}

void MainWindow::descendingSort() {
    QString selectedCourse = courseComboBox->currentText();
    std::sort(fileredStudents.begin(),fileredStudents.end(),[selectedCourse](const Student& a, const Student& b){
        return a.getCourseScoreByName(selectedCourse) > b.getCourseScoreByName(selectedCourse);
    });
    refreshScoresTree(fileredStudents);
}


