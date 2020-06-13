/*
 * author:fengclchn@outlook.com
 * project:CET6 Word Test
 * school:Harbin Institute of Technology at Weihai
 * grade:2019
 * student ID:2190400203
 * name:冯昶霖
 */
#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QFile>
#include <QTextStream>
using namespace std;

aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
    ui->setupUi(this);
    outputAbout();
}

aboutDialog::~aboutDialog()
{
    delete ui;
}

void aboutDialog::outputAbout()
{
    QFile f(":/text/about");
    QString str;
    if(f.open(QFile::ReadOnly))
    {
        QTextStream out(&f);
        out.setCodec("GBK");
        ui->textBrowser->setText(out.readAll());
        f.close();
    }
    else
    {
        ui->textBrowser->setText("Something error!");
    }
}
