#ifndef SML_METER_H
#define SML_METER_H


#include <QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>


#include"unit.h"

class smlMeterDataJson:public QObject
{Q_OBJECT
public:
 QString energy;
 QString power;
 QString voltage;
 QString timeStamp;
 QString chargerID="testCharger1";
 QString meterID="testmeter1";
};

class smlClientData
{
  public:
    QString clientID;
    QString serverID;
    QString listName;
    int duration;
};

class smlMeterData :public QObject
{  Q_OBJECT
  public:
    //  quint8 group_id;
       QString transactionID;
       quint8 abortOnError;
       quint16 crc;
       //sml list

    QByteArray objName;
     QByteArray status; // optional
       QByteArray valTime; // optional
       int unit; // optional
       qint8 scaler;
       QByteArray value;
       QByteArray valueSignature;
};
class smlMeter :public QObject
{  friend smlMeterData;
    Q_OBJECT
private:


    QByteArray smlBuf=NULL;   //raw data
    QByteArray smlData=NULL;

uint otectString(int position,QByteArray data);
int setData(int position,QByteArray data);


public:
QSerialPort *serial;
smlClientData client1;
smlMeterData manufacturerID;
smlMeterData deviceNumber;
smlMeterData energy;
smlMeterData power;
smlMeterData propertyNo;
smlMeterData voltage;

   void openPort();
   void dataParse();
   void getData(QByteArray data);
   void getClientdata(int dataPosition, QByteArray data);

     ~smlMeter();

private slots:

   void readData();


};

#endif // SML_METER_H
