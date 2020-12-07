#ifndef IPC_PIPE_H
#define IPC_PIPE_H
#include <QString>
#include <sys/types.h>
#include <sys/stat.h>
#include <QDebug>
#include <QObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <qthread.h>

class IPC_Pipe : public QObject
{
    Q_OBJECT
public:
    IPC_Pipe(const char *filename);
    ~IPC_Pipe();

private:

    QByteArray pipedata;
    const char *pipename;

    pthread_mutex_t pipe_write_lock;
    pthread_mutex_t pipe_read_lock;
signals:
    void signal_pipe_read(QString allData);
private slots:
    void PipeRead();
    void PipeWrite(QString str);
    void PipeInit(void);

};

#endif // IPC_PIPE_H
