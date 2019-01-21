#ifndef CREATENOTEPADDIALOG_H
#define CREATENOTEPADDIALOG_H

#include <QDialog>

namespace Ui {
class CreateNotepadDialog;
}

class CreateNotepadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateNotepadDialog(QWidget *parent = 0);
     CreateNotepadDialog(const unsigned int &PomodroidoID, QWidget *parent = 0);
    ~CreateNotepadDialog();

private:
    Ui::CreateNotepadDialog *ui;
    unsigned int m_nPomodroidoID;

public slots:
    bool upLoadingSlot();
    void ShowContextMenu(const QPoint& );
    void DeleteIniSort();
    void loadIniInfoSlot();
    void ListInit();
};

#endif // CREATENOTEPADDIALOG_H
