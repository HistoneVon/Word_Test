/*
 * author:fengclchn@outlook.com
 * project:CET6 Word Test
 * school:Harbin Institute of Technology at Weihai
 * grade:2019
 * student ID:2190400203
 * name:冯昶霖
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <string>
#include <ctime>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QTableWidget>
#include <QTabWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>
using namespace std;

//字符串转换
QString cstr2qstr(string cs)
{
    return QString::fromLocal8Bit(cs.data());
}

string qstr2cstr(QString qs)
{
    QByteArray cdata = qs.toLocal8Bit();
    return string(cdata);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //设置lineEdit不可编辑
    ui->lineEdit->setReadOnly(true);
//    ui->lineEdit->setEnabled(false);//背景和字幕会变灰
    ui->lineEdit->setFocusPolicy(Qt::NoFocus);//无法获得焦点

    //改变表格样式
    //QHeaderView::ResizeToContents适合内容|QHeaderView::Stretch伸展
    ui->tableWidget_1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);//伸展
    ui->tableWidget_1->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget_1->horizontalHeader()->setDefaultSectionSize(90);//默认行宽
    ui->tableWidget_1->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中
    ui->tableWidget_1->setSelectionMode(QAbstractItemView::ExtendedSelection);//可以选中多个目标

    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);//伸展
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setDefaultSectionSize(90);//默认行宽
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::ExtendedSelection);//可以选中多个目标

    ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);//伸展
    ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget_3->horizontalHeader()->setDefaultSectionSize(90);//默认行宽
    ui->tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中
    ui->tableWidget_3->setSelectionMode(QAbstractItemView::ExtendedSelection);//可以选中多个目标

    //ui->tabWidgetWordpad->setTabShape(QTabWidget::Triangular);//设置选项卡形状

    //设置按钮初始状态
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_end->setEnabled(false);
    ui->pushButton_check->setEnabled(false);
    ui->pushButton_delete->setEnabled(false);
    ui->pushButton_export_1->setEnabled(false);
    ui->pushButton_export_2->setEnabled(false);

    //设置菜单栏工具栏初始状态
    ui->actionStart->setEnabled(false);
    ui->actionEnd->setEnabled(false);
    ui->actionCheck->setEnabled(false);
    ui->actionDelete->setEnabled(false);
    ui->actionExport->setEnabled(false);
    ui->actionDone->setEnabled(false);
    ui->actionClear->setEnabled(false);

    //暂时隐藏，后续开发可用
    ui->tableWidget_1->setColumnHidden(3,true);
    ui->tableWidget_2->setColumnHidden(3,true);
    ui->tableWidget_3->setColumnHidden(3,true);

    srand(time(NULL));

    QObject::connect(
                ui->lineEdit_2,
                SIGNAL(returnPressed()),
                ui->pushButton_check,
                SIGNAL(clicked()),
                Qt::UniqueConnection
                );//回车确认
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::importWord()
{
    QFile f(":/text/CET6");
    if(f.open(QFile::ReadOnly))
    {
        QTextStream str(&f);
        QString strEN,strZH;
        while (true)
        {
            strEN=str.readLine();
            strZH=str.readLine();
            int row=ui->tableWidget_1->rowCount();
            ui->tableWidget_1->insertRow(row);
            ui->tableWidget_1->setItem(row,0,new QTableWidgetItem(strEN));
            ui->tableWidget_1->setItem(row,1,new QTableWidgetItem(strZH));
            if(str.atEnd())
            {
                break;
            }
        }
        f.close();
    }
    else
    {
        ui->lineEdit->setText("文件打开错误");
    }
}

//出题函数
void MainWindow::startTest()
{
    wordTemp.en=ui->tableWidget_1->item(rowTemp,0)->text();
    wordTemp.zh=ui->tableWidget_1->item(rowTemp,1)->text();
    ui->lineEdit->setText(wordTemp.zh);
    ui->lineEdit_2->setAlignment(Qt::AlignLeft);
}

//向表格输出
void MainWindow::outputToChart(struct WORD temp,QTableWidget* tempWidget)
{
    int rowCurrent=tempWidget->rowCount();//当前行
    tempWidget->insertRow(rowCurrent);
    tempWidget->setItem(rowCurrent,0,new QTableWidgetItem(temp.en));
    tempWidget->setItem(rowCurrent,1,new QTableWidgetItem(temp.zh));
    if(temp.crct==true)//等于
    {
        tempWidget->setItem(rowCurrent,2,new QTableWidgetItem("√"));
    }
    else//不等于
    {
        tempWidget->setItem(rowCurrent,2,new QTableWidgetItem("×"));
    }
    tempWidget->scrollToBottom();//滚动条自动最底部
}

//提交
void MainWindow::check()
{
    wordTemp.ans=ui->lineEdit_2->text();//获取答案
    wordTemp.crct=!(QString::compare(wordTemp.en,wordTemp.ans));//判断正误
    //底层函数问题，compare相等为0，不等非0，故加!()
    outputToChart(wordTemp,ui->tableWidget_3);//向已答本写入
    if(wordTemp.crct==false)//错误时
    {
        outputToChart(wordTemp,ui->tableWidget_2);//向错题本写入
    }
    ui->lineEdit_2->clear();//清空输入栏
    //已答本刚答过的一题设置红色或黑体
    //错题显示红色
}

//获取表格一行内容（完整）
void MainWindow::handleChart(QTableWidget* tempWidget,int row,
                             QString& s1,QString& s2,bool& s3,int& s4)
{
    s1=tempWidget->item(row,0)->text();
    s2=tempWidget->item(row,1)->text();
    if(tempWidget->item(row,2)->text()=="√")
    {
        s3=true;
    }
    else
    {
        s3=false;
    }
//    s4=tempWidget->item(row,3)->text().toInt();
    s4=1;
}

//获取表格一行内容（无次数）
void MainWindow::handleChart(QTableWidget* tempWidget,int row,
                             QString& s1,QString& s2,bool& s3)
{
    s1=tempWidget->item(row,0)->text();
    s2=tempWidget->item(row,1)->text();
    if(tempWidget->item(row,2)->text()=="√")
    {
        s3=true;
    }
    else
    {
        s3=false;
    }
}

//仅获取中英文（重载）
void MainWindow::handleChart(QTableWidget* tempWidget,int row,
                             QString& s1,QString& s2)
{
    s1=tempWidget->item(row,0)->text();
    s2=tempWidget->item(row,1)->text();
}

//选中回显
void MainWindow::tableWidgetItemSelected(QTableWidget* tempWidget)
{
    int row=tempWidget->currentRow();
    QString tempEN,tempZH;
    handleChart(tempWidget,row,tempEN,tempZH);
    ui->lineEdit_2->setAlignment(Qt::AlignCenter);
    ui->lineEdit->setText(tempZH);
    ui->lineEdit_2->setText(tempEN);
}

//导出单词表格函数
void MainWindow::exportWord(QTableWidget* tempWidget,QString filename)
{
    struct WORD temp;
    int sumRow=tempWidget->rowCount();
    int i;
    QString sEN,sZH,sCRCT,sTIMES,str;//英文 中文 正误 答题次数 母串
    QFile f(filename);
    if(f.open(QFile::WriteOnly|QFile::Append|QIODevice::Text))//追加方式打开
    {
        QTextStream out(&f);
        for(i=0;i<sumRow;++i)
        {
            //将表格获取到结构体
            if(tempWidget->item(i,0)!=NULL)
            {
                handleChart(tempWidget,i,temp.en,temp.zh,temp.crct,temp.times);
            }
            else
            {
                continue;//如果出现空行（空英文单词）就跳过本行
            }

            //将结构体格式化为字符串
            sEN=temp.en;
            sZH=temp.zh;
            if(temp.crct)
            {
                sCRCT="true";
            }
            else
            {
                sCRCT="false";
            }
            sTIMES=temp.times;
            sTIMES="default";//临时缺省
            str=sEN+"~"+sZH+"~"+sCRCT+"~"+sTIMES;

            //将字符串写入文件中
            out<<str<<endl;
        }
        f.close();
    }
    else
    {
        ui->lineEdit->setText("文件无法创建");
    }
}

//字符串处理函数
void MainWindow::fileStringAnalyze(QString str, struct WORD& tempWord)
{
    int n1,n2,n3;
    n1=str.indexOf("~",0);
    n2=str.indexOf("~",n1+1);
    n3=str.indexOf("~",n2+1);
    tempWord.en=str.mid(0,n1);
    tempWord.zh=str.mid(n1+1,n2-n1-1);
    if(str.mid(n2+1,n3-n2-1)=="true")
    {
        tempWord.crct=true;
    }
    else
    {
        tempWord.crct=false;
    }
}

//打开已导出单词本
void MainWindow::importWordDone(QTableWidget *tempWidget, QString filename)
{
    struct WORD tempWord;
    QString tempStr;
    QFile f(filename);
    if(f.open(QFile::ReadOnly))
    {
        QTextStream tempLine(&f);
        while (!tempLine.atEnd())
        {
            tempStr=tempLine.readLine();
            fileStringAnalyze(tempStr,tempWord);//字符串处理
            outputToChart(tempWord,tempWidget);//向表格输出
        }
        f.close();
        ui->lineEdit->setText("导入文件成功");
    }
    else
    {
        ui->lineEdit->setText("找不到文件");
    }
}

void MainWindow::on_pushButton_import_clicked()//导入文件
{
    importWord();
    ui->pushButton_import->setEnabled(false);
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_delete->setEnabled(true);
    ui->actionImport->setEnabled(false);
    ui->actionStart->setEnabled(true);
    ui->actionDelete->setEnabled(true);
}

void MainWindow::on_pushButton_start_clicked()//出题
{
    //判断是否导入(default)
    //设置样式
    ui->pushButton_start->setText("下一题");
    ui->tabWidgetWordpad->setCurrentIndex(2);//设置活动标签页
    ui->lineEdit_2->setFocus();//将输入框设置为默认焦点
    ui->lineEdit_2->clear();
    //出题并获取答案
    sumRow=ui->tableWidget_1->rowCount();//总行数
    rowTemp=rand()%sumRow;//随机行数
    startTest();
    ui->pushButton_end->setEnabled(true);
    ui->pushButton_check->setEnabled(true);
    ui->pushButton_export_1->setEnabled(true);
    ui->pushButton_export_2->setEnabled(true);
    ui->actionEnd->setEnabled(true);
    ui->actionCheck->setEnabled(true);
    ui->actionExport->setEnabled(true);
    ui->actionDone->setEnabled(true);
    ui->pushButton_importDone->setEnabled(false);
    ui->pushButton_importWrong->setEnabled(false);
    ui->actionImportDone->setEnabled(false);
    ui->actionImportWrong->setEnabled(false);
}

void MainWindow::on_pushButton_end_clicked()//结束
{
    ui->pushButton_start->setText("开始");
    ui->pushButton_end->setEnabled(false);
    ui->pushButton_check->setEnabled(false);
    ui->actionEnd->setEnabled(false);
    ui->actionCheck->setEnabled(false);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
}

void MainWindow::on_pushButton_check_clicked()//提交
{
    if(ui->pushButton_start->text()=="下一题")//即如果没有开始就不执行提交
    {
        check();
        sumRow=ui->tableWidget_1->rowCount();//总行数
        rowTemp=rand()%sumRow;//随机行数
        startTest();//再次调用
    }
    ui->actionClear->setEnabled(true);
}

void MainWindow::on_pushButton_export_2_clicked()//导出已答
{
    exportWord(ui->tableWidget_3,"./Wordsdone.txt");
    ui->lineEdit->setText("导出已答完成");
    //ui->pushButton_importDone->setEnabled(true);
    //导出后不再能够打开文件，后续维护可以重开窗口
}

void MainWindow::on_pushButton_export_1_clicked()//导出错题
{
    exportWord(ui->tableWidget_2,"./Wordswrong.txt");
    ui->lineEdit->setText("导出错题完成");
    //ui->pushButton_importWrong->setEnabled(true);
    //导出后不再能够打开文件，后续维护可以重开窗口
}

void MainWindow::on_pushButton_delete_clicked()//删除
{
    QTableWidget* tableWidgetTemp;
    int row;
    if(ui->tabWidgetWordpad->currentIndex()==0)
    {
        tableWidgetTemp=ui->tableWidget_1;
    }
    else if(ui->tabWidgetWordpad->currentIndex()==1)
    {
        tableWidgetTemp=ui->tableWidget_2;
    }
    else
    {
        tableWidgetTemp=ui->tableWidget_3;
    }
    row=tableWidgetTemp->currentRow();
    tableWidgetTemp->removeRow(row);
}

void MainWindow::on_pushButton_importWrong_clicked()//导入错题本
{
    importWordDone(ui->tableWidget_2,"./Wordswrong.txt");
    ui->tabWidgetWordpad->setCurrentIndex(1);
    ui->pushButton_importWrong->setEnabled(false);
    ui->actionClear->setEnabled(true);
}

void MainWindow::on_pushButton_importDone_clicked()//导入已答本
{
    importWordDone(ui->tableWidget_3,"./Wordsdone.txt");
    ui->tabWidgetWordpad->setCurrentIndex(2);
    ui->pushButton_importDone->setEnabled(false);
}


//菜单栏
void MainWindow::on_actionImport_triggered()//导入文件
{
    on_pushButton_import_clicked();
}

void MainWindow::on_actionStart_triggered()//开始
{
    on_pushButton_start_clicked();
}

void MainWindow::on_actionEnd_triggered()//结束
{
    on_pushButton_end_clicked();
}

void MainWindow::on_actionExport_triggered()//导出错题
{
    on_pushButton_export_1_clicked();
}

void MainWindow::on_actionDone_triggered()//导出已答
{
    on_pushButton_export_2_clicked();
}

void MainWindow::on_actionCheck_triggered()//提交
{
    on_pushButton_check_clicked();
}

void MainWindow::on_actionDelete_triggered()//删除
{
    on_pushButton_delete_clicked();
}

void MainWindow::on_actionExit_triggered()//退出
{
    exit(0);
}

void MainWindow::on_actionWordpad_triggered()//单词本
{
    ui->tabWidgetWordpad->setCurrentIndex(0);
}

void MainWindow::on_actionWrongWords_triggered()//错题本
{
    ui->tabWidgetWordpad->setCurrentIndex(1);
}

void MainWindow::on_actionDoneWords_triggered()//已答本
{
    ui->tabWidgetWordpad->setCurrentIndex(2);
}

void MainWindow::on_actionClear_triggered()//清除错题记录（仅菜单栏有）
{
    //提示不可恢复
    QMessageBox::StandardButton choose;
    choose=QMessageBox::question(this,"清除错题记录","清除后不可恢复，\n确定要清除吗？",
                                 QMessageBox::Yes|QMessageBox::No);//消息框
    if(choose==QMessageBox::Yes)
    {
        //ui->tableWidget_2->clearContents();//清除内容
        int row=ui->tableWidget_2->rowCount()-1;//行数比行号大1
        while(row!=0)
        {
            row=ui->tableWidget_2->rowCount()-1;
            ui->tableWidget_2->removeRow(row);
        }
    }
    else
    {
        ui->lineEdit->setText("放弃清除");
    }
}

void MainWindow::on_tableWidget_1_itemSelectionChanged()//单词本选中回显
{
    tableWidgetItemSelected(ui->tableWidget_1);
}

void MainWindow::on_tableWidget_2_itemSelectionChanged()//错题本选中回显
{
    tableWidgetItemSelected(ui->tableWidget_2);
}

void MainWindow::on_tableWidget_3_itemSelectionChanged()//已答本选中回显
{
    tableWidgetItemSelected(ui->tableWidget_3);
}

void MainWindow::on_actionImportWrong_triggered()//导入错题本
{
    on_pushButton_importWrong_clicked();
}

void MainWindow::on_actionImportDone_triggered()//导入已答本
{
    on_pushButton_importDone_clicked();
}

void MainWindow::on_actionAbout_triggered()//关于
{
    aboutDialog about;
    about.exec();
}
