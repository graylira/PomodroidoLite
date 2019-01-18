#ifndef GRAPHBARS3D_H
#define GRAPHBARS3D_H

#include <QObject>
#include <QtDataVisualization/q3dbars.h>
#include <QtDataVisualization/qbardataproxy.h>
#include <QtDataVisualization/qabstract3dseries.h>

#include <QtGui/QFont>
#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QPointer>
#include <QtCore/QPropertyAnimation>

#include <vector>
using namespace std;
using namespace QtDataVisualization;

class GraphBars3D : public QObject
{
public:
    explicit GraphBars3D(Q3DBars *bargraph,QWidget* pUi,const QDateTime& dateTime);
    ~GraphBars3D();
    QString GetCurSelect()const;

public Q_SLOTS:
   void resetTemperatureData(vector<vector<int>> data);
   void changePresetCamera();
private:
    Q3DBars *m_graph;
    float m_fXRotation;
    float m_fYRotation;
    int m_nFontSize;
    int m_nSegments;
    int m_nSubSegments;
    float m_fMinval;
    float m_fMaxval;
    QStringList m_months;
    QStringList m_years;
    QValue3DAxis *m_temperatureAxis;
    QCategory3DAxis *m_yearAxis;
    QCategory3DAxis *m_monthAxis;
    QBar3DSeries *m_primarySeries;
    QBar3DSeries *m_secondarySeries;
    QAbstract3DSeries::Mesh m_barMesh;
    bool m_bSmooth;
    QPropertyAnimation m_animationCameraX;
    QPropertyAnimation m_animationCameraY;
    QPropertyAnimation m_animationCameraZoom;
    QPropertyAnimation m_animationCameraTarget;
    float m_fDefaultAngleX;
    float m_fDefaultAngleY;
    float m_fDefaultZoom;
    QVector3D m_defaultTarget;
};

#endif // GRAPHBARS3D_H
