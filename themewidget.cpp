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
#include <transform.h>
#include <analysis.h>


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
    this->setMouseTracking(true);


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
    m_ui->themeComboBox->addItem("In class", 3);
    m_ui->themeComboBox->addItem("In class 2", 4);
    m_ui->themeComboBox->addItem("In class 3", 5);
    m_ui->themeComboBox->addItem("In class 4", 6);
    m_ui->themeComboBox->addItem("In class 5 (distr)", 7);

    m_ui->themeComboBox->addItem("In class, fourier", 8);
    m_ui->themeComboBox->addItem("In class, fourier 2", 9);
    m_ui->themeComboBox->addItem("In class, fourier 4", 10);
    m_ui->themeComboBox->addItem("In class, ANTI", 11);
    m_ui->themeComboBox->addItem("In class, slide avg", 12);
    m_ui->themeComboBox->addItem("In class, spect 1", 13);
    m_ui->themeComboBox->addItem("In class, spect 2", 14);
    m_ui->themeComboBox->addItem("In class, spect 3", 15);
    m_ui->themeComboBox->addItem("In class, spect 4", 16);
    m_ui->themeComboBox->addItem("In class, spect 6", 17);
    m_ui->themeComboBox->addItem("Home Work", 18);
    m_ui->themeComboBox->addItem("In class reverse fourier", 19);
    m_ui->themeComboBox->addItem("In class window", 20);
    m_ui->themeComboBox->addItem("HW autocorell", 21);

    m_ui->themeComboBox->addItem("In class filters 1", 22);
    m_ui->themeComboBox->addItem("In class filters 2", 23);
    m_ui->themeComboBox->addItem("In class eq filters 3", 24);
    m_ui->themeComboBox->addItem("In class eq filters 4 ver 2", 25);
    m_ui->themeComboBox->addItem("In class eq filters 5", 26);
    m_ui->themeComboBox->addItem("In class eq filters 6", 27);

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
            normalized = new QPointF(normalize(p.x(), min_x, max_x, s), normalize(p.y(), min_y, max_y, s));
        } else {
            normalized = new QPointF(p.x(), p.y());
        }
        series->append(*normalized);
    }
    series->setName(name);
    chart->addSeries(series);



    //![3]

    double left_h = -s;
    double right_h = s;
    double top_v = s;
    double bot_v = -s;
    double delta_x = 0;
    double delta_y = 0;

    if (!normal) {
        double diff_x = max_x - min_x;
        double diff_y = max_y - min_y;
        delta_x = diff_x < 2 ? diff_x*0.2 : 2;
        delta_y = diff_y < 2 ? diff_y*0.2 : 2;

        left_h = min_x - delta_x;
        right_h = max_x + delta_x;
        top_v = max_y + delta_y;
        bot_v = min_y - delta_y;
    }


    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(left_h, right_h);
    qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).first())->setTickCount(11);
    qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).first())->setMinorTickCount(4);
    chart->axes(Qt::Vertical).first()->setRange(bot_v, top_v);
    qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first())->setTickCount(11);
    qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first())->setMinorTickCount(4);



    //    QValueAxis *axisX = new QValueAxis;
//    axisX->setRange(10, 20.5);
//    axisX->setTickCount(10);
//    axisX->setLabelFormat("%.2f");
//    chartView->chart()->setAxisX(axisX, series);

    //![3]
    //![4]
    // Add space to label to add space between labels and axis
    QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
    axisY->setLabelFormat("%.1f  ");
    //![4]
    chart->setAcceptHoverEvents(true);
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
const int IN_CLASS = 3;
const int IN_CLASS2 = 4;
const int IN_CLASS3 = 5;
const int IN_CLASS4 = 6;
const int IN_CLASS5 = 7;
const int IN_CLASS_fourier = 8;
const int IN_CLASS_fourier2 = 9;
const int IN_CLASS_fourier4 = 10;
const int IN_CLASS_ANTISHIFT_ANTISPYKES = 11;
const int IN_CLASS_SLIDE_AVG = 12;
const int IN_CLASS_SPEC_ANALYS = 13;
const int IN_CLASS_SPEC_ANALYS2 = 14;
const int IN_CLASS_SPEC_ANALYS3 = 15;
const int IN_CLASS_SPEC_ANALYS4 = 16;
const int IN_CLASS_SPEC_ANALYS5 = 17;
const int HOME_WORK = 18;
const int IN_CLASS_REVERSE_FOURIER = 19;
const int IN_CLASS_WINDOW = 20;
const int HW_AUTOCORELL = 21;

const int IN_CLASS_FILTER1 = 22;
const int IN_CLASS_FILTER2 = 23;
const int IN_CLASS_FILTER3 = 24;
const int IN_CLASS_FILTER4 = 25;
const int IN_CLASS_FILTER5 = 26;
const int IN_CLASS_FILTER6 = 27;

void ThemeWidget::renderModeGraph() {
    //create charts
    QChartView *chartView;

    if (!m_charts.isEmpty()) {
        m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(Model::getLine(3, 0, 10)), "line"));
        m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(Model::getLine(-3, 0, 10)), "-line"));
        m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(Model::getExp(1, 1, 10)), "exp"));
        m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(Model::getExp(-1, 1, 10)), "-exp"));
    } else {
        chartView = new QChartView(createChart(transform::transformTimeseriesForView(Model::getLine(3, 0, 10)), "line"));
        m_ui->top_left_graph->addWidget(chartView);
        m_charts << chartView;

        chartView = new QChartView(createChart(transform::transformTimeseriesForView(Model::getLine(-3, 0, 10)), "-line"));
        m_ui->top_right_graph->addWidget(chartView);
        m_charts << chartView;

        chartView = new QChartView(createChart(transform::transformTimeseriesForView(Model::getExp(1, 1, 10)), "exp"));
        m_ui->bottom_left_graph->addWidget(chartView);
        m_charts << chartView;

        chartView = new QChartView(createChart(transform::transformTimeseriesForView(Model::getExp(-1, 1, 10)), "-exp 6"));
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

        m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(a), "line"));
        m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(b), "sin"));
        m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(c), "exp"));
        m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(d), "line * sin * exp"));
    } else {
        chartView = new QChartView(createChart(transform::transformTimeseriesForView(Model::getLine(3, 0, 10)), "line 2"));
        m_ui->top_left_graph->addWidget(chartView);
        m_charts << chartView;

        chartView = new QChartView(createChart(transform::transformTimeseriesForView(Model::getLine(-3, 0, 10)), "line 4"));
        m_ui->top_right_graph->addWidget(chartView);
        m_charts << chartView;

        chartView = new QChartView(createChart(transform::transformTimeseriesForView(Model::getExp(1, 1, 10)), "exp 5"));
        m_ui->bottom_left_graph->addWidget(chartView);
        m_charts << chartView;

        chartView = new QChartView(createChart(transform::transformTimeseriesForView(Model::getExp(-1, 1, 10)), "-exp 6"));
        m_ui->bottom_right_graph->addWidget(chartView);
        m_charts << chartView;

    }
}

void ThemeWidget::renderModeInClass() {
    // spikes, shift, avg and var
    std::vector<Point> a = Model::getRandom(1000, 0.3);
    std::vector<Point> b = Model::getRandomSelf(1000, 100, 3.4, 1.7, 17);
    std::vector<Point> c = Model::getSpikes(1000, 5, 3);

    std::pair< std::vector<Point>, std::vector<Point> > f = analysis::getAvgsAndVars(a, 10);

    printf("%d\n", analysis::isStationar(a, 10, 0.05));
    printf("%d\n", analysis::isStationar(b, 10, 0.05));
    printf("%d\n", analysis::isStationar(c, 10, 0.05));

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(a), "random", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(b), "getRandomSelf", false));
    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(c), "getSpikes", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(transform::shift(a, 10, 60, 20)), "shift", false));
}

void ThemeWidget::renderModeInClass2() {
    // average variance
    std::vector<Point> a = Model::getRandom(10000, 100.0);
    std::vector<Point> b = Model::getRandomSelf(1000, 100, 3.4, 1.7, 17);
    std::vector<Point> c = Model::getSpikes(1000, 5, 3);
    std::vector<Point> d = Model::getRandomSpikes(1000, 10, 10, 3);

    std::pair< std::vector<Point>, std::vector<Point> > f = analysis::getAvgsAndVars(a, 10);

    printf("%d\n", analysis::isStationar(a, 10, 0.05));
    printf("%d\n", analysis::isStationar(b, 10, 0.05));
    printf("%d\n", analysis::isStationar(c, 10, 0.05));

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(a), "random", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(f.first), "average", false));
    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(f.second), "variance", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(d), "randomspikes", false));
}

void ThemeWidget::renderModeInClass3() {
    // additive multiplicative
    std::vector<Point> a = Model::getRandom(1000, 100.3);
    std::vector<Point> b = Model::getLine(1.1, 1.1, 1000);
    std::vector<Point> c = Model::getLine(-1.1, 1300.1, 1000);

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(transform::additive(a, b)), "++", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(transform::multiplicative(a, b)), "*+", false));
    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(transform::additive(a, c)), "+-", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(transform::multiplicative(a, c)), "*-", false));
}

void ThemeWidget::renderModeInClass4() {
    // подавление шума
    std::vector<Point> a = Model::getRandom(1000, 100.3);
    std::vector<Point> b = Model::getRandom(1000, 100.3);
    std::vector<Point> c = Model::getRandom(1000, 100.3);
    std::vector<Point> d = Model::getRandom(1000, 100.3);

    for (int i = 0; i < 10; ++i) {
        b = transform::additive(b, Model::getRandom(1000, 100.3));
    }
    for (int i = 0; i < 100; ++i) {
        c = transform::additive(b, Model::getRandom(1000, 100.3));
    }
    for (int i = 0; i < 1000; ++i) {
        d = transform::additive(b, Model::getRandom(1000, 100.3));
    }

    for (int i = 0; i < 1000; ++i) {
        Point p;
        p.x = b[i].x;
        p.y = b[i].y/10;
        b[i] = p;
    }

    for (int i = 0; i < 1000; ++i) {
        Point p;
        p.x = c[i].x;
        p.y = c[i].y/100;
        c[i] = p;
    }

    for (int i = 0; i < 1000; ++i) {
        Point p;
        p.x = d[i].x;
        p.y = d[i].y/1000;
        d[i] = p;
    }

    printf("%f\n", analysis::getStd(a));
    printf("%f\n", analysis::getStd(b));
    printf("%f\n", analysis::getStd(c));
    printf("%f\n", analysis::getStd(d));

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(a), "++", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(b), "*+", false));
    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(c), "+-", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(d), "*-", false));
}

void ThemeWidget::renderModeInClass5() {
    // additive multiplicative
    std::vector<Point> a = Model::getRandomSelf(1000, 100, 1.6, 1.3, 13);
    std::vector<Point> b = Model::getAutoCorrelartionFunc(a);

    std::vector<Point> c = Model::getRandomSelf(1000, 100, 3.4, 1.7, 17);
    std::vector<Point> d = Model::getCorrelartionFunc(a, c);



    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(a), "random", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(c), "randomSelf", false));
    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(b), "distr random", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(d), "cross", false));
}


void ThemeWidget::renderModeInClassFourier() {
    // additive multiplicative
    std::vector<Point> a = Model::fourier(100, 11.0, 1000, 0.001);
    std::vector<Point> b = Model::fourier(100, 110.0, 1000, 0.001);

    std::vector<Point> c = Model::fourier(100, 250.0, 1000, 0.001);
    std::vector<Point> d = Model::fourier(100, 510.0, 1000, 0.001);



    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(a), "random", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(c), "randomSelf", false));
    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(b), "distr random", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(d), "cross", false));
}

void ThemeWidget::renderModeInClassFourier2() {
    std::vector<Point> a = Model::fourier(100, 11.0, 1000, 0.001);
    std::vector<Point> b = Model::fourier(100, 110.0, 1000, 0.001);

    std::vector<Point> c = Model::fourier(100, 250.0, 1000, 0.001);
    std::vector<Point> d = Model::fourier(100, 510.0, 1000, 0.001);

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(a)), "random", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(b)), "randomSelf", false));
    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(c)), "distr random", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(d)), "cross", false));
}

//void ThemeWidget::renderModeInClassFourier3() {
//// fourier to freq, пперевести из фурье обратно в частоту
//}

void ThemeWidget::renderModeInClassFourier4() {
    // TODO доделать тоже самое что фурье ин класс 1 + фурье ин класс 2
    std::vector<Point> a = Model::fourier(25, 11.0, 1000, 0.001);
    std::vector<Point> b = Model::fourier(35, 41.0, 1000, 0.001);

    std::vector<Point> c = Model::fourier(30, 141.0, 1000, 0.001);
    std::vector<Point> d = transform::additive(c, transform::additive(a, b));

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(a), "random", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(c), "randomSelf", false));
    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(b), "distr random", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(d)), "cross", false));
}

void ThemeWidget::renderModeInClassAntiShiftANtiSpykes() {

    std::vector<Point> a = Model::getRandom(1000, 10);
    std::vector<Point> b = Model::getSpikes(1000, 8, 100);
    std::vector<Point> d = transform::shift(a, 1, 1000, 10000);


    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(b), "original spiked", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(d), "original shifted", false));

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(transform::antiShift(d)), "antishift", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(transform::antiSpike(b, 110)), "antispike", false));
}

void ThemeWidget::renderModeInClassSlideAvg() {

    std::vector<Point> d = Model::getRandom(1000, 10);
    std::vector<Point> c = Model::getLine(1.5, 10, 1000);
    std::vector<Point> b = analysis::slideAvg(transform::additive(d, c), 10);

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(transform::additive(d, c)), "original", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(c), "original", false));

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(b), "slide", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(transform::additive(b, transform::additive(d, c), true)), "antispike", false));
}

void ThemeWidget::renderModeInClassSpectrAnalysis() {

    std::vector<Point> a = Model::fourier(10, 15, 1000, 0.001);
    std::vector<Point> b = Model::getLine(0, 1000, 1000);

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(a)), "original", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(transform::additive(a, b))), "original", false));

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(a), "slide", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(transform::additive(a, b)), "antispike", false));
}

void ThemeWidget::renderModeInClassSpectrAnalysis2() {

    std::vector<Point> a = Model::fourier(10, 15, 1000, 0.001);
    std::vector<Point> b = Model::getLine(0, 1000, 1000);
    std::vector<Point> c = transform::additive(a, b);

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(c)), "original", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(transform::antiShift(c))), "original", false));

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(c), "slide", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(transform::antiShift(c)), "antispike", false));
}

void ThemeWidget::renderModeInClassSpectrAnalysis3() {

    std::vector<Point> a = Model::fourier(10, 15, 1000, 0.001);
    std::vector<Point> b = Model::getLine(0.9, 0, 1000);
    std::vector<Point> c = transform::additive(a, b);
    std::vector<Point> antiTrend = analysis::slideAvg(c, 6);

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(c)), "original", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(transform::antiShift(transform::additive(antiTrend, c, true)))), "original", false));

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(c), "slide", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(transform::additive(antiTrend, c, true)), "antispike", false));
}

void ThemeWidget::renderModeInClassSpectrAnalysis4() {

    std::vector<Point> a = Model::getRandom(1000, 10);
    std::vector<Point> b = Model::getPureSpikes(1000, 20, 100);
    std::vector<Point> c = transform::ampSpecter(a);
    std::vector<Point> d = transform::ampSpecter(b);

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(a), "a", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(b), "b", false));

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(c), "c", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(d), "d", false));
}

void ThemeWidget::renderModeInClassSpectrAnalysis5() {

    std::vector<Point> a = Model::getRandom(1000, 30);
    std::vector<Point> b = Model::getPureSpikes(1000, 20, 100);
    std::vector<Point> c = transform::additive(Model::fourier(10, 15, 1000, 0.001), a);
    std::vector<Point> d = transform::additive(Model::fourier(10, 15, 1000, 0.001), b);

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(c), "a", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(d), "b", false));

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(c)), "c", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(d)), "d", false));
}

void ThemeWidget::renderModeHomeWork() {

    std::vector<Point> a = transform::additive(Model::fourier(10, 5, 1000, 0.001), transform::additive(Model::fourier(63, 55, 1000, 0.001), Model::fourier(15, 250, 1000, 0.001)));
    std::vector<Point> b = transform::antiShift( Model::fromFile("/home/matt/polytech/experimental data analysys/app/res.txt") );
    std::vector<Point> antiTrend1 = analysis::slideAvg(b, 4);
    std::vector<Point> antiTrend2 = analysis::slideAvg(b, 8);
    std::vector<Point> antiTrend3 = analysis::slideAvg(b, 12);
    std::vector<Point> c = b;


    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(b), "antiTrend", false));

    std::vector<Point> res = transform::ampSpecter(c);
    for (Point p : res) {
        printf("%f:%f\n", p.x, p.y);
    }
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(transform::reverseFourier(transform::ampSpecter(c, false, false))), "f 26 200", false));

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(a)), "f spectr", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(res), "anti trend spectr", false));
}

void ThemeWidget::renderReverseFourier() {
    std::vector<Point> a = Model::fourier(30, 5, 1000, 0.001);
    std::vector<Point> c = transform::reverseFourier(transform::ampSpecter(a, false, false));
    std::vector<Point> d = transform::ampSpecter(a, true, true);

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(a), "aaaaa", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(c), "cccc", false));

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(d), "dddd", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(d), "dddd", false));
}

void ThemeWidget::renderWindow() {
    std::vector<Point> a = Model::fourier(30, 5, 1000, 0.001);
    std::vector<Point> b = transform::window(a);
    std::vector<Point> c = transform::ampSpecter(a, true, true);
    std::vector<Point> d = transform::ampSpecter(b, true, true);

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(a), "aaaaa", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(c), "cccc", false));

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(b), "bbbb", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(d), "dddd", false));
}

void ThemeWidget::renderHomeWorkAutocorrelation() {
    std::vector<Point> a = Model::fromFile("/home/matt/polytech/experimental data analysys/app/res.txt");
    std::vector<Point> b = Model::getAutoCorrelartionFunc(a);
    std::vector<Point> c = Model::fourier(30, 5, 1000, 0.001);
    std::vector<Point> d = Model::getCorrelartionFunc(a, c);

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(a), "aaaaa", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(b), "bbbb", false));

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(c), "cccc", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(d), "dddd", false));
}

void ThemeWidget::renderInClassFilters1() {
    std::vector<Point> a = Model::getRegularSpike(120.0, 0.005, 1000);
//    std::vector<Point> a = Model::getPureSpikes(1000, 10, 10);
    std::vector<Point> b = Model::getHeartbeat(0.005, 10, 200);

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(a), "spec a", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(b), "spec b", false));

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(transform::convulation(a, b)), "res", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(transform::convulation(a, b))), "res spec", false));
}

void ThemeWidget::renderInClassFilters2() {
    std::vector<Point> a = Model::getRegularSpike(120.0, 0.005, 1000);
//    std::vector<Point> a = Model::getPureSpikes(1000, 10, 10);
    std::vector<Point> b = Model::getHeartbeat(0.005, 10, 200);
    std::vector<Point> c = Model::getHeartbeat(0.005, 10, 200);

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(a), "spec a", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(b), "spec b", false));

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(transform::convulation(transform::convulation(a, b), c)), "res", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(transform::convulation(a, b))), "res spec", false));
}

void ThemeWidget::renderInClassFilters3() {

    std::vector<Point> a = transform::lowPassFilter(32, 0.001, 60.0);
    std::vector<Point> b = Model::fromFile("/home/matt/polytech/experimental data analysys/app/res.txt");

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(a), "aaaaa", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(a)), "bbbb", false));

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(transform::convulation(b, a)), "cccc", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(transform::convulation(b, a))), "dddd", false));
}

void ThemeWidget::renderInClassFilters4() {
     std::vector<Point> a = transform::highPassFilter(64, 0.001, 50.0);
     std::vector<Point> b = transform::bandPassFilter(64, 0.001, 20.0, 100.0);
     std::vector<Point> c = transform::bandStopFilter(64, 0.001, 20.0, 100.0);

     std::vector<Point> d = Model::fromFile("/home/matt/polytech/experimental data analysys/app/res.txt");

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(transform::convulation(d, a))), "high", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(transform::convulation(d, b))), "band pass", false));

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(transform::convulation(d, c))), "band stop", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(d)), "original", false));
}

//void ThemeWidget::renderInClassFilters5() {
//    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(a), "aaaaa", false));
//    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(b), "bbbb", false));

//    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(c), "cccc", false));
//    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(d), "dddd", false));
//}

//void ThemeWidget::renderInClassFilters6() {
//    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(a), "aaaaa", false));
//    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(b), "bbbb", false));

//    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(c), "cccc", false));
//    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(d), "dddd", false));
//}


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
    case IN_CLASS:
        renderModeInClass();
        break;
    case IN_CLASS2:
        renderModeInClass2();
        break;
    case IN_CLASS3:
        renderModeInClass3();
        break;
    case IN_CLASS4:
        renderModeInClass4();
        break;
    case IN_CLASS5:
        renderModeInClass5();
        break;
    case IN_CLASS_fourier:
        renderModeInClassFourier();
        break;
    case IN_CLASS_fourier2:
        renderModeInClassFourier2();
        break;
    case IN_CLASS_fourier4:
        renderModeInClassFourier4();
        break;
    case IN_CLASS_ANTISHIFT_ANTISPYKES:
        renderModeInClassAntiShiftANtiSpykes();
        break;
    case IN_CLASS_SLIDE_AVG:
        renderModeInClassSlideAvg();
        break;
    case IN_CLASS_SPEC_ANALYS:
        renderModeInClassSpectrAnalysis();
        break;
    case IN_CLASS_SPEC_ANALYS2:
        renderModeInClassSpectrAnalysis2();
        break;
    case IN_CLASS_SPEC_ANALYS3:
        renderModeInClassSpectrAnalysis3();
        break;
    case IN_CLASS_SPEC_ANALYS4:
        renderModeInClassSpectrAnalysis4();
        break;
    case IN_CLASS_SPEC_ANALYS5:
        renderModeInClassSpectrAnalysis5();
        break;
    case HOME_WORK:
        renderModeHomeWork();
        break;
    case IN_CLASS_REVERSE_FOURIER:
        renderReverseFourier();
        break;
    case IN_CLASS_WINDOW:
        renderWindow();
        break;
    case HW_AUTOCORELL:
        renderHomeWorkAutocorrelation();
        break;

    case IN_CLASS_FILTER1:
        renderInClassFilters1();
        break;
    case IN_CLASS_FILTER2:
        renderInClassFilters2();
        break;
    case IN_CLASS_FILTER3:
        renderInClassFilters3();
        break;
    case IN_CLASS_FILTER4:
        renderInClassFilters4();
        break;
//    case IN_CLASS_FILTER5:
//        renderInClassFilters5();
//        break;
//    case IN_CLASS_FILTER6:
//        renderInClassFilters6();
//        break;

    default:
        printf("Unknown mode %d\n", mode);
    }

    const auto charts = m_charts;

    for (QChartView *chart : charts) {
        chart->setRenderHint(QPainter::Antialiasing, true);
    }

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
