#ifndef POMODROIDOLITEMAIN_H
#define POMODROIDOLITEMAIN_H

#include <QMainWindow>
#include <QSettings>
#include <vector>
#include "user/graphbars3d.h"
#include "databaseoperate.h"

using namespace std;

namespace Ui {
class PomodroidoLiteMain;
}

class PomodroidoLiteMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit PomodroidoLiteMain(QWidget *parent = 0);
    ~PomodroidoLiteMain();
    void TimerInit(unsigned int timerNo, bool control);
    inline void RefreshTimeStrDis();
    void ReadIniConfig();
    void DataLoadInit(const QDateTime &dateTime);

protected:
    //鼠标点击事件
    void mousePressEvent(QMouseEvent* event);
//    //鼠标移动事件
//    void mouseMoveEvent(QMouseEvent *event);
//    //鼠标释放事件
//    void mouseReleaseEvent(QMouseEvent *event);

public slots:
    void setTomatoSlot();
    void createTomatoSlot();
    void discardTomatoSlot();
    void createDiarySlot();
    void RefreshStatusBarSlot();
    void RefreshTomatoSlot();
    void actVersionMenu_clicked();
    void actDBQueryMenu_clicked();
    void actTimeDownMenu_clicked();
    void action_calculatorLite();
private:
    Ui::PomodroidoLiteMain *ui;
    GraphBars3D *m_pModifier;
    Q3DBars *m_pBars;
    QTimer *m_pRefreshStatusBarTimer;
    QTimer *m_pRefreshTomatoTimer;
    QSettings *m_pConfigTemp;
    QString m_strStatusBarInfo;
    DatabaseOperate *DBOperate;
    QSqlDatabase DBConect;
    QString m_strStartTime;
    QString m_strEndTime;
    QDateTime m_getTime;
    uint32_t m_nKeyValue;
    QDateTime m_dateTime;
};

#endif // POMODROIDOLITEMAIN_H
