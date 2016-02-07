#include "scoreboardmain.h"
#include <QApplication>
#include <direct.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScoreboardMain w;
    mkdir(".\\Output\\");
    mkdir(".\\Update\\");
    w.show();

    return a.exec();
}
