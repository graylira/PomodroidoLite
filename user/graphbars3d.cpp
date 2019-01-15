#include "graphbars3d.h"
#include <QtDataVisualization/qcategory3daxis.h>
#include <QtDataVisualization/qvalue3daxis.h>
#include <QtDataVisualization/qbardataproxy.h>
#include <QtDataVisualization/q3dscene.h>
#include <QtDataVisualization/q3dcamera.h>
#include <QtDataVisualization/qbar3dseries.h>
#include <QtDataVisualization/q3dtheme.h>
#include <QtCore/QTime>
#include <QtWidgets/QComboBox>
#include <QtCore/qmath.h>
#include <QHBoxLayout>

using namespace QtDataVisualization;

const QString celsiusString ="个";

GraphBars3D::GraphBars3D(Q3DBars *bargraph, QWidget *pUi)
    : m_graph(bargraph),
      m_fXRotation(0.0f),
      m_fYRotation(0.0f),
      m_nFontSize(30),
      m_nSegments(4),
      m_nSubSegments(3),
      m_fMinval(0.0f),
      m_fMaxval(300.0f),
      m_temperatureAxis(new QValue3DAxis),
      m_yearAxis(new QCategory3DAxis),
      m_monthAxis(new QCategory3DAxis),
      m_primarySeries(new QBar3DSeries),
      m_secondarySeries(new QBar3DSeries),
      m_barMesh(QAbstract3DSeries::MeshBevelBar),
      m_bSmooth(false)
{
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftMedium);
    m_graph->activeTheme()->setBackgroundEnabled(false);
    m_graph->activeTheme()->setFont(QFont("Times New Roman", m_nFontSize));
    m_graph->activeTheme()->setLabelBackgroundEnabled(true);
    m_graph->setMultiSeriesUniform(true);

    //这里为测试用
    m_months << "一月" << "二月" << "三月" << "四月" << "五月" << "六月" << "七月" << "八月" << "九月" << "十月" << "十一月" << "十二月";
    m_years << "2015" << "2016" << "2017" << "2018";

    m_temperatureAxis->setTitle("🍅数目");
    m_temperatureAxis->setSegmentCount(m_nSegments);
    m_temperatureAxis->setSubSegmentCount(m_nSubSegments);
    m_temperatureAxis->setRange(m_fMinval, m_fMaxval);
    m_temperatureAxis->setLabelFormat(QString(QStringLiteral("%.1f ") + celsiusString));
    m_temperatureAxis->setLabelAutoRotation(30.0f);
    m_temperatureAxis->setTitleVisible(true);

    m_yearAxis->setTitle("年份");
    m_yearAxis->setLabelAutoRotation(30.0f);
    m_yearAxis->setTitleVisible(true);
    m_monthAxis->setTitle("月份");
    m_monthAxis->setLabelAutoRotation(30.0f);
    m_monthAxis->setTitleVisible(true);

    m_graph->setValueAxis(m_temperatureAxis);
    m_graph->setRowAxis(m_yearAxis);
    m_graph->setColumnAxis(m_monthAxis);

    m_primarySeries->setItemLabelFormat(QStringLiteral(" @rowLabel @colLabel: @valueLabel"));
    m_primarySeries->setMesh(QAbstract3DSeries::MeshBevelBar);
    m_primarySeries->setMeshSmooth(false);

    m_secondarySeries->setItemLabelFormat(QStringLiteral("Helsinki - @colLabel @rowLabel: @valueLabel"));
    m_secondarySeries->setMesh(QAbstract3DSeries::MeshBevelBar);
    m_secondarySeries->setMeshSmooth(false);
    m_secondarySeries->setVisible(false);

    m_graph->addSeries(m_primarySeries);
    m_graph->addSeries(m_secondarySeries);

    m_graph->setSelectionMode(QAbstract3DGraph::SelectionFlags(int(QAbstract3DGraph::SelectionSlice | QAbstract3DGraph::SelectionItemAndRow)));
    changePresetCamera();

    Q3DCamera *camera = m_graph->scene()->activeCamera();
    m_fDefaultAngleX = camera->xRotation();
    m_fDefaultAngleY = camera->yRotation();
    m_fDefaultZoom = camera->zoomLevel();
    m_defaultTarget = camera->target();

    m_animationCameraX.setTargetObject(camera);
    m_animationCameraY.setTargetObject(camera);
    m_animationCameraZoom.setTargetObject(camera);
    m_animationCameraTarget.setTargetObject(camera);

    m_animationCameraX.setPropertyName("xRotation");
    m_animationCameraY.setPropertyName("yRotation");
    m_animationCameraZoom.setPropertyName("zoomLevel");
    m_animationCameraTarget.setPropertyName("target");

    int duration = 1700;
    m_animationCameraX.setDuration(duration);
    m_animationCameraY.setDuration(duration);
    m_animationCameraZoom.setDuration(duration);
    m_animationCameraTarget.setDuration(duration);

    // The zoom always first zooms out above the graph and then zooms in
    qreal zoomOutFraction = 0.3;
    m_animationCameraX.setKeyValueAt(zoomOutFraction, QVariant::fromValue(0.0f));
    m_animationCameraY.setKeyValueAt(zoomOutFraction, QVariant::fromValue(90.0f));
    m_animationCameraZoom.setKeyValueAt(zoomOutFraction, QVariant::fromValue(50.0f));
    m_animationCameraTarget.setKeyValueAt(zoomOutFraction,
                                          QVariant::fromValue(QVector3D(0.0f, 0.0f, 0.0f)));

//    QSizeF size(1, 1);
//    m_graph->setBarSpacing(size);//修改条距
//    m_graph->setBarThickness(0.7);//修改厚度
    //修改style
    Q3DTheme *currentTheme = m_graph->activeTheme();
    QList<QColor> colors;
    colors<< "#C0FF3E" ;
    currentTheme->setBaseColors(colors);
    currentTheme->setLabelBackgroundColor(QColor(0xff,0xc1,0x25));
    currentTheme->setGridLineColor(QColor(0xff,0xc1,0x25));

    QWidget *container = QWidget::createWindowContainer(bargraph);
    QHBoxLayout *hLayout = new QHBoxLayout(pUi);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);
}



GraphBars3D::~GraphBars3D()
{
    delete m_graph;
}

void GraphBars3D::resetTemperatureData(vector<vector<int> > data)
{
    //! [5]
    // Set up data 这里为测试
    static const float tempOulu[8][12] = {

        {144, 121, 70, 23, 110, 126, 188, 138, 94, 39, 56, 0},
        {90, 152, 38, 26, 83, 159, 186, 149, 111, 53, 18, 2},
        {87, 113, 23, 4, 75, 122, 164, 141, 92, 31, 3, 121},
        {79, 53, 91, 8, 116, 166, 159, 155, 112, 40, 12, 19}
    };


    // Create data arrays
    QBarDataArray *dataSet = new QBarDataArray;
    //QBarDataArray *dataSet2 = new QBarDataArray;
    QBarDataRow *dataRow;
   // QBarDataRow *dataRow2;

    dataSet->reserve(m_years.size());
    for (int year = 0; year < m_years.size(); year++) {
        // Create a data row
        dataRow = new QBarDataRow(m_months.size());
        //dataRow2 = new QBarDataRow(m_months.size());
        for (int month = 0; month < m_months.size(); month++) {
            // Add data to the row
            if(0)
                (*dataRow)[month].setValue(tempOulu[year][month]);
            else
                (*dataRow)[month].setValue(data[year][month]);
            //(*dataRow2)[month].setValue(tempHelsinki[year][month]);
        }
        // Add the row to the set
        dataSet->append(dataRow);
       // dataSet2->append(dataRow2);
    }

    // Add data to the data proxy (the data proxy assumes ownership of it)
    m_primarySeries->dataProxy()->resetArray(dataSet, m_years, m_months);
    //m_secondarySeries->dataProxy()->resetArray(dataSet2, m_years, m_months);
    //! [5]
}


void GraphBars3D::changePresetCamera()
{
    m_animationCameraX.stop();
    m_animationCameraY.stop();
    m_animationCameraZoom.stop();
    m_animationCameraTarget.stop();

    // Restore camera target in case animation has changed it
    m_graph->scene()->activeCamera()->setTarget(QVector3D(0.0f, 0.0f, 0.0f));

    //! [10]
    static int preset = Q3DCamera::CameraPresetFront;

    m_graph->scene()->activeCamera()->setCameraPreset((Q3DCamera::CameraPreset)preset);

    if (++preset > Q3DCamera::CameraPresetDirectlyBelow)
        preset = Q3DCamera::CameraPresetFrontLow;
    //! [10]
}

QString GraphBars3D::GetCurSelect()const
{
    return QStringLiteral(" @rowLabel @colLabel: @valueLabel");
}

