#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <qdebug.h>
#include "countdowntimer.h"
#include "ui_countdowntimer.h"

CountDownTimer::CountDownTimer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CountDownTimer)
{
    //ui->lcdNumber = new QLCDNumber;
//    ui->lcdNumber->setDigitCount(20);
//    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
//    QPalette lcdpat = ui->lcdNumber->palette();
//    ui->lcdNumber->setPalette(lcdpat);
    ui->setupUi(this);
    ui->pushButton_start->setText("开始计时");
    ui->dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    ui->dateTimeEdit->setDateTime(QDateTime(QDate::currentDate(), QTime::currentTime()));//QDateTime::currentDateTime(),

}

CountDownTimer::~CountDownTimer()
{
    delete ui;
}

void CountDownTimer::StartTimerBtn()
{

    dateTime = ui->dateTimeEdit->dateTime();
    dateTimeCur = QDateTime::currentDateTime();
    uint stime = dateTimeCur.toTime_t();
    uint etime = dateTime.toTime_t();

    if(etime<=stime)
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QStringLiteral("请设置时间！"));
    else
    {
        m_fValue = etime - stime;
        QString data = QString("float is %1").arg(m_fValue);
        ui->lineEdit->setText(data);
        ui->pushButton_start->setEnabled(false);
        qDebug() << "starttime" << dateTime;
        qDebug() << "endtime" << dateTimeCur;
        qDebug() << "......................";

        qDebug() << "etime" << etime;
        qDebug() << "stime" << stime;
        qDebug() << "etime - stime" << etime - stime;

        int ndaysec = 24*60*60;
        qDebug() << "Day = " << (etime - stime)/(ndaysec) + ((etime - stime)%(ndaysec)+(ndaysec-1))/(ndaysec) - 1;

        m_pRefreshTimer = new QTimer;	//状态按更新定时器
        m_pRefreshTimer->start(1000);	/*状态栏1000ms定时*/
        connect(m_pRefreshTimer, SIGNAL(timeout()), this, SLOT(RefreshTimerSlot()));	// 状态栏刷新
    }
}

void CountDownTimer::RefreshTimerSlot()
{
    m_fValue-=1;
    ui->lcdNumber->display(m_fValue);
    if(m_fValue <= 0)
        ui->pushButton_start->setEnabled(true);
    //ui->lcdNumber->display(dateTime.toString("yyyy-MM-dd HH:mm:ss.zzz"));
}
