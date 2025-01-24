#include "mainwindow.h"
#include <QMessageBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStringList>
#include <QModelIndex>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    studentmanager = new StudentManager();
    if(!studentmanager->loadData()){
        studentmanager->dataImportSucceeded = false;
        QMessageBox::warning(this, "警告", "数据导入失败,本次对数据信息的修改将不会保存");
    }
    stepUi();
}

MainWindow::~MainWindow() {
    studentmanager->saveData();
    delete studentmanager;
}

void MainWindow::stepUi(){
    setWindowTitle("成绩分析");
    resize(900,675);

    mainTabWidget = new QTabWidget();
    setCentralWidget(mainTabWidget);
    createdScoreAnalysisTab();
    createdScoreManagerTab();
    createdStudentTab();
}



