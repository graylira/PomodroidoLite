#ifndef DBQUERYDIALOG_H
#define DBQUERYDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
namespace Ui {
class DBQueryDialog;
}

class DBQueryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DBQueryDialog(QWidget *parent = 0);
    ~DBQueryDialog();

private:
    Ui::DBQueryDialog *ui;
    QSqlDatabase database;
};

#endif // DBQUERYDIALOG_H
