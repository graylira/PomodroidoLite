#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlTableModel>
#include "dbquerydialog.h"
#include "ui_dbquerydialog.h"

DBQueryDialog::DBQueryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBQueryDialog)
{
    ui->setupUi(this);

    if (QSqlDatabase::contains("my_LocalDataBase"))
    {
        database = QSqlDatabase::database("my_LocalDataBase");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("PomodroidoDB.db");
    }

    if (!database.open())
    {
        QMessageBox::information(this, "提示", QStringLiteral("数据库未连接！"));
    }
    else
    {
        QSqlTableModel *sqlModelAll = new QSqlTableModel();
        sqlModelAll->setTable("PomodoroList");	//选择表
        sqlModelAll->setEditStrategy(QSqlTableModel::OnManualSubmit);
        sqlModelAll->select(); //显示结果
        while (sqlModelAll->canFetchMore())
        {
            sqlModelAll->fetchMore();		//获取全部结果//
        }
        int m_nRowCount = sqlModelAll->rowCount();
        if (m_nRowCount >= 300)
        {
            QMessageBox message(QMessageBox::Warning, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("数据已经超过300条！是否继续？"),
                QMessageBox::Yes | QMessageBox::No, NULL);
            if (message.exec() == QMessageBox::No)
            {
                //return false;
            }
        }
        sqlModelAll->setHeaderData(0, Qt::Horizontal, "番茄号");
        sqlModelAll->setHeaderData(1, Qt::Horizontal, "起始时间");
        sqlModelAll->setHeaderData(2, Qt::Horizontal, "结束时间");
        ui->tableView_DBquery->setModel(sqlModelAll);

    }
    ui->tableView_DBquery->setColumnWidth(0, 50);
    ui->tableView_DBquery->setColumnWidth(1, 180);
    ui->tableView_DBquery->setColumnWidth(2, 180);

}

DBQueryDialog::~DBQueryDialog()
{
    delete ui;
}
