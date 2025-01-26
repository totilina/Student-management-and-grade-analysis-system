#include "mainwindow.h"
#include <QMessageBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStringList>
#include <QModelIndex>
/* @author totilina */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    studentmanager = new StudentManager(); //初始化学生管理器
    // 加载数据
    if(!studentmanager->loadData()){
        studentmanager->dataImportSucceeded = false;
        QMessageBox::warning(this, "警告", "数据导入失败,本次对数据信息的修改将不会保存");
    }
    // 初始化筛选数据
    fileredStudents = studentmanager->students;
    // 创建界面
    stepUi();
}

MainWindow::~MainWindow() {
    // 保存数据
    studentmanager->saveData();

    delete studentmanager;
}

void MainWindow::stepUi(){
    // 设置窗口参数
    setWindowTitle("成绩分析");
    resize(900,675);

    //建立主界面
    mainTabWidget = new QTabWidget();
    setCentralWidget(mainTabWidget);

    //创建三个界面
    createdScoreManagerTab();   // 成绩管理/导出界面
    createdScoreAnalysisTab();  // 成绩分析界面
    createdStudentTab();        // 学生管理界面
}



