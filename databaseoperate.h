#ifndef DATABASEOPERATE_H
#define DATABASEOPERATE_H
/*--------------------
 * QSqlDatabase类实现了数据库连接的操作
 * QSqlQuery类执行SQL语句
 * QSqlRecord类封装数据库所有记录
--------------------*/
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QObject>

class DatabaseOperate : public QObject, public QSqlDatabase
{
    Q_OBJECT
public:
   explicit DatabaseOperate();
    ~DatabaseOperate();
    uint32_t GetLastPKey();
    uint8_t CreateConnect(QSqlDatabase database,const QString& connectName,const QString& DBtype);
    uint8_t CreateConnect(QSqlDatabase database,const QString& connectName,const QString& DBtype,const QString& hostName,const QString& DBname
                          ,const QString& userName,const QString& password);
    bool SaveToDatabase(QSqlDatabase database, const int &pkey, const QString& keystart, const QString& keyend);
private:
    uint32_t m_nPKey;
    QString m_strKeyStartTime;
    QString m_strKeyEndTime;
};

#endif // DATABASEOPERATE_H
