#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    meterDataJSON=new smlMeterDataJson;
    meterD1A=new smlMeter;
    meterD1A->openPort();
    meterData=new Thread_nayax();

    testTimer=new QTimer(this);
    testTimer->setInterval(1000);
    testTimer->start();
    connect(testTimer,&QTimer::timeout,this,&MainWindow::refreshUI); //test timer is an OBJECT from Qtimer, this represent the object called mainwindow.
//QScreen *imx335=QGuiApplication::primaryScreen();
//QRect screenGeometry=imx335->geometry();
//int height =screenGeometry.height();
//int width=screenGeometry.width();
//qDebug()<<"height"<<height<<"width:"<<width;

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::refreshUI()
{ bool ok;
    double energy;
    double power;
    double voltage;
QString temp;
QString P;
QString W;
QString V;
QString I;
//QPainter painter(this);
//    painter.setPen(QPen(Qt::green,4));
//painter.setBrush(QColor(112, 173, 71));
//painter.drawEllipse(20,20,200,200);
QByteArray propertyNoValue,manufacturerIDValue;
propertyNoValue=QByteArray::fromHex( meterD1A->propertyNo.value);
manufacturerIDValue=QByteArray::fromHex( meterD1A->manufacturerID.value);


//ui->propertyNO->setText(propertyNoValue);

//QFont font=ui->propertyNO->font();
//font.setPointSize(24);
//ui->propertyNO->setFont(font);

//ui->ServerID->setText(meterD1A->deviceNumber.value);
//font.setPointSize(19);
//ui->ServerID->setFont(font);
//font.setPointSize(20);
//font.setBold(true);
//ui->info->setText("Received");
//ui->info->setFont(font);\











QFont font=ui->eng->font();
font.setPointSize(22);




W=QString::number(energy)+" Wh";
ui->eng->setFont(font);
ui->eng->setText("Energy: "+W);                      //set energy

    power=meterD1A->power.value.toInt(&ok,16)*qPow(10,meterD1A->power.scaler);
    P=QString::number(power)+" W";
        ui->pow->setFont(font);
        ui->pow->setText("Power: "+P);

voltage=meterD1A->voltage.value.toInt(&ok,16)*qPow(10,meterD1A->voltage.scaler);
V=QString::number(voltage)+" V";
    ui->voltage->setFont(font);
    ui->voltage->setText("Voltage: "+V);

I=QString::number(power/voltage)+" A";
    ui->current->setFont(font);
    ui->current->setText("Current: "+I);

   ui->pubKey->setFont(font);
   ui->pubKey->setText("Pub Key: NULL");

   ui->serNO->setFont(font);
   ui->serNO->setText("SN: "+propertyNoValue);

    ui->status->setFont(font);
    energy=meterD1A->energy.value.toInt(&ok,16)*qPow(10,meterD1A->energy.scaler);
    ui->status->setText("Status: "+meterD1A->energy.status);    //set status


    ui->manID->setFont(font);
    ui->manID->setText("User ID: "+manufacturerIDValue);    //set product name

     ui->cost->setFont(font);
     ui->cost->setText("Cost: 0 Euro");

     ui->duration->setFont(font);
     ui->duration->setText("Duration: NULL");
//ui->Energy->setDigitCount(5);
//ui->Energy->display(energy/1000);
//ui->Power->display(power);
//ui->info->setText("info");

//set Time
QDateTime time=QDateTime::currentDateTime();
QString timeStr=time.toString("yyyy-MM-dd hh:mm:ss.zzz");
//currentTimeLabel->setText(timeStr);
QString timeStr2=time.toString("yyyy-MM-dd hh:mm:ss ddd");
ui->clock->setText(timeStr2);


//----------------------------SET JSON data----------

meterDataJSON->voltage=V;

meterDataJSON->power=P;

meterDataJSON->energy=W;
meterDataJSON->timeStamp=timeStr;
meterDataJSON->chargerID=meterD1A->deviceNumber.value;
   meterData->nayax_open(meterDataJSON);
}



