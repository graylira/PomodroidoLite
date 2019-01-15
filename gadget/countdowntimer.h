#ifndef COUNTDOWNTIMER_H
#define COUNTDOWNTIMER_H
#include<QTimer>
#include<QDateTime>
#include <QMainWindow>
#include <QDialog>

namespace Ui {
class CountDownTimer;
}

class CountDownTimer : public QDialog
{
    Q_OBJECT

public:
    explicit CountDownTimer(QWidget *parent = 0);
    ~CountDownTimer();

private:
    Ui::CountDownTimer *ui;
    QTimer* m_pRefreshTimer;
    QDateTime dateTime;
    QDateTime dateTimeCur;
    float m_fValue;
public slots:
    void StartTimerBtn();
    void RefreshTimerSlot();
};

#endif // COUNTDOWNTIMER_H
