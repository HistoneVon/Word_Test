#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QObject>
#include <string>
#include "aboutdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct WORD
{
    QString en;
    QString zh;
    QString ans;//我的答案
    bool crct;//正误 相等为0，不等非0
    int times;//答题次数
    int corTimes;//正确次数
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void importWord();//导入文件
    void startTest();//出题
    void outputToChart(struct WORD temp,QTableWidget* tempWidget);//向表格输出
    void tableWidgetItemSelected(QTableWidget* tempWidget);//选中回显
    void check();//提交
    void handleChart(QTableWidget* tempWidget,int row,
                     QString& s1,QString& s2,bool& s3,int& s4);//获取一行表格内容至字符串（暂时未写）
    void handleChart(QTableWidget* tempWidget,int row,
                     QString& s1,QString& s2,bool& s3);//获取表格（无次数）
    void handleChart(QTableWidget* tempWidget,int row,
                     QString& s1,QString& s2);//仅获取中英文（重载）
    void exportWord(QTableWidget* tempWidget,QString filename);//导出单词（通用）
    void fileStringAnalyze(QString str,struct WORD& tempWord);//字符串解析
    void importWordDone(QTableWidget* tempWidget,QString filename);//打开已导出单词本

protected:
    Ui::MainWindow *ui;
    struct WORD wordTemp;
    int rowTemp,sumRow;//随机行数，总行数
//    virtual void keyPressEvent(QKeyEvent* event);
private slots:
    void on_pushButton_import_clicked();
    void on_pushButton_start_clicked();
    void on_pushButton_end_clicked();
    void on_pushButton_check_clicked();
    void on_pushButton_export_2_clicked();
    void on_pushButton_export_1_clicked();
    void on_pushButton_delete_clicked();
    void on_actionImport_triggered();
    void on_actionStart_triggered();
    void on_actionEnd_triggered();
    void on_actionExport_triggered();
    void on_actionDone_triggered();
    void on_actionCheck_triggered();
    void on_actionDelete_triggered();
    void on_actionExit_triggered();
    void on_actionWordpad_triggered();
    void on_actionWrongWords_triggered();
    void on_actionDoneWords_triggered();
    void on_actionClear_triggered();
    void on_tableWidget_1_itemSelectionChanged();
    void on_tableWidget_2_itemSelectionChanged();
    void on_tableWidget_3_itemSelectionChanged();
    void on_pushButton_importWrong_clicked();
    void on_pushButton_importDone_clicked();
    void on_actionImportWrong_triggered();
    void on_actionImportDone_triggered();
    void on_actionAbout_triggered();
};

#endif // MAINWINDOW_H
