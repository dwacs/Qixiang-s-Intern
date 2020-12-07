#ifndef THREAD_NAYAX_H
#define THREAD_NAYAX_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include "ipc_pipe.h"
#include "sml_meter.h"
class Thread_nayax : public QObject
{
    Q_OBJECT
public:
    explicit Thread_nayax(QObject *parent = nullptr);
    ~Thread_nayax();
private:
    IPC_Pipe *ptin;
    IPC_Pipe *ptout;
    QThread  *PTin_Read_Thread;
    QThread  *PTout_Write_Thread;

signals:
    void ptwrite(QString str);
    void signal_nayax_vend_timeout(ushort ConnectorId);
    void signal_IDtagRead(QString idtag);

private slots:
    void ptin_datapro(QString data);

    void stop(void);
public slots:
    void nayax_open(smlMeterDataJson *data);
};

#endif // THREAD_NAYAX_H
