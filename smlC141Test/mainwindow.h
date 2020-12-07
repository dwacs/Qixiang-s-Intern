#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QDateTime>
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QScreen>
#include<QString>
#include<QLatin1String>
#include<QPainter>
#include<QtMath>



#include"ipc_pipe.h"
#include"thread_nayax.h"
#include"sml_meter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
friend smlMeter;
public:
QTimer *testTimer;
QTimer *led;
    smlMeterDataJson *meterDataJSON;
    smlMeter *meterD1A;
    Thread_nayax *meterData;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int serial_port_open(const char* device);
void transport_receiver(unsigned char *buffer, size_t buffer_len);
private:
    Ui::MainWindow *ui;

private slots:
      void refreshUI();
};
#endif // MAINWINDOW_H
