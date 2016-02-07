#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <scoreboardmain.h>
#include <qdebug.h>
class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(int id, QObject *parent = 0);
    void run();
signals:
    void error(QTcpSocket::SocketError socketerror);
public slots :
    void readyRead();
    void disconnection();
private:
 QTcpSocket *socket;
 int Socketdiscriptor;
};

#endif // MYTHREAD_H
