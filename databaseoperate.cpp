/*-----------------------------
-	DatabaseOperate.cpp
-	创建人：赵泽文
-	时间：2018.11.23
- 文件说明：用于多种数据库控制
-----------------------------*/
#include <QMessageBox>
#include "databaseoperate.h"
//#include "parameter_config.h"

/*--------------------------------------------------------
函数名：DatabaseOperate()
功能：构造函数
创建：赵泽文
出口：
入口：
--------------------------------------------------------*/
DatabaseOperate::DatabaseOperate()
{

}
/*--------------------------------------------------------
函数名：~DatabaseOperate()
功能：析构函数
创建：赵泽文
出口：
入口：定时器号,开关控制
--------------------------------------------------------*/
DatabaseOperate::~DatabaseOperate()
{

}
/*--------------------------------------------------------
函数名：CreateConnect(const QString& connectName,const QString& DBtype)
功能：创建一次连接
创建：赵泽文
出口：
入口：
QSqlError::NoError	0	无错语
QSqlError::ConnectionError	1	连接错误
QSqlError::StatementError	2	语句错语
QSqlError::TransactionError	3	事务方面错误（可能不支持事务）
QSqlError::UnknownError	4	未知错误
--------------------------------------------------------*/
uint8_t DatabaseOperate::CreateConnect(QSqlDatabase database,const QString& connectName,const QString& DBtype)
{

    QSqlError error;
    if (QSqlDatabase::contains(connectName))
    {
        database = QSqlDatabase::database(connectName);
    }
    else
    {
        database = QSqlDatabase::addDatabase(DBtype);
        database.setDatabaseName("PomodroidoDB.db");
        //database.setUserName("zhaozw");
        //database.setPassword("123456");
    }
    if (!database.open())
    {
        qWarning() << "Error: Failed to connect database." << database.lastError();
        error = database.lastError();

        if(error.isValid())//发生错误时isValid()返回true
        {
            switch (error.type())
            {
            case QSqlError::ConnectionError://连接错语
                qWarning()<<error.text();
                break;
            case QSqlError::StatementError://语句错语
                qWarning()<<error.text();
                break;
            case QSqlError::TransactionError://事务错误
                qWarning()<<error.text();
                break;
            default://未知错误
                qWarning()<<error.text();
                break;
            }
        }
        return error.type();
    }
    else
    {
        qInfo () << "The Database has been connected successfully!";
    }
    return 0;
}


bool DatabaseOperate::SaveToDatabase(QSqlDatabase database,const int& pkey,const QString& keystart,const QString& keyend)
{
    database = QSqlDatabase::database("my_LocalDataBase"); //建立数据库连接
    QSqlQuery query(database);

    query.prepare("INSERT INTO PomodoroList (pkey,begin,end)"
        "VALUES (:pkey, :begin, :end)");
    query.bindValue(":pkey", pkey);
    query.bindValue(":begin", keystart);
    query.bindValue(":end", keyend);

    if (!query.exec())
    {
        QSqlError lastError = query.lastError();
        qWarning() << lastError.driverText() << QString(QObject::tr("INSERT failed."));
    }
    return 0;
}

uint32_t DatabaseOperate::GetLastPKey()
{
    QSqlQuery query;
    query.exec("select * from PomodoroList");
    if(query.last())
    {
      qDebug() << "rowNum is : " << query.at()
                << " id is : " << query.value(0).toInt()
                << " name is : " << query.value(1).toString();
    }
    return query.value(0).toInt();
}
