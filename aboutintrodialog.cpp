#include "aboutintrodialog.h"
#include "ui_aboutintrodialog.h"

AboutIntroDialog::AboutIntroDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutIntroDialog)
{
    ui->setupUi(this);
}

AboutIntroDialog::~AboutIntroDialog()
{
    delete ui;
}
