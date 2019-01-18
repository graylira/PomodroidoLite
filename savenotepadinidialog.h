#ifndef SAVENOTEPADINIDIALOG_H
#define SAVENOTEPADINIDIALOG_H

#include <QDialog>

namespace Ui {
class SaveNotepadIniDialog;
}

class SaveNotepadIniDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveNotepadIniDialog(QWidget *parent = 0);
    ~SaveNotepadIniDialog();
    QString GetIniName()const;
public slots:
    bool addNameToTxtSlot();

private:
    Ui::SaveNotepadIniDialog *ui;
    QString m_strTxtName;
};

#endif // SAVENOTEPADINIDIALOG_H
