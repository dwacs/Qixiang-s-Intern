#ifndef SML_METER_H
#define SML_METER_H


#include <QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>


#include"unit.h"

class smlMeterDataJson:public QObject       //Define a class for D2C transmission
{Q_OBJECT
public:
 QString energy;
 QString power;
 QString voltage;
 QString timeStamp;
 QString chargerID="testCharger1";
 QString meterID="testmeter1";
};

class smlClientData                  // Define a class to record the client data from the DC meter
{
  public:
    QString clientID;
    QString serverID;
    QString listName;
    int duration;
};

class smlMeterData :public QObject      //Define a class to record the meter data. such as energy, voltage ....
{  Q_OBJECT
  public:
    //  quint8 group_id;
     //  QString transactionID;
      // quint8 abortOnError;
       //quint16 crc;
       //sml list

    QByteArray objName;
     QByteArray status; // optional
       QByteArray valTime; // optional
       int unit; // optional
       qint8 scaler;
       QByteArray value;
       QByteArray valueSignature;
};



class smlMeter :public QObject        // Define a class to implement the meter function.
{  friend smlMeterData;
    Q_OBJECT
private:


    QByteArray smlBuf=NULL;   //raw data
    QByteArray smlData=NULL;

uint otectString(int position,QByteArray data);           //Parse the otect string and return the length of the string for reading.
int setData(int position,QByteArray data);             // Record the data from the sml meter and return the data position after that data.


public:
QSerialPort *serial;
smlClientData client1;
smlMeterData manufacturerID;
smlMeterData deviceNumber;
smlMeterData energy;
smlMeterData power;
smlMeterData propertyNo;
smlMeterData voltage;

   void openPort();     // open serial port
   void dataParse();    // Parse the meter data
   void getData(QByteArray data);  // extract the meter data from the sml data pack
   void getClientdata(int dataPosition, QByteArray data);  // extract the client data from the sml data pact

     ~smlMeter();

private slots:

   void readData();   // read the data from the IR sensor


};

#endif // SML_METER_H
