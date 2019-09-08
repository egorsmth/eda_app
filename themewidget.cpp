/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <stdio.h>
#include "themewidget.h"
#include "ui_themewidget.h"
#include <model.h>
#include <float.h>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtCore/QRandomGenerator>
#include <QtCharts/QBarCategoryAxis>
#include <QtWidgets/QApplication>
#include <QtCharts/QValueAxis>

ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent),
    m_listCount(3),
    m_valueMax(10),
    m_valueCount(7),
    m_dataTable(generateRandomData(m_listCount, m_valueMax, m_valueCount)),
    m_ui(new Ui_ThemeWidgetForm)
{
    m_ui->setupUi(this);

    populateModeBox();

//    QPushButton *button = new QPushButton("click me");
//    QObject::connect(button, SIGNAL(clicked()), this, SLOT(handleButton()));
//    m_ui->gridLayout_4->addWidget(button, 0, 2);

//    chartView = new QChartView(createScatterChart());
//    m_ui->gridLayout->addWidget(chartView, 2, 2);
//    m_charts << chartView;

    // Set defaults
//    m_ui->antialiasCheckBox->setChecked(true);

    // Set the colors from the light theme as default ones
    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
    pal.setColor(QPalette::WindowText, QRgb(0x404044));
    qApp->setPalette(pal);
}

ThemeWidget::~ThemeWidget()
{
    delete m_ui;
}

DataTable ThemeWidget::generateRandomData(int listCount, int valueMax, int valueCount) const
{
    DataTable dataTable;

    // generate random data
    for (int i(0); i < listCount; i++) {
        DataList dataList;
        qreal yValue(0);
        for (int j(0); j < valueCount; j++) {
            yValue = yValue + QRandomGenerator::global()->bounded(valueMax / (qreal) valueCount);
            QPointF value((j + QRandomGenerator::global()->generateDouble()) * ((qreal) m_valueMax / (qreal) valueCount),
                          yValue);
            QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
            dataList << Data(value, label);
        }
        dataTable << dataList;
    }

    return dataTable;
}

void ThemeWidget::populateModeBox()
{
    // add items to theme combobox
    m_ui->themeComboBox->addItem("Graph", 1);
    m_ui->themeComboBox->addItem("Graph*", 2);
}

double normalize(double x, double x_min, double x_max, double s = 5.0) {
    return ((x-x_min)/(x_max-x_min) - 0.5)*2*s;
}

QChart *ThemeWidget::createChart(const DataList &list, const char* name, bool normal = true, double s = 5.0) const
{
    //![1]
    QChart *chart = new QChart();
    //![1]

    double min_x = DBL_MAX;
    double max_x = -DBL_MAX;
    double min_y = DBL_MAX;
    double max_y = -DBL_MAX;

    QLineSeries *series = new QLineSeries(chart);
    for (const Data &data : list) {
        QPointF p = data.first;
        if (p.x() > max_x) {
            max_x = p.x();
        }
        if (p.x() < min_x) {
            min_x = p.x();
        }

        if (p.y() > max_y) {
            max_y = p.y();
        }
        if (p.y() < min_y) {
            min_y = p.y();
        }
    }

    for (const Data &data : list) {
        QPointF p = data.first;
        QPointF *normalized;
        if (normal) {
            normalized = new QPointF(normalize(p.x(), min_x, max_x, 9.5), normalize(p.y(), min_y, max_y, 9.5));
        } else {
            normalized = new QPointF(p.x(), p.y());
        }
        series->append(*normalized);
    }
    series->setName(name);
    chart->addSeries(series);



    //![3]
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(-10, 10);
    chart->axes(Qt::Vertical).first()->setRange(-10, 10);
    //![3]
    //![4]
    // Add space to label to add space between labels and axis
    QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
    axisY->setLabelFormat("%.1f  ");
    //![4]

    return chart;
}

void clearLayout(QLayout * layout) {
    if (layout->children().isEmpty()) return;

        QLayoutItem *item;
        while((item = layout->takeAt(0))) {
            if (item->layout()) {
                clearLayout(item->layout());
                delete item->layout();
            }
            if (item->widget()) {
               delete item->widget();
            }
            if (item->spacerItem()) {
               delete item->spacerItem();
            }
            delete item;
        }

 }

void ThemeWidget::clearMode() {
    printf("length: %d\n", m_charts.length());
    if (!m_charts.isEmpty()) {
        m_ui->top_left_graph->removeWidget(m_charts[0]);
        m_ui->top_right_graph->removeWidget(m_charts[1]);
        m_ui->bottom_left_graph->removeWidget(m_charts[2]);
        m_ui->bottom_right_graph->removeWidget(m_charts[3]);
        m_charts.clear();
    }

}

const int MODE_GRAPH = 1;
const int MODE_GRAPH_STAR = 2;

void ThemeWidget::renderModeGraph() {
    //create charts
    QChartView *chartView;

    if (!m_charts.isEmpty()) {
        m_charts[0]->setChart(createChart(Model::transformTimeseriesForView(Model::getLine(3, 0, 10)), "line"));
        m_charts[1]->setChart(createChart(Model::transformTimeseriesForView(Model::getLine(-3, 0, 10)), "-line"));
        m_charts[2]->setChart(createChart(Model::transformTimeseriesForView(Model::getExp(1, 1, 10)), "exp"));
        m_charts[3]->setChart(createChart(Model::transformTimeseriesForView(Model::getExp(-1, 1, 10)), "-exp"));
    } else {
        chartView = new QChartView(createChart(Model::transformTimeseriesForView(Model::getLine(3, 0, 10)), "line"));
        m_ui->top_left_graph->addWidget(chartView);
        m_charts << chartView;

        chartView = new QChartView(createChart(Model::transformTimeseriesForView(Model::getLine(-3, 0, 10)), "-line"));
        m_ui->top_right_graph->addWidget(chartView);
        m_charts << chartView;

        chartView = new QChartView(createChart(Model::transformTimeseriesForView(Model::getExp(1, 1, 10)), "exp"));
        m_ui->bottom_left_graph->addWidget(chartView);
        m_charts << chartView;

        chartView = new QChartView(createChart(Model::transformTimeseriesForView(Model::getExp(-1, 1, 10)), "-exp 6"));
        m_ui->bottom_right_graph->addWidget(chartView);
        m_charts << chartView;
    }
}

std::vector<Point> combine(std::vector<Point> a, std::vector<Point> b) {
    std::vector<Point> res;
    for (unsigned long i = 0; i < a.size(); i++) {
        Point p;
        p.x = a[i].x;
        p.y = a[i].y * b[i].y;
        res.push_back(p);
    }

    return res;
}

void ThemeWidget::renderModeGraphStar() {
    //create charts
    QChartView *chartView;

    if (!m_charts.isEmpty()) {
        std::vector<Point> a = Model::getLine(-0.5, 0, 10);
        std::vector<Point> b = Model::getSin(10);
        std::vector<Point> c = Model::getExp(0.4, 1, 10);
        std::vector<Point> d = combine(combine(b, c), a);

        m_charts[0]->setChart(createChart(Model::transformTimeseriesForView(a), "line"));
        m_charts[1]->setChart(createChart(Model::transformTimeseriesForView(b), "sin"));
        m_charts[2]->setChart(createChart(Model::transformTimeseriesForView(c), "exp"));
        m_charts[3]->setChart(createChart(Model::transformTimeseriesForView(d), "line * sin * exp"));
    } else {
        chartView = new QChartView(createChart(Model::transformTimeseriesForView(Model::getLine(3, 0, 10)), "line 2"));
        m_ui->top_left_graph->addWidget(chartView);
        m_charts << chartView;

        chartView = new QChartView(createChart(Model::transformTimeseriesForView(Model::getLine(-3, 0, 10)), "line 4"));
        m_ui->top_right_graph->addWidget(chartView);
        m_charts << chartView;

        chartView = new QChartView(createChart(Model::transformTimeseriesForView(Model::getExp(1, 1, 10)), "exp 5"));
        m_ui->bottom_left_graph->addWidget(chartView);
        m_charts << chartView;

        chartView = new QChartView(createChart(Model::transformTimeseriesForView(Model::getExp(-1, 1, 10)), "-exp 6"));
        m_ui->bottom_right_graph->addWidget(chartView);
        m_charts << chartView;

    }
}

void ThemeWidget::updateUI()
{
    int mode = m_ui->themeComboBox->itemData(m_ui->themeComboBox->currentIndex()).toInt();

    switch (mode) {
    case MODE_GRAPH:
        renderModeGraph();
        break;
    case MODE_GRAPH_STAR:
        renderModeGraphStar();
        break;
    default:
        printf("Unknown mode %d\n", mode);
    }

    const auto charts = m_charts;

    for (QChartView *chart : charts)
        chart->setRenderHint(QPainter::Antialiasing, true);

    Qt::Alignment alignment(Qt::AlignTop);

    if (!alignment) {
        for (QChartView *chartView : charts)
            chartView->chart()->legend()->hide();
    } else {
        for (QChartView *chartView : charts) {
            chartView->chart()->legend()->setAlignment(alignment);
            chartView->chart()->legend()->show();
        }
    }
    //![10]
}
