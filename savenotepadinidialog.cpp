#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <qDebug>
#include "savenotepadinidialog.h"
#include "ui_savenotepadinidialog.h"
#include "parameter_config.h"

SaveNotepadIniDialog::SaveNotepadIniDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveNotepadIniDialog)
{
    ui->setupUi(this);
}

SaveNotepadIniDialog::~SaveNotepadIniDialog()
{
    delete ui;
}

bool SaveNotepadIniDialog::addNameToTxtSlot()
{
    QDir *dir = new QDir(Base_Path);
    QStringList filter;
    QList<QFileInfo> *fileInfo = new QList<QFileInfo>(dir->entryInfoList(filter));
    if(ui->lineEdit_input->text()=="")
    {
        QMessageBox::information(this, QStringLiteral("提示！"), QStringLiteral("请输入日志名称！"));
        return false;
    }
    m_strTxtName = ui->lineEdit_input->text()+".txt";
    bool bRepeat = false;
    for (int i = 0; i<fileInfo->count(); i++)
    {
        if (m_strTxtName == fileInfo->at(i).fileName())
            bRepeat = true;
        qDebug() << "已存入配置文件名："+fileInfo->at(i).fileName();
    }
    if (!bRepeat)
        accept();
    else
        QMessageBox::information(this, QStringLiteral("提示！"), QStringLiteral("日志名称重复！"));
    delete fileInfo;
    return true;
}

QString SaveNotepadIniDialog::GetIniName(void)const
{
    return m_strTxtName;
}
