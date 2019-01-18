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
    ~CreateNotepadDialog();

private:
    Ui::CreateNotepadDialog *ui;

public slots:
    void upLoadingSlot();
    void ShowContextMenu(const QPoint& );
    void DeleteIniSort();
    void loadIniInfoSlot();
    void ListInit();
};

#endif // CREATENOTEPADDIALOG_H
