#ifndef ABOUTINTRODIALOG_H
#define ABOUTINTRODIALOG_H

#include <QDialog>

namespace Ui {
class AboutIntroDialog;
}

class AboutIntroDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutIntroDialog(QWidget *parent = 0);
    ~AboutIntroDialog();

private:
    Ui::AboutIntroDialog *ui;
};

#endif // ABOUTINTRODIALOG_H
