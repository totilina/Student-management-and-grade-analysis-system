#include "mainwindow.h"
#include <QTableView>
/* @author totilina */

void MainWindow::createdScoreAnalysisTab(){
    scoreAnalysisTab = new QWidget();

    QVBoxLayout *layout = new QVBoxLayout();
    scoreAnalysisTab->setLayout(layout);
    mainTabWidget->addTab(scoreAnalysisTab,"成绩分析");


    QGroupBox *filteredGroupBox = new QGroupBox("筛选信息");
    QHBoxLayout *ComboxLayout = new QHBoxLayout();

    analysisCourseComboBox = new QComboBox(this);
    analysisCourseComboBox->setEditable(true);
    analysisCourseComboBox->addItem("加权平均");
    analysisCourseComboBox->setPlaceholderText("选择比较成绩");

    analysisMajorComboBox = new QComboBox(this);
    analysisMajorComboBox->setEditable(true);
    analysisMajorComboBox->addItem("所有专业");
    analysisMajorComboBox->setPlaceholderText("选择专业");

    analysisGradeComboBox = new QComboBox(this);
    analysisGradeComboBox->setEditable(true);
    analysisGradeComboBox->addItem("所有年级");
    analysisGradeComboBox->setPlaceholderText("选择年级");

    analysisClassComboBox = new QComboBox(this);
    analysisClassComboBox->setEditable(true);
    analysisClassComboBox->addItem("所有班级");
    analysisClassComboBox->setPlaceholderText("选择班级");

    for(const QString& course : courses){
        analysisCourseComboBox->addItem(course);
    }

    for(const QString& major : majors){
        analysisMajorComboBox->addItem(major);
    }

    for(const QString& grade : grades){
        analysisGradeComboBox->addItem(grade);
    }
    for(const QString& className : classes){
        analysisClassComboBox->addItem(className);
    }


    ComboxLayout->addWidget(new QLabel("成绩:"));
    ComboxLayout->addWidget(analysisCourseComboBox);
    ComboxLayout->addWidget(new QLabel("专业:"));
    ComboxLayout->addWidget(analysisMajorComboBox);
    ComboxLayout->addWidget(new QLabel("年级:"));
    ComboxLayout->addWidget(analysisGradeComboBox);
    ComboxLayout->addWidget(new QLabel("班级:"));
    ComboxLayout->addWidget(analysisClassComboBox);

    filteredGroupBox->setLayout(ComboxLayout);
    layout->addWidget(filteredGroupBox);

    QGroupBox *gradeDistGroup = new QGroupBox("成绩分布");
    QVBoxLayout *gradeDistLayout = new QVBoxLayout();

    analysisTableView = new QTableView();
    analysisModel = new QStandardItemModel(this);
    analysisTableView->setModel(analysisModel);
    analysisModel->setHorizontalHeaderLabels(
        QStringList() << "课程" << "平均分"  << "最高分" << "最低分"
                      << "总人数" << "不及格人数" << "60~69分人数"
                      << "70~79分人数" << "80~89分人数" << "90分以上人数"
    );

    gradeDistLayout->addWidget(analysisTableView);
    gradeDistGroup->setLayout(gradeDistLayout);
    layout->addWidget(gradeDistGroup);

    connect(analysisCourseComboBox, &QComboBox::currentTextChanged, this, &MainWindow::analysisFilterChanged);
    connect(analysisMajorComboBox, &QComboBox::currentTextChanged, this, &MainWindow::analysisFilterChanged);
    connect(analysisGradeComboBox, &QComboBox::currentTextChanged, this, &MainWindow::analysisFilterChanged);
    connect(analysisClassComboBox, &QComboBox::currentTextChanged, this, &MainWindow::analysisFilterChanged);

    refreshAnalysisTableView(fileredStudents);


}

void MainWindow::refreshAnalysisTableView(const QVector<Student>& students){
    analysisModel->clear();
    analysisModel->setHorizontalHeaderLabels(
        QStringList() << "课程" << "平均分"  << "最高分" << "最低分"
                      << "总人数" << "不及格人数" << "60~69分人数"
                      << "70~79分人数" << "80~89分人数" << "90分以上人数"
        );
    for(const QString& course : courses){
        QVector<double> courseScores;
        for (const Student& student : students){
            courseScores.append(student.getCourseScoreByName(course));
        }
        if(!courseScores.isEmpty()){
            double scoreSum = 0;
            double scoreMax = courseScores[0];
            double scoreMin = courseScores[0];

            int sumCount = courseScores.size();
            int filedCount = 0;
            int score60to69Count = 0;
            int score70to79Count = 0;
            int score80to89Count = 0;
            int score90PlusCount = 0;

            for(const double& score : courseScores){
                scoreSum += score;
                scoreMax = qMax(scoreMax, score);
                scoreMin = qMin(scoreMin, score);
                if(score < 60) filedCount++;
                if(score >= 60 && score < 70) score60to69Count++;
                if(score >= 70 && score < 80) score70to79Count++;
                if(score >= 80 && score < 90) score80to89Count++;
                if(score >= 90 && score < 101) score90PlusCount++;
            }
            double avg = scoreSum / sumCount;

            QList<QStandardItem*> row;
            row << new QStandardItem(course)
                << new QStandardItem(QString::number(avg, 'f', 2))
                << new QStandardItem(QString::number(scoreMax, 'f', 1))
                << new QStandardItem(QString::number(scoreMin, 'f', 1))
                << new QStandardItem(QString::number(sumCount))
                << new QStandardItem(QString::number(filedCount))
                << new QStandardItem(QString::number(score60to69Count))
                << new QStandardItem(QString::number(score70to79Count))
                << new QStandardItem(QString::number(score80to89Count))
                << new QStandardItem(QString::number(score90PlusCount));
            analysisModel->appendRow(row);
        }
    }
}

void MainWindow::analysisFilterChanged(){
    fileredStudents.clear();
    // QString selectedCourse = analysisCourseComboBox->currentText();
    QString selectedMajor = analysisMajorComboBox->currentText();
    QString selectedGrade = analysisGradeComboBox->currentText();
    QString selectedClass = analysisClassComboBox->currentText();

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

    refreshAnalysisTableView(fileredStudents);
}
