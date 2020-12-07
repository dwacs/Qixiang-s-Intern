#include "thread_nayax.h"


QTimer *timer_vend_cancel = new QTimer();

Thread_nayax::Thread_nayax(QObject *parent) : QObject(parent)
{
    //---------------------------------GCC------------------------
    //const char *ptin_filepath = "/home/liang_jingzhe/tmp/ptin";
    //---------------------------------IMX------------------------
    const char *ptin_filepath = "/home/root/build-smlC141Test-imx_c141-Debug/ptin";
    ptin = new IPC_Pipe(ptin_filepath);
    PTin_Read_Thread = new QThread;
    ptin->moveToThread(PTin_Read_Thread);
   // if(Config_Payment == Nayax)
        PTin_Read_Thread->start();
    connect(PTin_Read_Thread,SIGNAL(finished()),ptin,SLOT(deleteLater()));
    connect(PTin_Read_Thread,SIGNAL(finished()),PTin_Read_Thread,SLOT(deleteLater()));
    connect(PTin_Read_Thread,SIGNAL(started()),ptin,SLOT(PipeRead()));

    //---------------------------------GCC------------------------
    //const char *ptout_filepath = "/home/liang_jingzhe/tmp/ptout";
    //---------------------------------IMX------------------------
    const char *ptout_filepath = "/home/root/build-smlC141Test-imx_c141-Debug/ptout";
    ptout = new IPC_Pipe(ptout_filepath);
    PTout_Write_Thread = new QThread;
    ptout->moveToThread(PTout_Write_Thread);
    //if(Config_Payment == Nayax)
        PTout_Write_Thread->start();
    connect(PTout_Write_Thread,SIGNAL(finished()),ptout,SLOT(deleteLater()));
    connect(PTout_Write_Thread,SIGNAL(finished()),PTout_Write_Thread,SLOT(deleteLater()));
    connect(PTout_Write_Thread,SIGNAL(started()),ptout,SLOT(PipeInit()));

    connect(this,SIGNAL(ptwrite(QString)),ptout,SLOT(PipeWrite(QString)));
    connect(ptin,SIGNAL(signal_pipe_read(QString)),this,SLOT(ptin_datapro(QString)));
}
Thread_nayax::~Thread_nayax()
{
    if(ptin!=nullptr)
        delete ptin;
    if(ptout!=nullptr)
        delete ptout;

    if(PTout_Write_Thread->isRunning())
        PTout_Write_Thread->exit();
    if(PTin_Read_Thread->isRunning())
        PTin_Read_Thread->exit();
}

void Thread_nayax::stop(void)
{
    if(PTout_Write_Thread->isRunning())
        PTout_Write_Thread->exit();
    if(PTin_Read_Thread->isRunning())
        PTin_Read_Thread->exit();

    delete timer_vend_cancel;
}
void Thread_nayax::ptin_datapro(QString data)
{
    qDebug() << "ptin: " << data;
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(data.toUtf8(), &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "nayax json error!"<<json_error.error;
        return;
    }

    QJsonObject PTin_rootobj = jsonDoc.object();
    //进行接收管道消息的处理
    if (PTin_rootobj.contains("Item"))
    {

    }
}
void Thread_nayax::nayax_open(smlMeterDataJson *data)
{
    //准备json数据
    QJsonObject openObject;
    openObject.insert("Voltage", data->voltage);

    openObject.insert("Power", data->power);

    openObject.insert("Energy", data->energy);
 openObject.insert("TimeStamp", data->timeStamp);
 openObject.insert("MeterId",data->meterID);
 openObject.insert("ChargerId",data->chargerID);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(openObject);
    QString str_json = jsonDoc.toJson();

    qDebug() << "ptout: " << str_json;
    //发送写管道信号
    emit ptwrite(str_json);
}
