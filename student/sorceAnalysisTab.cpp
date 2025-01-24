#include "mainwindow.h"
#include <QMessageBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStringList>
#include <QHeaderView>
#include <QComboBox>
#include <algorithm>
#include <QSet>
#include <QFile>
#include <QFileDialog>
#include <QStringConverter>

void MainWindow::createdScoreAnalysisTab(){
    scoreAnalysisTab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();

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
    ComboxLayout->addWidget(new QLabel("成绩:"));
    ComboxLayout->addWidget(classComboBox);

    filteredGroupBox->setLayout(ComboxLayout);
    layout->addWidget(filteredGroupBox);

    QGroupBox *buttonGroupBox = new QGroupBox("操作");
    QHBoxLayout *buttonLayout = new QHBoxLayout();


    QPushButton *exportCsvButton = new QPushButton("将当前表格导出为CSV文件");
    QPushButton *exportCsvsButton = new QPushButton("批量导出数据");

    buttonLayout->addWidget(exportCsvButton);
    buttonLayout->addWidget(exportCsvsButton);
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
    refreshScoresTree(stus);


    connect(courseComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onFilterChanged);
    connect(majorComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onFilterChanged);
    connect(gradeComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onFilterChanged);
    connect(classComboBox, &QComboBox::currentTextChanged, this, &MainWindow::onFilterChanged);

    connect(exportCsvButton, &QPushButton::clicked, this, &MainWindow::exportCsvFile);

    mainTabWidget->addTab(scoreAnalysisTab,"成绩分析");
    scoreAnalysisTab->setLayout(layout);
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
                   << new QStandardItem(QString::number(student.getSourseScoreByName(courseComboBox->currentText()), 'f', 2));


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

    QSet<QString> majors, grades, classes;
    for(const Course& cour : studentmanager->courses){
        courseComboBox->addItem(cour.name);
    }
    for(const Student& stu : students){
        majors.insert(stu.major);
        grades.insert(stu.grade);
        classes.insert(stu.className);
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
    QString selectedCourse = courseComboBox->currentText();
    QString selectedMajor = majorComboBox->currentText();
    QString selectedGrade = gradeComboBox->currentText();
    QString selectedClass = classComboBox->currentText();

    QVector<Student> fileredStudents;
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
        return a.getSourseScoreByName(selectedCourse) > b.getSourseScoreByName(selectedCourse);
    });
    refreshScoresTree(fileredStudents);
    // qDebug()<<fileredStudents.first().name;
}

void MainWindow::exportCsvFile(){
    QString fileName = QFileDialog::getSaveFileName(this, "导出为CSV文件", "", "CSV文件(*.csv)");
    if(fileName.isEmpty()){
        return ;
    }

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        QMessageBox::warning(this, "错误", "无法打开文件进行写入");
        return ;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Encoding::System);
    out.setGenerateByteOrderMark(false);

    // auto writeLine =

    // out << "学号,姓名,专业,年级,班级," << courseComboBox->currentText() << "成绩\n";
    for(int col = 0; col <scoresModel->columnCount(); ++col){
        out << scoresModel->headerData(col, Qt::Horizontal).toString();
        if(col < scoresModel->columnCount() - 1){
            out << ",";
        }
    }
    out << "\n";

    for(int row = 0; row < scoresModel->rowCount(); ++row){
        for(int col = 0; col < scoresModel->columnCount(); ++col){
            QStandardItem *item = scoresModel->item(row, col);
            if(item){
                out << item->text();
            }
            if(col < scoresModel->columnCount() - 1){
                out << ",";
            }
        }
        out << "\n";
    }
    file.close();
    QMessageBox::information(this, "成功", "CSV文件导出成功");
}
