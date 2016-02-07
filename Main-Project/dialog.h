#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
public slots:
    void Home_Name(QString);
    void Away_Name(QString);
    void Home_Score(int);
    void Away_Score(int);
    void period(int);
    void Clock(QString);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
