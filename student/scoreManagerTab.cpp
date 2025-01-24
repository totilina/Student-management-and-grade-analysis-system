#include "mainwindow.h"


void MainWindow::createdScoreManagerTab(){
    scoreManagerTab = new QWidget();

    mainTabWidget->addTab(scoreManagerTab,"成绩管理");
}
