//////////////////////////////////////////////////////////////////////////
/// COPYRIGHT NOTICE
/// Copyright (c) 2018, Write by 赵泽文
/// All rights reserved.
/// @file    PomodroidoLiteMain.cpp
/// @brief   基于QT开发的番茄钟软件
/// Creation time：2018.11.21
/// to my little baby
//////////////////////////////////////////////////////////////////////////
#include <QtCharts>
#include <Q3DBars>
#include <QtDataVisualization>
#include "parameter_config.h"
#include "pomodroidolitemain.h"
#include "ui_pomodroidolitemain.h"
#include "user/graphbars3d.h"
#include "settomatodialog.h"
#include "dbquerydialog.h"
#include "gadget/countdowntimer.h"
#include "aboutintrodialog.h"
#include "databaseoperate.h"

using namespace QtCharts;
using namespace QtDataVisualization;

TomatoTiming TomatoPara;
/*--------------------------------------------------------
函数名：PomodroidoLiteMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PomodroidoLiteMain)
功能：构造函数
创建：赵泽文
出口：
入口：
--------------------------------------------------------*/
PomodroidoLiteMain::PomodroidoLiteMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PomodroidoLiteMain)
{
    ui->setupUi(this);
    m_pBars = new Q3DBars();
    m_pModifier = new GraphBars3D(m_pBars,ui->widget_3DBars);
    TimerInit(TimerStatusBar,TimerOpen);    //状态栏更新
    ReadIniConfig();    //配置表更新
    DataLoadInit();     //数据库载入
    RefreshTimeStrDis();    //倒计时字符更新
    qInfo () << "Program up and running...";    //日志记录
    connect(ui->action_Version_Info, SIGNAL(triggered()), this, SLOT(actVersionMenu_clicked())); //	关于版本
    connect(ui->action_databaseQuery, SIGNAL(triggered()), this, SLOT(actDBQueryMenu_clicked())); //	数据记录查询
    connect(ui->action_timeCountDown, SIGNAL(triggered()), this, SLOT(actTimeDownMenu_clicked())); //	倒计时小工具

}

/*--------------------------------------------------------
函数名：PomodroidoLiteMain::~PomodroidoLiteMain()
功能：析构函数
创建：赵泽文
出口：
入口：
--------------------------------------------------------*/
PomodroidoLiteMain::~PomodroidoLiteMain()
{
    delete ui;  //这里已经自动释放
}

/*--------------------------------------------------------
函数名：setTomatoSlot()
功能：设置番茄钟
创建：赵泽文
出口：
入口：
--------------------------------------------------------*/
void PomodroidoLiteMain::setTomatoSlot()
{
    SetTomatoDialog setTomatoDlg;
    if (setTomatoDlg.exec() == QDialog::Accepted)
    {
        ReadIniConfig();
        RefreshTimeStrDis();
    }

}

/*--------------------------------------------------------
函数名：createTomatoSlot()
功能：开启番茄钟
创建：赵泽文
出口：
入口：
--------------------------------------------------------*/
void PomodroidoLiteMain::createTomatoSlot()
{
    ui->pushButton_create->setEnabled(false);
    ReadIniConfig();
    TimerInit(TimerBeginTomato,TimerOpen);
    m_strStartTime = m_getTime.toString("yyyy.MM.dd hh:mm:ss ddd");
    qInfo () << "Pomodoro Have Created!";
}

/*--------------------------------------------------------
函数名：discardTomatoSlot()
功能：丢弃番茄钟
创建：赵泽文
出口：
入口：
--------------------------------------------------------*/
void PomodroidoLiteMain::discardTomatoSlot()
{
    ui->pushButton_create->setEnabled(true);
    TimerInit(TimerBeginTomato,TimerClose);
    TomatoPara.Second = 0;
    ui->lcdNumber->display("00:00");
    m_strStartTime = "";
    m_strEndTime = "";
    qInfo () << "Pomodoro Have been discarded!";
    QMessageBox::information(this, "提示",QStringLiteral("🍅已被丢弃！"));
}


/*--------------------------------------------------------
函数名：TimerInit(unsigned int timerNo, bool control)
功能：定时器初始化
创建：赵泽文
出口：
入口：定时器号,开关控制
--------------------------------------------------------*/
void PomodroidoLiteMain::TimerInit(unsigned int timerNo, bool control)
{
    switch (timerNo)
    {
        case TimerStatusBar:
        {
            if (control == TimerOpen)
            {
                m_pRefreshStatusBarTimer = new QTimer;	//状态按更新定时器
                m_pRefreshStatusBarTimer->start(500);	/*状态栏500ms定时*/
                connect(m_pRefreshStatusBarTimer, SIGNAL(timeout()), this, SLOT(RefreshStatusBarSlot()));	// 状态栏刷新
            }
            else if (control == TimerClose)
                m_pRefreshStatusBarTimer->stop();
            break;
        }
        case TimerBeginTomato:
        {
            if (control == TimerOpen)
            {
                m_pRefreshTomatoTimer = new QTimer;	//状态按更新定时器
                m_pRefreshTomatoTimer->start(1000);	/*状态栏1000ms定时*/
                connect(m_pRefreshTomatoTimer, SIGNAL(timeout()), this, SLOT(RefreshTomatoSlot()));	// 状态栏刷新
            }
            else if (control == TimerClose)
                m_pRefreshTomatoTimer->stop();
            break;
        }
        default:
            break;
    }
}
/*--------------------------------------------------------
函数名：DataLoadInit()
功能：数据载入
创建：赵泽文
出口：
入口：
--------------------------------------------------------*/
void PomodroidoLiteMain::DataLoadInit()
{
    QString nextDate = "";
    QDateTime dateTime = QDateTime::currentDateTime();
    QDateTime afterOneMonthDateTime = dateTime.addYears(-4);    //过去四个月
    QString startDate = afterOneMonthDateTime.toString("yyyy.MM");	//得到遍历的起始时间
    int nEndDate = dateTime.toString("yyyy").toInt()+1;
    QString endDate = QString::number(nEndDate) +".1";//dateTime.toString("yyyy.MM");	//得到遍历的结束时间
    int year,month;
    vector<vector<int> > result(4, vector<int> (12));

    if(DBOperate->CreateConnect(DBConect,"my_LocalDataBase","QSQLITE") == 0)    //连接数据库
    {
        m_strStatusBarInfo = "  🍅仓库已连接";
        pKeyValue = DBOperate->GetLastPKey();


        QSqlQuery sql_query;
        QString select_sql = "select * from PomodoroList";
        int count = 0;
        if (!sql_query.exec(select_sql))
        {
            qWarning() << sql_query.lastError();
        }
        else
        {
            while (sql_query.next())
            {
                QString date = sql_query.value(1).toString();
                QString dateDatabase = date.left(date.length() - 15);
                while ((startDate != endDate))
                {
                    if (startDate == dateDatabase)
                    {
                        count++;
                        year = 4 -(endDate.left(startDate.length() - 3).toInt() - startDate.left(startDate.length() - 3).toInt());
                        month = startDate.right(startDate.length() - 5).toInt() - 1;
                        result[year][month] = count;
                        break;
                    }
                    else
                    {
                        afterOneMonthDateTime = afterOneMonthDateTime.addMonths(1);
                        startDate = afterOneMonthDateTime.toString("yyyy.MM");	//得到遍历的起始时间
                        nextDate = startDate;
                        count = 0;
                    }
                    startDate = nextDate;	//替换
                }
            }
        }
        m_pModifier->resetTemperatureData(result);
    }
    else
        m_strStatusBarInfo = "  🍅仓库异常";

}

/*--------------------------------------------------------
函数名：RefreshStatusBarSlot()
功能：状态烂刷新显示
创建：赵泽文
出口：
入口：
--------------------------------------------------------*/
void PomodroidoLiteMain::RefreshStatusBarSlot()
{
    m_getTime = QDateTime::currentDateTime();
    //设置系统时间显示格式
    QString qstr_time = m_getTime.toString("yyyy.MM.dd hh:mm:ss ddd");
    QString qstr_display = qstr_time + m_strStatusBarInfo;
    ui->statusBar->showMessage(QString(qstr_display));
}

/*--------------------------------------------------------
函数名：RefreshTomatoSlot()
功能：番茄钟计时更新
创建：赵泽文
出口：
入口：
--------------------------------------------------------*/
void PomodroidoLiteMain::RefreshTomatoSlot()
{
    if(TomatoPara.Minute>=0)
    {
        RefreshTimeStrDis();
        TomatoPara.Second--;
        if(TomatoPara.Second<0)
        {
           TomatoPara.Second = 59;
           TomatoPara.Minute--;
        }
    }
    else
    {
        pKeyValue++;
        ui->pushButton_create->setEnabled(true);
        QMessageBox::information(this, "提示",QStringLiteral("🍅已完成！"));
        m_strEndTime = m_getTime.toString("yyyy.MM.dd hh:mm:ss ddd");
        DBOperate->SaveToDatabase(DBConect,pKeyValue,m_strStartTime,m_strEndTime);
        qInfo () << "Pomodoro Have Finished！";
        TimerInit(TimerBeginTomato,TimerClose);
    }

}

/*--------------------------------------------------------
函数名：RefreshTimeStrDis()
功能：时间字符串刷新
创建：赵泽文
出口：
入口：
--------------------------------------------------------*/
inline void PomodroidoLiteMain::RefreshTimeStrDis()
{
    QString strMin,strSec,strTime;
    strMin = QString::number(TomatoPara.Minute);
    strSec = QString::number(TomatoPara.Second);
    if(TomatoPara.Minute < 10) strMin ='0'+strMin;
    if(TomatoPara.Second < 10) strSec ='0'+strSec;
    strTime =strMin +':'+strSec;
    ui->lcdNumber->display(strTime);
}

/*--------------------------------------------------------
函数名：ReadIniConfig()
功能：读配置表
创建：赵泽文
出口：
入口：
--------------------------------------------------------*/
void PomodroidoLiteMain::ReadIniConfig()
{
    m_pConfigTemp = new QSettings(Files_Path, QSettings::IniFormat);
    TomatoPara.Minute = m_pConfigTemp->value(CONFIG_TOMATO_TIME).toInt();
    TomatoPara.dayGoal =m_pConfigTemp->value(CONFIG_TOMATO_DAY_NUM).toInt();
    TomatoPara.monthGoal =m_pConfigTemp->value(CONFIG_TOMATO_MONTH_NUM).toInt();
    TomatoPara.yearGoal =m_pConfigTemp->value(CONFIG_TOMATO_YEAR_NUM).toInt();
    delete m_pConfigTemp;
}
/*********************************** 菜单 action ****************************************/
/*--------------------------------------------------------
函数名：actVersionMenu_clicked()
功能：菜单栏版本号
创建：赵泽文
出口：
入口：
--------------------------------------------------------*/
void PomodroidoLiteMain::actVersionMenu_clicked()
{
    AboutIntroDialog aboutIntroDialog;
    if (aboutIntroDialog.exec() == QDialog::Accepted)
    {
    }
}

/*--------------------------------------------------------
函数名：actDBQueryMenu_clicked()
功能：菜单栏数据库查询
创建：赵泽文
出口：
入口：
--------------------------------------------------------*/
void PomodroidoLiteMain::actDBQueryMenu_clicked()
{
    DBQueryDialog dbQueryDialog;
    if (dbQueryDialog.exec() == QDialog::Accepted)
    {
    }
}


void PomodroidoLiteMain::actTimeDownMenu_clicked()
{
    CountDownTimer countDownDialog;
    if (countDownDialog.exec() == QDialog::Accepted)
    {
    }
}


void PomodroidoLiteMain::mousePressEvent(QMouseEvent* event)
{

}

