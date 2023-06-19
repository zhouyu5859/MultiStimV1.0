#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTcpSocket>
#include "mutichannelstim.h"
#include <QTime>
#include <QFileDialog>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    void serialPort_readyRead();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_checkBox_clicked();

    void on_checkBox_2_clicked();

    void on_pushButton_6_clicked();

    void on_checkBox_3_clicked();

    void on_checkBox_4_clicked();

private:
    Ui::MainWindow *ui;

    QSerialPort *serial;
    QString Receivetext;
    QString Sendtext;
    QTcpSocket *tcpsocket;
    mutiChannelStim *multistim;
    //STIM3PARAM stim3param;
    QMap<unsigned short, unsigned short> mychannleAmp;

};
#endif // MAINWINDOW_H
