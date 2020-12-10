#include "ipc_pipe.h"

IPC_Pipe::IPC_Pipe(const char *filename)
{
    //qstrcpy(pipename,filename);
    pipename  = filename;

    pthread_mutex_init(&pipe_write_lock,NULL);   //file lock
    pthread_mutex_init(&pipe_read_lock,NULL);
}
IPC_Pipe::~IPC_Pipe()
{
}
void IPC_Pipe::PipeInit(void)
{
    QFile file(pipename);
    if(file.exists() == false)
    {
        mknod(pipename, S_IFIFO | 0666, 0);     //mknod(filename, S_IFIFO | 0666,0) everyone can read and write
        qDebug() << "new a fifo file ."<<pipename;
    }
    else
    {
        file.remove();
        QThread::msleep(10);

        mknod(pipename, S_IFIFO | 0666, 0);
        qDebug() << "remove the old one and new a fifo file ."<<pipename; //delete old log
    }
}
void IPC_Pipe::PipeRead(void)
{
    QFile filein(pipename);
    if(filein.exists() == false)
    {
        mknod(pipename, S_IFIFO | 0666, 0);
        qDebug() << "new a fifo file .";
    }
    else
    {
        filein.remove();
        QThread::msleep(10);

        mknod(pipename, S_IFIFO | 0666, 0);
        qDebug() << "remove the old one and new a fifo file ."<<pipename;
    }
    while(1)
    {
      //  qDebug() << "pipe thread read=" << QThread::currentThreadId();
        pthread_mutex_lock(&pipe_read_lock);
        if(!filein.open(QIODevice::ReadOnly|QIODevice::Text))
        {
         //   qDebug() << "open pipe failed.";
        }

        QString allData;
        allData = filein.readAll();
        filein.close();

   //     qDebug() << "pipe file data:" << allData;
        emit signal_pipe_read(allData);
        pthread_mutex_unlock(&pipe_read_lock);
    }
}
void IPC_Pipe::PipeWrite(QString str)
{
    //qDebug() << "pipe thread write=" << QThread::currentThreadId();
    pthread_mutex_lock(&pipe_write_lock);
    QFile fileout(pipename);

    if(!fileout.open(QIODevice::WriteOnly | QIODevice::Text))
    {
     //   qDebug() << "open failed";
    }

    QTextStream txtOutput(&fileout);
    txtOutput << str << endl;
    fileout.flush();
    fileout.close();
    pthread_mutex_unlock(&pipe_write_lock);
}

