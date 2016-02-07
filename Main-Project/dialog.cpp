#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::Home_Name(QString text)
{
    ui->Home_Name->setText(text);
}
void Dialog::Away_Name(QString text)
{
    ui->Away_Name->setText(text);
}
void Dialog::Home_Score(int text)
{
    ui->Home_Score->setText(QString::number(text));
}
void Dialog::Away_Score(int text)
{
    ui->Away_Score->setText(QString::number(text));
}
void Dialog::period(int text)
{
    ui->Period->setText(QString::number(text));
}

void Dialog::Clock(QString text)
{
    ui->Clock->setText(text);
}
