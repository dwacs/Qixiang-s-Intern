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

QByteArray propertyNoValue,manufacturerIDValue;
propertyNoValue=QByteArray::fromHex( meterD1A->propertyNo.value);
manufacturerIDValue=QByteArray::fromHex( meterD1A->manufacturerID.value);

QFont font("Caption",22);
//QFont font=ui->eng->font();
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
double month;
QString dur;
     month=double(meterD1A->client1.duration)/3600/24;
     qDebug()<<"time---"<<meterD1A->client1.duration<<"version1xxx"<<month<<"xx";
     dur=QString::number(month,'f',3);
     ui->duration->setFont(font);
     ui->duration->setText("Duration: "+dur+" days");


//-----------------set Time------------------
QDateTime time=QDateTime::currentDateTime();
QString timeStr=time.toString("yyyy-MM-dd hh:mm:ss.zzz");
QString timeStr2=time.toString("yyyy-MM-dd hh:mm:ss ddd");
font.setPointSize(14);
ui->clock->setFont(font);
ui->clock->setText(timeStr2);


//----------------------------SET JSON data----------

meterDataJSON->voltage=V;

meterDataJSON->power=P;

meterDataJSON->energy=W;
meterDataJSON->timeStamp=timeStr;
meterDataJSON->chargerID=meterD1A->deviceNumber.value;
   meterData->nayax_open(meterDataJSON);

   //------------set meter1-----------------------
   font.setPointSize(22);
   ui->eng_2->setFont(font);
   ui->eng_2->setText("Energy: ");

   ui->pow_2->setFont(font);
   ui->pow_2->setText("Power: ");

   ui->voltage_2->setFont(font);
   ui->voltage_2->setText("Voltage: ");


   ui->serNO_2->setFont(font);
   ui->serNO_2->setText("SN: ");

   //-------------Set meter2 ---------------------
   ui->eng_3->setFont(font);
   ui->eng_3->setText("Energy: ");

   ui->pow_3->setFont(font);
   ui->pow_3->setText("Power: ");

   ui->voltage_3->setFont(font);
   ui->voltage_3->setText("Voltage: ");


   ui->serNO_3->setFont(font);
   ui->serNO_3->setText("SN: ");

   //---------------set cloud input
   ui->max_input->setFont(font);
   ui->max_input->setText("Max Input (kW): ");

   ui->max_output->setFont(font);
   ui->max_output->setText("Max Output (kW): ");

   ui->storage_cap->setFont(font);
   ui->storage_cap->setText("Storage Cap (kWh): ");

   ui->source_type->setFont(font);
   ui->source_type->setText("Source type: ");

   ui->node->setFont(font);
   ui->node->setText("Slave");

   ui->nodeNO->setFont(font);
   ui->nodeNO->setText("Node No.: ");

}




void MainWindow::on_pushButton_clicked()
{
    QApplication::exit();
}
