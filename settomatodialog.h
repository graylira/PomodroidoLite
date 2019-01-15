#ifndef SETTOMATODIALOG_H
#define SETTOMATODIALOG_H

#include <QDialog>

namespace Ui {
class SetTomatoDialog;
}

class SetTomatoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetTomatoDialog(QWidget *parent = 0);
    ~SetTomatoDialog();
public slots:
    void setOkSlot();
private:
    Ui::SetTomatoDialog *ui;
    QString m_strSetNum;
};

#endif // SETTOMATODIALOG_H
