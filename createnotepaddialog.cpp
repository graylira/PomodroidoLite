#include <QSettings>
#include <QMessageBox>
#include <vector>
#include <QDir>
#include <QMenu>
#include <QTextStream>
#include <iostream>
#include "ui_createnotepaddialog.h"
#include "createnotepaddialog.h"
#include "savenotepadinidialog.h"
#include "parameter_config.h"
using namespace std;
CreateNotepadDialog::CreateNotepadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateNotepadDialog)
{
    ui->setupUi(this);
    ListInit();
    ui->listWidget_record->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget_record, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));	//配置表table 增加右键菜单
}

CreateNotepadDialog::~CreateNotepadDialog()
{
    delete ui;
}

void CreateNotepadDialog::ListInit()
{
    vector<QString> vecName;
    vector<QString>::const_iterator it;
    QDir *dir = new QDir(Base_Path);
    QStringList filter;
    QString strNameTemp;
    QList<QFileInfo> *fileInfo = new QList<QFileInfo>(dir->entryInfoList(filter));

    int num = ui->listWidget_record->count();
    ui->listWidget_record->setFocus();
    for (int i = 0; i<num; i++)
    {
        QListWidgetItem*item = ui->listWidget_record->takeItem(0);
        delete item;
    }
    for (int i = 0; i<fileInfo->count(); i++)
    {
        strNameTemp = fileInfo->at(i).fileName();
        strNameTemp = strNameTemp.left(strNameTemp.length() - 4);
        vecName.push_back(strNameTemp);
    }
    if ((fileInfo->count()>0) && (!vecName.empty()))
    {
        for (it = vecName.begin() + 2; it != vecName.end(); ++it)
        {
            ui->listWidget_record->addItem(*it);
        }
    }
    delete fileInfo;
}

void CreateNotepadDialog::upLoadingSlot()
{
    SaveNotepadIniDialog saveNotepadIniDialog;
    QString curName, text, qPath;
    if (saveNotepadIniDialog.exec() == QDialog::Accepted)
    {
        curName = saveNotepadIniDialog.GetIniName();
        /*存入当前配置名到系统配置*/
        QSettings *configIniWriteSetting = new QSettings(Files_Path, QSettings::IniFormat);
        configIniWriteSetting->setValue(Config_Setting_IniName, curName);
        delete configIniWriteSetting;

        /*分选配置*/
        qPath = Base_Path + curName;
        QSettings *configIniWriteSettingUser = new QSettings(qPath, QSettings::IniFormat);

        /*用户信息配置*/
//        qStr = "ui->lineEdit_Name->text()";
//        configIniWriteSettingUser->setValue("Config_USER_ProductName", qStr);
        text = ui->textEdit_content->toPlainText();

        QFile f(qPath);
        if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
        {
           QMessageBox::information(this, "提示","写入失败！");//return -1;
        }
        QTextStream txtOutput(&f);
        txtOutput << text << endl;

        QMessageBox::information(this, "提示","日志保存成功！");
        ListInit();
        delete configIniWriteSettingUser;
    }
}



/*--------------------------------------------------------
函数名：ShowContextMenu(void)
功能：list 菜单模块
创建：赵泽文
出口：
入口：
--------------------------------------------------------*/
void CreateNotepadDialog::ShowContextMenu(const QPoint& )
{
    QMenu *cmenu = nullptr;
    if (cmenu)//保证同时只存在一个menu，及时释放内存
    {
        delete cmenu;
        cmenu = nullptr;
    }
    cmenu = new QMenu(ui->listWidget_record);
    QAction *deleteSortAction = cmenu->addAction("删除");
    connect(deleteSortAction, SIGNAL(triggered(bool)), this, SLOT(DeleteIniSort()));
    cmenu->exec(QCursor::pos());//在当前鼠标位置显示
}

/*--------------------------------------------------------
函数名：DeleteIniSort(void)
功能：list 菜单 的删除选项
创建：赵泽文
出口：
入口：
--------------------------------------------------------*/
void CreateNotepadDialog::DeleteIniSort()
{
    QString curName = ui->listWidget_record->currentItem()->text();
    QString strPath = Base_Path + curName+".txt";
    QFile file(strPath);
    QListWidgetItem* item = ui->listWidget_record->takeItem(ui->listWidget_record->currentRow());
    file.remove();
    int num = ui->listWidget_record->count();
    if (num > 0)	//重新写入
    {
        curName = ui->listWidget_record->currentItem()->text();
        curName = curName.left(curName.length() - 4);
        QSettings *configIniWriteSetting = new QSettings(Files_Path, QSettings::IniFormat);
        configIniWriteSetting->setValue(Config_Setting_IniName, curName);
        delete configIniWriteSetting;
    }
    else if (num == 0)
        QMessageBox::information(this, "提示", "无任何配置信息！");
    delete item;
}

/*--------------------------------------------------------
函数名：loadIniInfoSlot(void)
功能：tabwidget双击载入配置方案
创建：赵泽文
出口：
入口：
--------------------------------------------------------*/
void CreateNotepadDialog::loadIniInfoSlot()
{
    QString qPath,curName;
    curName = ui->listWidget_record->currentItem()->text();
    qPath = Base_Path + curName +".txt";
    QFile f(qPath);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(this, "提示","读取失败！");//return -1;
    }

    QTextStream txtInput(&f);
    QString lineStr;
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
    }
    f.close();

    ui->textEdit_content->setText(lineStr);
//    /*存入当前配置名到系统配置*/
//    curName = curName.left(curName.length() - 4);
//    QSettings *configIniWriteSetting = new QSettings(Files_Path, QSettings::IniFormat);
//    configIniWriteSetting->setValue(Config_Setting_IniName, curName);
//    delete configIniWriteSetting;
}
