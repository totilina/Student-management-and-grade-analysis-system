#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QComboBox>
#include <QMessageBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTableView>
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
    void refreshAnalysisTableView(const QVector<Student>& students);
    void analysisFilterChanged();
    QWidget *scoreManagerTab;
    QTableView *analysisTableView;
    QStandardItemModel *analysisModel;
    QComboBox *analysisClassComboBox;
    QComboBox *analysisGradeComboBox;
    QComboBox *analysisMajorComboBox;
    QComboBox *analysisCourseComboBox;

    // 成绩分析界面
    void createdScoreAnalysisTab();
    void refreshScoresTree(const QVector<Student>& students);
    void onFilterChanged();
    void updateFileredOptios(const QVector<Student>& students);
    // void exportCsvFile();
    bool exportCsvFile(const QVector<Student>& students,const QString& courseName, QString fileName = NULL);
    bool exportCsvFile(const QVector<Student>& students,const QVector<QString>& courseNames, QString fileName = NULL);
    void batchExportData();


    void ascendingSort();
    void descendingSort();

    void addNewCourse(); // 待实现

    // 可以使用QSet,为了简化数据结构可以使用QVector,但效率会降低
    QVector<QString> majors, grades, classes, courses;

    // bool isBatchExportData = false;

    QVector<Student> fileredStudents;
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
