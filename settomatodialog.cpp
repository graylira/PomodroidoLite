#include <QSettings>
#include "settomatodialog.h"
#include "ui_settomatodialog.h"
#include "QMessageBox"
#include "parameter_config.h"
#include "pomodroidolitemain.h"

extern TomatoTiming TomatoTime;

SetTomatoDialog::SetTomatoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetTomatoDialog)
{
    ui->setupUi(this);
    QSettings *configIniReadSettingUser = new QSettings(Files_Path, QSettings::IniFormat);
    m_strSetNum = configIniReadSettingUser->value(CONFIG_TOMATO_TIME).toString();
    ui->lineEdit_setTime->setText(QString(m_strSetNum));
    m_strSetNum = configIniReadSettingUser->value(CONFIG_TOMATO_DAY_NUM).toString();
    ui->lineEdit_day->setText(QString(m_strSetNum));
    m_strSetNum = configIniReadSettingUser->value(CONFIG_TOMATO_MONTH_NUM).toString();
    ui->lineEdit_month->setText(QString(m_strSetNum));
    m_strSetNum = configIniReadSettingUser->value(CONFIG_TOMATO_YEAR_NUM).toString();
    ui->lineEdit_year->setText(QString(m_strSetNum));
    delete configIniReadSettingUser;
}

SetTomatoDialog::~SetTomatoDialog()
{
    delete ui;
}
void SetTomatoDialog::setOkSlot()
{

    if((ui->lineEdit_day->text() =="")||(ui->lineEdit_month->text() =="")||(ui->lineEdit_year->text() ==""))
    {
        QMessageBox::information(this, "提示",QStringLiteral("有目标没有输入噢！"));
    }
    else
    {
        QSettings *configIniWriteSettingUser = new QSettings(Files_Path, QSettings::IniFormat);
        m_strSetNum = ui->lineEdit_setTime->text();
        configIniWriteSettingUser->setValue(CONFIG_TOMATO_TIME, m_strSetNum);
        m_strSetNum = ui->lineEdit_day->text();
        configIniWriteSettingUser->setValue(CONFIG_TOMATO_DAY_NUM, m_strSetNum);
        m_strSetNum = ui->lineEdit_month->text();
        configIniWriteSettingUser->setValue(CONFIG_TOMATO_MONTH_NUM, m_strSetNum);
        m_strSetNum = ui->lineEdit_year->text();
        configIniWriteSettingUser->setValue(CONFIG_TOMATO_YEAR_NUM, m_strSetNum);
        delete configIniWriteSettingUser;
        accept();
    }
}
