#ifndef DEBUGLOGGER_H
#define DEBUGLOGGER_H

#include <qdatetime.h>
#include <qfile.h>
#include <QMutex>

namespace dd::earbug::logging {
    const qint64 LOG_FILE_LIMIT = 3000000;
    const QString LOG_PATH = "./";
    const QString LOG_FILENAME = "mylogfile.log";

    //thread safety
    QMutex mutex;

    void RedirectDebugMessagesToFile(QtMsgType type, const QMessageLogContext & context, const QString & str)
    {
        //thread safety
        mutex.lock();
        QString txt;

        //prepend timestamp to every message
        QString datetime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
        //prepend a log level label to every message
        switch (type) {
            case QtDebugMsg:
                txt = QString("[Debug*] ");
            break;
            case QtWarningMsg:
                txt = QString("[Warning] ");
            break;
            case QtInfoMsg:
                txt = QString("[Info] ");
            break;
            case QtCriticalMsg:
                txt = QString("[Critical] ");
            break;
            case QtFatalMsg:
                txt = QString("[Fatal] ");
        }

        QString filePath = LOG_PATH + LOG_FILENAME;
        QFile outFile(filePath);

        //if file reached the limit, rotate to filename.1
        if(outFile.size() > LOG_FILE_LIMIT){
            //roll the log file.
            QFile::remove(filePath + ".1");
            QFile::rename(filePath, filePath + ".1");
            QFile::resize(filePath, 0);
        }

        //write message
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&outFile);
        ts << datetime << txt << str << Qt::endl;

        //close fd
        outFile.close();
        mutex.unlock();
    }
}
#endif //DEBUGLOGGER_H
