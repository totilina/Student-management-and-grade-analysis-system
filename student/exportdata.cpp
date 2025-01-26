#include "mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QStringConverter>
#include <QCheckBox>
#include <QDir>
#include <QStandardPaths>
/* @author totilina */

bool MainWindow::exportCsvFile(const QVector<Student>& students,const QString& courseName, QString fileName){
    // 如果 fileName 为空，弹出文件对话框
    if (fileName.isEmpty()) {
        // 获取当前目录的 exportData 文件夹路径
        QDir currentDir = QDir::current(); // 获取当前目录
        QString dataDirPath = currentDir.filePath("exportData"); // 当前目录下的 data 文件夹

        // 如果 exportData 文件夹不存在，则创建
        if (!QDir(dataDirPath).exists()) {
            if (!QDir().mkdir(dataDirPath)) {
                QMessageBox::warning(this, "错误", "无法创建 exportData 文件夹");
                return false;
            }
        }

        // 设置默认路径为 exportData 文件夹
        QString defaultFileName = QDir(dataDirPath).filePath(courseName + ".csv");

        // 弹出文件对话框，默认路径为 exportData 文件夹
        fileName = QFileDialog::getSaveFileName(this, "导出为CSV文件", defaultFileName, "CSV文件(*.csv)");
    }

    // 如果用户取消了对话框，返回 false
    if (fileName.isEmpty()) {
        return false;
    }

    // 确保文件名以 .csv 结尾
    if (!fileName.endsWith(".csv", Qt::CaseInsensitive)) {
        fileName += ".csv";
    }

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        // QMessageBox::warning(this, "错误", "无法打开文件进行写入");
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Encoding::System);
    out.setGenerateByteOrderMark(false);

    out << "学号,姓名,专业,年级,班级," << courseName << "成绩\n";

    for(const Student& stu : students){
        out<< stu.id <<","
            << stu.name <<","
            << stu.major <<","
            << stu.grade <<","
            << stu.className <<","
            << QString::number(stu.getCourseScoreByName(courseName), 'f', 2) <<"\n";

    }
    file.close();
    return true;
}

bool MainWindow::exportCsvFile(const QVector<Student>& students,const QVector<QString>& courseNames, QString fileName){
    // 如果用户取消了对话框，返回 false
    if (fileName.isEmpty()) {
        return false;
    }

    // 确保文件名以 .csv 结尾
    if (!fileName.endsWith(".csv", Qt::CaseInsensitive)) {
        fileName += ".csv";
    }

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        // QMessageBox::warning(this, "错误", "无法打开文件进行写入");
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Encoding::System);  // 设置保存文件编码为系统默认文件编码
    out.setGenerateByteOrderMark(false);

    out << "学号,姓名,专业,年级,班级,";

    for(int i = 0 ; i < courseNames.size(); ++i){
        out << courseNames[i] << "成绩";
        if(i == courseNames.size() - 1){
            out << "\n";
        }else{
            out << ",";
        }
    }

    for(const Student& stu : students){
        out << stu.id <<","
            << stu.name <<","
            << stu.major <<","
            << stu.grade <<","
            << stu.className <<",";
        for(int i = 0 ; i < courseNames.size(); ++i){
            out << QString::number(stu.getCourseScoreByName(courseNames[i]), 'f', 2);
            if(i == courseNames.size() - 1){
                out << "\n";
            }else{
                out << ",";
            }
        }
    }
    file.close();
    return true;
}

void MainWindow::batchExportData(){
    QDialog batchExportDialog(this);
    batchExportDialog.setWindowTitle("批量导出");

    QVBoxLayout *layout = new QVBoxLayout(&batchExportDialog);

    QGroupBox *firstGroupBox = new QGroupBox("多文件单一成绩导出（一个文件只有一个成绩数据）");
    QGridLayout *firstLayout = new QGridLayout();
    firstGroupBox->setLayout(firstLayout);
    QVector<QCheckBox *> firstCheckBoxs(courses.size()+1);

    QGroupBox *secondGroupBox = new QGroupBox("多成绩单一文件导出（一个文件有多个成绩数据）");
    QGridLayout *secondLayout = new QGridLayout();
    secondGroupBox->setLayout(secondLayout);
    QVector<QCheckBox *> secondCheckBoxs(courses.size()+1);


    QGroupBox *thirdGroupBox = new QGroupBox("选择专业");
    QGridLayout *thirdLayout = new QGridLayout();
    thirdGroupBox->setLayout(thirdLayout);
    QVector<QCheckBox *> thirdCheckBoxs(majors.size()+1);

    QGroupBox *fourGroupBox = new QGroupBox("选择年级");
    QGridLayout *fourLayout = new QGridLayout();
    fourGroupBox->setLayout(fourLayout);
    QVector<QCheckBox *> fourCheckBoxs(grades.size()+1);

    QGroupBox *fiveGroupBox = new QGroupBox("选择导出某班级全部学生");
    QGridLayout *fiveLayout = new QGridLayout();
    fiveGroupBox->setLayout(fiveLayout);
    QVector<QCheckBox *> fiveCheckBoxs(classes.size()+1);

    QGroupBox *sixGroupBox = new QGroupBox("选择导出某专业某年级全部学生");
    QGridLayout *sixLayout = new QGridLayout();
    sixGroupBox->setLayout(sixLayout);
    sixLayout->addWidget(thirdGroupBox);
    sixLayout->addWidget(fourGroupBox);

    firstCheckBoxs[0] = new QCheckBox("加权平均");
    secondCheckBoxs[0] = new QCheckBox("加权平均");
    thirdCheckBoxs[0] = new QCheckBox("所有专业");
    fourCheckBoxs[0] = new QCheckBox("所有年级");
    fiveCheckBoxs[0] = new QCheckBox("所有班级");

    firstLayout->addWidget(firstCheckBoxs[0], 0, 0);
    secondLayout->addWidget(secondCheckBoxs[0], 0, 0);
    thirdLayout->addWidget(thirdCheckBoxs[0], 0, 0);
    fourLayout->addWidget(fourCheckBoxs[0], 0, 0);
    fiveLayout->addWidget(fiveCheckBoxs[0], 0, 0);


    for(int i = 0; i < courses.size(); ++i){
        int row = (i + 4) / 4;
        int col = (i + 4) % 4;
        firstCheckBoxs[i+1] = new QCheckBox(courses[i]);
        secondCheckBoxs[i+1] = new QCheckBox(courses[i]);

        firstLayout->addWidget(firstCheckBoxs[i+1], row, col);
        secondLayout->addWidget(secondCheckBoxs[i+1], row, col);
    }
    for(int i = 0; i < majors.size(); ++i){
        int row = (i + 4) / 4;
        int col = (i + 4) % 4;
        thirdCheckBoxs[i+1] = new QCheckBox(majors[i]);
        thirdLayout->addWidget(thirdCheckBoxs[i+1], row, col);
    }


    for(int i = 0; i < grades.size(); ++i){
        int row = (i + 4) / 4;
        int col = (i + 4) % 4;
        fourCheckBoxs[i+1] = new QCheckBox(grades[i]);
        fourLayout->addWidget(fourCheckBoxs[i+1], row, col);
    }

    for(int i = 0; i < classes.size(); ++i){
        int row = (i + 9) / 9;
        int col = (i + 9) % 9;
        fiveCheckBoxs[i+1] = new QCheckBox(classes[i]);
        fiveLayout->addWidget(fiveCheckBoxs[i+1], row, col);
    }

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *exportButton = new QPushButton("导出", &batchExportDialog);
    QPushButton *cancelButton = new QPushButton("取消", &batchExportDialog);

    buttonLayout->addWidget(exportButton);
    buttonLayout->addWidget(cancelButton);


    connect(exportButton, &QPushButton::clicked, &batchExportDialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &batchExportDialog, &QDialog::reject);


    layout->addWidget(firstGroupBox);
    layout->addWidget(secondGroupBox);
    // layout->addWidget(thirdGroupBox);
    // layout->addWidget(fourGroupBox);
    layout->addWidget(sixGroupBox);
    layout->addWidget(fiveGroupBox);

    layout->addLayout(buttonLayout);

    if(batchExportDialog.exec() == QDialog::Accepted){
        QVector<QVector<Student>> batchData;
        QVector<QString> fileNames;

        // 获取第一个课程框选中的课程
        QStringList selectedFirstCourses;
        for (QCheckBox *checkBox : firstCheckBoxs) {
            if (checkBox && checkBox->isChecked()) {
                selectedFirstCourses.append(checkBox->text());
            }
        }

        // 获取第二个课程框选中的课程
        QStringList selectedSecondCourses;
        for (QCheckBox *checkBox : secondCheckBoxs) {
            if (checkBox && checkBox->isChecked()) {
                selectedSecondCourses.append(checkBox->text());
            }
        }

        QVector<QVector<Student>> majorsStudents;
        StudentManager temp;

        bool allmajor = false;

        // 获取选中的专业
        QStringList selectedMajors;
        for (QCheckBox *checkBox : thirdCheckBoxs) {
            if (checkBox && checkBox->isChecked()) {
                if(checkBox->text() == "所有专业"){
                    majorsStudents.append(studentmanager->students);
                    allmajor = true;
                }else {
                    QVector<Student> majorsStudent = studentmanager->findStudentsByMajor(checkBox->text());
                    majorsStudents.append(majorsStudent);
                }
            }
        }

        // 获取选中的年级
        QStringList selectedGrades;
        for (QCheckBox *checkBox : fourCheckBoxs) {
            if (checkBox && checkBox->isChecked()) {
                selectedGrades.append(checkBox->text());

                for(const QVector<Student>& students : majorsStudents){
                    QVector<Student> stus;

                    for(const Student &stu : students){
                        if(stu.grade == checkBox->text() || checkBox->text() == "所有年级"){
                            stus.append(stu);
                        }
                    }

                    if(!stus.isEmpty()) {
                        batchData.append(stus);
                        if(allmajor) {
                            fileNames.append("所有专业" + checkBox->text());
                        }else {
                            fileNames.append(stus.first().major + checkBox->text());
                        }
                    }
                }
            }
        }

        // 获取选中的班级
        QStringList selectedClasses;
        for (QCheckBox *checkBox : fiveCheckBoxs) {
            if (checkBox && checkBox->isChecked()) {
                selectedClasses.append(checkBox->text());
                if(checkBox->text() == "所有班级"){
                    batchData.append(studentmanager->students);
                    fileNames.append("所有班级");
                }else{
                    batchData.append(studentmanager->findStudentsByClass(checkBox->text()));
                    fileNames.append(checkBox->text());
                }
            }
        }
        // 输出选中的内容 （调试用后续可以删去，包括其用到的变量）
        qDebug() << "选中第一个框的课程:" << selectedFirstCourses;
        qDebug() << "选中第二个框的课程:" << selectedSecondCourses;
        qDebug() << "选中的专业:" << selectedMajors;
        qDebug() << "选中的年级:" << selectedGrades;
        qDebug() << "选中的班级:" << selectedClasses;

        // 用户选择路径
        QDir currentDir = QDir::current(); // 获取当前目录
        QString dataDirPath = currentDir.filePath("exportData"); // 当前目录下的 exportData 文件夹
        // 如果 exportData 文件夹不存在，则创建
        if (!QDir(dataDirPath).exists()) {
            if (!QDir().mkdir(dataDirPath)) {
                QMessageBox::warning(this, "错误", "无法创建 exportData 文件夹");
                return ;
            }
        }
        QString dirPath = QFileDialog::getExistingDirectory(this, "选择保存路径", dataDirPath);



        // 如果用户取消了对话框，返回
        if (dirPath.isEmpty()) {
            return ;
        }

        bool e = true; // 判断是否成功导出全部数据文件的变量
        for(int i = 0; i < batchData.size() ; ++i){
            QString fileName = QDir(dirPath).filePath(fileNames[i] + "_混合成绩.csv");
            if(!exportCsvFile(batchData[i], selectedFirstCourses, fileName)){
                e = false;
                QMessageBox::warning(this, "错误", "文件" + fileNames[i] + "_混合成绩.csv" + "导出失败");
            }
        }


        for (const QString& courseName: selectedSecondCourses){
            for(int i = 0; i < batchData.size() ; ++i){
                QString fileName = QDir(dirPath).filePath(fileNames[i] + "_" + courseName + ".csv");
                if(!exportCsvFile(batchData[i], courseName, fileName)){
                    e = false;
                    QMessageBox::warning(this, "错误", "文件" + fileNames[i] + "_" + courseName + ".csv" + "导出失败");
                }
            }
        }
        if(e) QMessageBox::information(this, "成功", "全部文件导出成功");

    }
}

/* 废弃代码/被优化的代码

// void MainWindow::exportCsvFile(){
//     QString fileName = QFileDialog::getSaveFileName(this, "导出为CSV文件", "", "CSV文件(*.csv)");
//     if(fileName.isEmpty()){
//         return ;
//     }

//     QFile file(fileName);
//     if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
//         QMessageBox::warning(this, "错误", "无法打开文件进行写入");
//         return ;
//     }

//     QTextStream out(&file);
//     out.setEncoding(QStringConverter::Encoding::System);
//     out.setGenerateByteOrderMark(false);

//     // auto writeLine =

//     // out << "学号,姓名,专业,年级,班级," << courseComboBox->currentText() << "成绩\n";
//     for(int col = 0; col <scoresModel->columnCount(); ++col){
//         out << scoresModel->headerData(col, Qt::Horizontal).toString();
//         if(col < scoresModel->columnCount() - 1){
//             out << ",";
//         }
//     }
//     out << "\n";

//     for(int row = 0; row < scoresModel->rowCount(); ++row){
//         for(int col = 0; col < scoresModel->columnCount(); ++col){
//             QStandardItem *item = scoresModel->item(row, col);
//             if(item){
//                 out << item->text();
//             }
//             if(col < scoresModel->columnCount() - 1){
//                 out << ",";
//             }
//         }
//         out << "\n";
//     }
//     file.close();
//     QMessageBox::information(this, "成功", "CSV文件导出成功");
// }

废弃代码
*/
