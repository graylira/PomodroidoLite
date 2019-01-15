#include "pomodroidolitemain.h"
#include <QApplication>
#include <QMutex>
#include <QDateTime>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>

void outputMessage(QtMsgType type, const QMessageLogContext &, const QString & str)
{
    static QMutex mutex;
    mutex.lock();

    QString text, textName;
    switch (type)
    {
    case QtInfoMsg:
        text = QString("Info:");
        textName = QString("Info ");
        break;

    case QtDebugMsg:
        text = QString("Debug:");
        textName = QString("Debug ");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        textName = QString("Warning ");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        textName = QString("Critical ");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
        textName = QString("Fatal ");
        break;
    }

    QString context_info = QString("File:(%1) Line:(%2)").arg(__FILE__).arg(__LINE__);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(str).arg(current_date_time);
    QString filesName = "Log/" + textName + current_date + ".txt";
    //QFile file("LogFiles/log.txt");
    QFile file(filesName);
    file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out(&file);
    out << message << "\n";
    file.flush();
    file.close();
    mutex.unlock();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler(outputMessage);
    PomodroidoLiteMain w;
    qApp->setStyleSheet("QStatusBar {background: brown;}");
    w.show();

    return a.exec();
}
