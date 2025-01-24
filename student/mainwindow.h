#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QComboBox>
#include "studentmanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:


private:
    void stepUi();

    StudentManager *studentmanager;

    // 学生管理界面
    void createdStudentTab();
    void refreshStudentTree(const QVector<Student> &students);
    void clearInputEdits();
    void addStudent();
    void updateStudent();
    void deleteStudent();
    void searchStudent();
    void onStudentSelected(const QModelIndex &index);
    // void treeDataChanged(QStandardItem *item);
    QWidget *studentTab;
    QTreeView *studentTreeView;
    QStandardItemModel *studentModel;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *searchButton;
    QPushButton *updateButton;
    QPushButton *clearButton;
    QLineEdit *idEdit;
    QLineEdit *nameEdit;
    QLineEdit *majorEdit;
    QLineEdit *gradeEdit;
    QLineEdit *classEdit;
    QLabel *creditLabel;
    QLabel *averageScoreLabel;
    QLabel *failedCourseCountLabel;

    // 成绩管理界面
    void createdScoreManagerTab();
    QWidget *scoreManagerTab;

    // 成绩分析界面
    void createdScoreAnalysisTab();
    void refreshScoresTree(const QVector<Student>& students);
    void onFilterChanged();
    void updateFileredOptios(const QVector<Student>& students);
    void exportCsvFile();
    QTreeView *scoresTreeView;
    QStandardItemModel *scoresModel;
    QComboBox *courseComboBox;
    QComboBox *majorComboBox;
    QComboBox *gradeComboBox;
    QComboBox *classComboBox;
    QWidget *scoreAnalysisTab;



    QTabWidget *mainTabWidget;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
