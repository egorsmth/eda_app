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
#include <audio.h>
#include <random>


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
#include <QImageReader>

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
    m_ui->themeComboBox->addItem("EXAM", 27);

    m_ui->themeComboBox->addItem("Reverse cardio (new semester)", 28);
    m_ui->themeComboBox->addItem("rw jpg", 29);
    m_ui->themeComboBox->addItem("jpg correction", 30);
    m_ui->themeComboBox->addItem("jpg histogram", 31);
    m_ui->themeComboBox->addItem("jpg strips filter", 32);
    m_ui->themeComboBox->addItem("jpg BIG strips filter", 33);
    m_ui->themeComboBox->addItem("jpg noises", 34);

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

// new semester
const int REVERSE_CARDIO = 28;
const int RW_JPG = 29;
const int JPG_CORR = 30;
const int JPG_HIST = 31;
const int JPG_STRIPS = 32;
const int JPG_STRIPS2 = 33;
const int JPG_NOISES = 34;

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
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(a)), "original", false));
}

void ThemeWidget::renderInClassFilters5() {
    AudioFile<double> audioFile;
    AudioFile<double> audioFileOut;
    audioFile.load ("/home/matt/polytech/experimental data analysys/app/papa.wav");
    audioFile.printSummary();

    int channel = 0;
    int numSamples = audioFile.getNumSamplesPerChannel();
    std::vector<Point> res;
    for (int i = 0; i < numSamples; i++)
    {
        Point p;
        p.x = i;
        p.y = audioFile.samples[channel][i];
        res.push_back(p);
    }
    double rate = 1.0 / (double)audioFile.getSampleRate();
    std::vector<Point> af = transform::ampSpecter(res);

    AudioFile<double>::AudioBuffer buffer;
    buffer.resize (1);
    std::cout << "RATE: " << rate << std::endl;

//    std::vector<Point> a = transform::bandStopFilter(64, rate, 0.03, 0.05);
//    std::vector<Point> a = transform::highPassFilter(64, rate, 1600);
//    std::vector<Point> a = transform::lowPassFilter(64, rate, 250);
    std::vector<Point> a = transform::bandPassFilter(64, rate, 400, 650);
    std::vector<Point> b = transform::convulation(res, a, rate);
    //std::vector<Point> c = transform::reverseFourier(b);


    // 3. Set number of samples per channel
    buffer[0].resize (numSamples);
    for (int j = 0; j < b.size(); j++) {
        buffer[0][j] = b[j].y * 100;
    }

    bool ok = audioFileOut.setAudioBuffer (buffer);
    if (!ok){
        throw "wtf";
    }
    audioFileOut.setSampleRate(audioFile.getSampleRate());
    audioFileOut.save ("/home/matt/polytech/experimental data analysys/app/out2.wav");

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(res), "original", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(af), "specter", false));

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(b), "modified", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(b)), "reverse spector", false));
}

void ThemeWidget::renderInClassFilters6() {
    std::vector<Point> a = Model::fromFile("/home/matt/polytech/experimental data analysys/app/data.dat");
    std::vector<Point> antiTrend = analysis::slideAvg(a, 6);
    std::vector<Point> b = transform::additive(antiTrend, a, true);
    std::vector<Point> c = transform::antiSpike(b, 25);

    std::vector<Point> bp = transform::bandPassFilter(128, 0.001, 46, 56);
    std::vector<Point> d = transform::convulation(b, bp, 1);


    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(a), "aaaaa", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(d), "bbbb", false));

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(a)), "cccc", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(d)), "dddd", false));
}

void ThemeWidget::renderReverseCardio() {
    std::vector<Point> a = Model::getRegularSpike(120.0, 0.005, 1000);
    std::vector<Point> b = Model::getHeartbeat(0.005, 10, 200);
    std::vector<Point> y = transform::convulation(a, b);

    std::vector<Point> x = transform::addZeros(b, 1000);

    std::vector<ComplexPoint> yy = transform::ampSpectorComplex(y, false);
    std::vector<ComplexPoint> xx = transform::ampSpectorComplex(x, false);

    std::vector<ComplexPoint> zz = transform::divideComplex(yy, xx);

    std::vector<Point> res = transform::complextToPoint(zz);

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(y), "heart Beat", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(res), "rev", false));

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(transform::reverseFourier(res)), "result", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(transform::ampSpecter(x)), "hb", false));
}

void ThemeWidget::renderRWJpg() {
    QImageReader reader("/home/matt/polytech/experimental data analysys/app/grace.jpg");
    std::cout << reader.size().rwidth() << " " << reader.size().rheight() << std::endl;
    QImage image = reader.read();
//    QImage* resultImgae = new QImage(image.width(), image.height(), image.format());
//    for (int x = 1; x < image.width()-1; x++) {
//        for (int y = 1; y < image.height()-1; y++) {
//            QColor l = image.pixelColor(x-1, y);
//            QColor r = image.pixelColor(x+1, y);
//            QColor u = image.pixelColor(x, y+1);
//            QColor d = image.pixelColor(x, y-1);

//            int red = l.red() + r.red() + u.red() + d.red();
//            int green = l.green() + r.green() + u.green() + d.green();
//            int blue = l.blue() + r.blue() + u.blue() + d.blue();
//            QColor* res = new QColor(red / 4, green/4, blue/4);

//            resultImgae->setPixelColor(x, y, *res);
//        }
//    }
//    resultImgae->save("/home/matt/polytech/experimental data analysys/app/grace2.jpg");



//    // scale up
//    double delta = 2.7;
//    QImage* resultImage = new QImage(image.width() *delta, image.height()*delta, image.format());
//    for (int x = 1; x < resultImage->width()-1; x++) {
//        for (int y = 1; y < resultImage->height()-1; y++) {
//                        QColor l = image.pixelColor((x-1)/delta, y/delta);
//                        QColor r = image.pixelColor((x+1)/delta, y/delta);
//                        QColor u = image.pixelColor(x/delta, (y+1)/delta);
//                        QColor d = image.pixelColor(x/delta, (y-1)/delta);

//                        int red = l.red() + r.red() + u.red() + d.red();
//                        int green = l.green() + r.green() + u.green() + d.green();
//                        int blue = l.blue() + r.blue() + u.blue() + d.blue();
//                        QColor* res = new QColor(red / 4, green/4, blue/4);

//                        resultImage->setPixelColor(x, y, *res);

//        }
//    }
//    resultImage->save("/home/matt/polytech/experimental data analysys/app/grace2.jpg");


        // scale down
        double delta = 0.7;
        QImage* resultImage = new QImage(image.width() *delta, image.height()*delta, image.format());
        for (int x = 1; x < resultImage->width()-1; x++) {
            for (int y = 1; y < resultImage->height()-1; y++) {
                            QColor l = image.pixelColor((x-1)/delta, y/delta);
                            QColor r = image.pixelColor((x+1)/delta, y/delta);
                            QColor u = image.pixelColor(x/delta, (y+1)/delta);
                            QColor d = image.pixelColor(x/delta, (y-1)/delta);

                            int red = l.red() + r.red() + u.red() + d.red();
                            int green = l.green() + r.green() + u.green() + d.green();
                            int blue = l.blue() + r.blue() + u.blue() + d.blue();
                            QColor* res = new QColor(red / 4, green/4, blue/4);

                            resultImage->setPixelColor(x, y, *res);

            }
        }
        resultImage->save("/home/matt/polytech/experimental data analysys/app/grace3.jpg");
}

void ThemeWidget::renderJpgCorr() {
    QImageReader reader1("/home/matt/polytech/experimental data analysys/app/image1.jpg");
    std::cout << reader1.size().rwidth() << " " << reader1.size().rheight() << std::endl;
    QImage image1 = reader1.read();


    QImage* resultImage = new QImage(image1.width(), image1.height(), image1.format());
        for (int x = 1; x < resultImage->width(); x++) {
            for (int y = 1; y < resultImage->height(); y++) {
                QColor c = image1.pixelColor(x, y);
                c.setHsv(c.hsvHue(), c.hsvSaturation(),  int(30*pow(c.value(),0.3)) % 255);
                resultImage->setPixelColor(x, y, c);
            }
        }



    // gamma correction
//    for (int x = 1; x < resultImage->width(); x++) {
//        for (int y = 1; y < resultImage->height(); y++) {
//            QColor c = image1.pixelColor(x, y);
//            c.setHsv(c.hsvHue(), c.hsvSaturation(), 0.9*pow(c.value(),0.9));
//            resultImage->setPixelColor(x, y, c);
//        }
//    }



    // negative
//    int maxL = 0;
//    for (int x = 1; x < resultImage->width(); x++) {
//        for (int y = 1; y < resultImage->height(); y++) {
//                        int s = image1.pixelColor(x, y).value();
//                        if (s > maxL) maxL = s;
//        }
//    }
//    for (int x = 1; x < resultImage->width(); x++) {
//        for (int y = 1; y < resultImage->height(); y++) {
//             QColor c = image1.pixelColor(x, y);
//             c.setHsv(c.hsvHue(), c.hsvSaturation(), abs(maxL - c.value()));
//             resultImage->setPixelColor(x, y, c);
//        }
//    }
    resultImage->save("/home/matt/polytech/experimental data analysys/app/image1-log.jpg");
}



void ThemeWidget::renderJpgHist() {
    QImageReader reader1("/home/matt/polytech/experimental data analysys/app/holywood.jpg");
    std::cout << reader1.size().rwidth() << " " << reader1.size().rheight() << std::endl;
    QImage image1 = reader1.read();

    std::vector<int> hist(256);

    QImage* resultImage = new QImage(image1.width(), image1.height(), image1.format());
    long total = resultImage->width() * resultImage->height();
        for (int x = 1; x < resultImage->width(); x++) {
            for (int y = 1; y < resultImage->height(); y++) {
                QColor c = image1.pixelColor(x, y);
                hist[c.value()] += 1;
            }
        }
    std::vector<double> cdf(256);
    for (int j = 0; j < 256; j++) {
        int t = hist[j];
        if (j != 0) t += cdf[j - 1];

        cdf[j] = double(t);
    }
    for (int j = 0; j < 256; j++) {


        cdf[j] = (cdf[j] / total) * 255.0;
    }

    std::vector<Point> a;
    for (int j = 0; j < 256; j++) {
        Point p;
        p.x = j;
        p.y = hist[j];
        a.push_back(p);
    }

    std::vector<Point> b;
    for (int j = 0; j < 256; j++) {
        Point p;
        p.x = j;
        p.y = cdf[j];
        b.push_back(p);
    }


    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(a), "heart Beat", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(b), "rev", false));

    for (int x = 1; x < resultImage->width(); x++) {
        for (int y = 1; y < resultImage->height(); y++) {
                        QColor c = image1.pixelColor(x, y);
                        c.setHsv(c.hsvHue(), c.hsvSaturation(), cdf[c.value()]);
                        resultImage->setPixelColor(x, y, c);
        }
    }
    resultImage->save("/home/matt/polytech/experimental data analysys/app/hol1.jpg");



    std::vector<Point> c;
    std::vector<int> cdf1(256);
    for (int j = 0; j < 256; j++) {
        cdf1[cdf[j]] = j;
        Point p;
        p.x = cdf[j];
        p.y = j;
        c.push_back(p);
    }
    for (int x = 1; x < resultImage->width(); x++) {
        for (int y = 1; y < resultImage->height(); y++) {
            QColor c = image1.pixelColor(x, y);
            c.setHsv(c.hsvHue(), c.hsvSaturation(), cdf1[c.value()]);
            resultImage->setPixelColor(x, y, c);
        }
    }
    resultImage->save("/home/matt/polytech/experimental data analysys/app/hol2.jpg");
    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(c), "rev", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(c), "rev", false));

}

void ThemeWidget::renderFilterStrips() {
    QImageReader reader1("/home/matt/polytech/experimental data analysys/app/xray.jpg");
    std::cout << reader1.size().rwidth() << " " << reader1.size().rheight() << std::endl;
    QImage image1 = reader1.read();

    std::vector<Point> row;
    for (int x = 0; x < image1.width(); x++) {
        Point p;
        p.x = x;
        p.y = image1.pixelColor(x, 150).value();
        row.push_back(p);
    }

    std::vector<Point> a = Model::getAutoCorrelartionFunc(row);
    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(row), "raw", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(a), "autocorrel", false));

    std::vector<Point> q = transform::ampSpecter(a);
    for (int i = 0; i < q.size(); i++) {
        q[i].x = q[i].x / 256;
    }
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(q), "spectr", false));

    std::vector<Point> c = transform::bandStopFilter(4, 1, 0.1, 0.6);
    std::vector<Point> d = transform::convulation(row, c);
    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(d), "filtered", false));
    QImage* resultImage = new QImage(image1.width(), image1.height(), image1.format());

    for (int y = 0; y < resultImage->height(); y++) {
        std::vector<Point> row;
        for (int x = 0; x < resultImage->width(); x++) {
            Point p;
            p.x = x;
            p.y = image1.pixelColor(x, y).value();
            row.push_back(p);
        }

        std::vector<Point> row2 = transform::convulation(row, c);

        for (int x = 0; x < resultImage->width(); x++) {
            QColor c = image1.pixelColor(x, y);
            c.setHsv(c.hsvHue(), c.hsvSaturation(), row2[x].y);
            resultImage->setPixelColor(x, y, c);
        }
    }
    resultImage->save("/home/matt/polytech/experimental data analysys/app/xray2.jpg");

}

void ThemeWidget::renderFilterStrips2() {
    QImageReader reader1("/home/matt/polytech/experimental data analysys/app/XrayBig.jpg");
    std::cout << reader1.size().rwidth() << " " << reader1.size().rheight() << std::endl;
    QImage image1 = reader1.read();

    QImage* image2 = new QImage(image1.width(), image1.height(), image1.format());
    std::vector< std::vector<Point> > res1 = transform::removeStrips(&image1, 8, 0.000001, 250, image2);
    image2->save("/home/matt/polytech/experimental data analysys/app/XrayBig1Strips.jpg");

    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(res1[0]), "spec res1", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(res1[1]), "conv res1", false));
    QImage* image3 = new QImage(image1.width(), image1.height(), image1.format());
    transform::betterBrightness(image2, image3);
    image3->save("/home/matt/polytech/experimental data analysys/app/XrayBig2Bright.jpg");

    QImage* image4 = new QImage(image1.width(), image1.height(), image1.format());
    std::vector< std::vector<Point> > res2 = transform::removeStrips(image3, 4, 0.000001, 10, image4);
    image4->save("/home/matt/polytech/experimental data analysys/app/XrayBig3Strips.jpg");

    m_charts[2]->setChart(createChart(transform::transformTimeseriesForView(res2[0]), "spec res2", false));
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(res2[1]), "conv res2", false));
}

void ThemeWidget::charttts(QImage *image, QString filename) {

    auto ch = createChart(transform::transformTimeseriesForView(analysis::getRowValueDistribution(image, 150)), "Distribution", false);
    QChartView* chartView = new QChartView;
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(900, 600);
    chartView->setChart(ch);
    chartView->grab().toImage().save(filename + "_CHART_DISTR.jpg");

    std::vector<Point> a;
        for (int x = 0; x < image->width(); x++)
        {
            Point p;
            p.x = x;
            p.y = image->pixelColor(x, 150).value();
            a.push_back(p);
        }

    //auto ch2 = createChart(transform::transformTimeseriesForView(transform::ampSpecter(a, true, true, 1.0f/(double)image->width())), "Spectrum", false);
    //QChartView* chartView2 = new QChartView;
    //chartView2->setRenderHint(QPainter::Antialiasing);
    //chartView2->resize(900, 600);
    //chartView2->setChart(ch2);
    //chartView2->grab().toImage().save(filename + "_CHART_SPECTR.jpg");
}

void ThemeWidget::renderImageNoises() {
    QImageReader reader1("/home/matt/polytech/experimental data analysys/app/MODEL.jpg");
    QImage image1 = reader1.read();
    charttts(&image1, "/home/matt/polytech/experimental data analysys/app/MODEL");
    m_charts[3]->setChart(createChart(transform::transformTimeseriesForView(analysis::getRowValueDistribution(&image1, 150)), "original", false));
    std::vector<double> zs;

    for (int y = 0; y < image1.height(); y++) {
        for (int x = 0; x < image1.width(); x++) {
            zs.push_back((double)image1.pixelColor(x, y).value());
        }
    }

    double avg = analysis::getAverage(zs);
    double var = sqrt(analysis::getVariance(zs, avg));

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0, var);

    double level, min, max;
    QImage* resultImage = new QImage(image1.width(), image1.height(), image1.format());
    QImage r = transform::ddSpec(&image1);
    r.save("/home/matt/polytech/experimental data analysys/app/fft_test.jpg");

    level = 0.2;
    for (int y = 0; y < resultImage->height(); y++) {
        for (int x = 0; x < resultImage->width(); x++) {
            QColor c = image1.pixelColor(x, y);
            int v = c.value() + distribution(generator) * level;
            if (v < 0) v = 0;
            else if (v > 255) v = 255;
            // std::cout << v << std::endl;
            c.setHsv(c.hsvHue(), c.hsvSaturation(), v);
            resultImage->setPixelColor(x, y, c);
        }
    }
    QImage rr = transform::ddSpec(resultImage);
    rr.save("/home/matt/polytech/experimental data analysys/app/fft_test_noise.jpg");
#if 0

    resultImage->save("/home/matt/polytech/experimental data analysys/app/1/normal_1%.jpg");
    charttts(resultImage, "/home/matt/polytech/experimental data analysys/app/1/normal_1%");
    QImage resultImage1 = transform::arithmeticMeanFilter(resultImage, 11);
    m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(analysis::getRowValueDistribution(resultImage, 150)), "with noise", false));
    m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(analysis::getRowValueDistribution(&resultImage1, 150)), "filtered", false));
    resultImage1.save("/home/matt/polytech/experimental data analysys/app/1/normal_1%_arithmetic.jpg");
    charttts(&resultImage1, "/home/matt/polytech/experimental data analysys/app/1/normal_1%_arithmetic");

    QImage resultImage1_a = transform::geometricMeanFilter(resultImage, 11);
    resultImage1_a.save("/home/matt/polytech/experimental data analysys/app/1/normal_1%_geometric.jpg");
    charttts(&resultImage1_a, "/home/matt/polytech/experimental data analysys/app/1/normal_1%_geometric");

    QImage resultImage1_b = transform::harmonicMeanFilter(resultImage, 11);
    resultImage1_b.save("/home/matt/polytech/experimental data analysys/app/1/normal_1%_harmonic.jpg");
    charttts(&resultImage1_b, "/home/matt/polytech/experimental data analysys/app/1/normal_1%_harmonic");

    QImage resultImage1_c = transform::midpointFilter(resultImage, 5);
    resultImage1_c.save("/home/matt/polytech/experimental data analysys/app/1/normal_1%_midpoint.jpg");
    charttts(&resultImage1_c, "/home/matt/polytech/experimental data analysys/app/1/normal_1%_midpoint");

    QImage resultImage1_d = transform::adaptiveMedianFilter(resultImage, 27);
    resultImage1_d.save("/home/matt/polytech/experimental data analysys/app/1/normal_1%_adaptive_median.jpg");
    charttts(&resultImage1_d, "/home/matt/polytech/experimental data analysys/app/1/normal_1%_adaptive_median");

    QImage resultImage1_e = transform::adaptiveArithmFilter(resultImage, 27);
    resultImage1_e.save("/home/matt/polytech/experimental data analysys/app/1/normal_1%_adaptive_arithm.jpg");
    charttts(&resultImage1_e, "/home/matt/polytech/experimental data analysys/app/1/normal_1%_adaptive_arithm");

    QImage resultImage1_f = transform::medianFilter(resultImage, 11);
    resultImage1_f.save("/home/matt/polytech/experimental data analysys/app/1/normal_1%_median.jpg");
    charttts(&resultImage1_f, "/home/matt/polytech/experimental data analysys/app/1/normal_1%_median");

    QImage resultImage1_g = transform::lowPassImageFilter(resultImage, 32, 0.1);
    resultImage1_g.save("/home/matt/polytech/experimental data analysys/app/1/normal_1%_lp.jpg");
    charttts(&resultImage1_g, "/home/matt/polytech/experimental data analysys/app/1/normal_1%_lp");


    level = 0.5;
    for (int y = 0; y < resultImage->height(); y++) {
        for (int x = 0; x < resultImage->width(); x++) {
            QColor c = image1.pixelColor(x, y);
            int v = c.value() + distribution(generator) * level;
            if (v < 0) v = 0;
            else if (v > 255) v = 255;
            //std::cout << v << std::endl;
            c.setHsv(c.hsvHue(), c.hsvSaturation(), v);
            resultImage->setPixelColor(x, y, c);
        }
    }

    resultImage->save("/home/matt/polytech/experimental data analysys/app/2/normal_5%.jpg");
    charttts(resultImage, "/home/matt/polytech/experimental data analysys/app/2/normal_5%");

    QImage resultImage2 = transform::arithmeticMeanFilter(resultImage, 9);
    resultImage2.save("/home/matt/polytech/experimental data analysys/app/2/normal_5%_arithmetic.jpg");
    charttts(&resultImage2, "/home/matt/polytech/experimental data analysys/app/2/normal_5%_arithmetic");

    QImage resultImage2_a = transform::geometricMeanFilter(resultImage, 5);
    resultImage2_a.save("/home/matt/polytech/experimental data analysys/app/2/normal_5%_geometric.jpg");
    charttts(&resultImage2_a, "/home/matt/polytech/experimental data analysys/app/2/normal_5%_geometric");

    QImage resultImage2_b = transform::harmonicMeanFilter(resultImage, 5);
    resultImage2_b.save("/home/matt/polytech/experimental data analysys/app/2/normal_5%_harmonic.jpg");
    charttts(&resultImage2_b, "/home/matt/polytech/experimental data analysys/app/2/normal_5%_harmonic");

    QImage resultImage2_c = transform::midpointFilter(resultImage, 5);
    resultImage2_c.save("/home/matt/polytech/experimental data analysys/app/2/normal_5%_midpoint.jpg");
    charttts(&resultImage2_c, "/home/matt/polytech/experimental data analysys/app/2/normal_5%_midpoint");

    QImage resultImage2_d = transform::adaptiveMedianFilter(resultImage, 25);
    resultImage2_d.save("/home/matt/polytech/experimental data analysys/app/2/normal_5%_adaptive_median.jpg");
    charttts(&resultImage1_d, "/home/matt/polytech/experimental data analysys/app/2/normal_5%_adaptive_median");

    QImage resultImage2_e = transform::adaptiveArithmFilter(resultImage, 25);
    resultImage2_e.save("/home/matt/polytech/experimental data analysys/app/2/normal_5%_adaptive_arithm.jpg");
    charttts(&resultImage2_e, "/home/matt/polytech/experimental data analysys/app/2/normal_5%_adaptive_arithm");

    QImage resultImage2_f = transform::medianFilter(resultImage, 3);
    resultImage1_f.save("/home/matt/polytech/experimental data analysys/app/2/normal_5%_median.jpg");
    charttts(&resultImage1_f, "/home/matt/polytech/experimental data analysys/app/2/normal_5%_median");

    QImage resultImage2_g = transform::lowPassImageFilter(resultImage, 32, 0.06);
    resultImage2_g.save("/home/matt/polytech/experimental data analysys/app/2/normal_5%_lp.jpg");
    charttts(&resultImage2_g, "/home/matt/polytech/experimental data analysys/app/2/normal_5%_lp");


    level = 1.2;
    for (int y = 0; y < resultImage->height(); y++) {
        for (int x = 0; x < resultImage->width(); x++) {
            QColor c = image1.pixelColor(x, y);
            int v = c.value() + distribution(generator) * level;
            if (v < 0) v = 0;
            else if (v > 255) v = 255;
            // std::cout << v << std::endl;
            c.setHsv(c.hsvHue(), c.hsvSaturation(), v);
            resultImage->setPixelColor(x, y, c);
        }
    }

    resultImage->save("/home/matt/polytech/experimental data analysys/app/3/normal_15%.jpg");
    charttts(resultImage, "/home/matt/polytech/experimental data analysys/app/3/normal_15%");

    QImage resultImage3 = transform::arithmeticMeanFilter(resultImage, 21);
    resultImage3.save("/home/matt/polytech/experimental data analysys/app/3/normal_15%_arithmetic.jpg");
    charttts(&resultImage3, "/home/matt/polytech/experimental data analysys/app/3/normal_15%_arithmetic");

    QImage resultImage3_a = transform::geometricMeanFilter(resultImage, 3);
    resultImage3_a.save("/home/matt/polytech/experimental data analysys/app/3/normal_15%_geometric.jpg");
    charttts(&resultImage3_a, "/home/matt/polytech/experimental data analysys/app/3/normal_15%_geometric");

    QImage resultImage3_b = transform::harmonicMeanFilter(resultImage, 3);
    resultImage3_b.save("/home/matt/polytech/experimental data analysys/app/3/normal_15%_harmonic.jpg");
    charttts(&resultImage3_b, "/home/matt/polytech/experimental data analysys/app/3/normal_15%_harmonic");

    QImage resultImage3_c = transform::midpointFilter(resultImage, 5);
    resultImage3_c.save("/home/matt/polytech/experimental data analysys/app/3/normal_15%_midpoint.jpg");
    charttts(&resultImage3_c, "/home/matt/polytech/experimental data analysys/app/3/normal_15%_midpoint");

    QImage resultImage3_d = transform::adaptiveMedianFilter(resultImage, 31);
    resultImage3_d.save("/home/matt/polytech/experimental data analysys/app/3/normal_15%_adaptive_median.jpg");
    charttts(&resultImage3_d, "/home/matt/polytech/experimental data analysys/app/3/normal_15%_adaptive_median");

    QImage resultImage3_e = transform::adaptiveArithmFilter(resultImage, 31);
    resultImage3_e.save("/home/matt/polytech/experimental data analysys/app/3/normal_15%_adaptive_arithm.jpg");
    charttts(&resultImage3_e, "/home/matt/polytech/experimental data analysys/app/3/normal_15%_adaptive_arithm");

    QImage resultImage3_f = transform::medianFilter(resultImage, 21);
    resultImage3_f.save("/home/matt/polytech/experimental data analysys/app/3/normal_15%_median.jpg");
    charttts(&resultImage3_f, "/home/matt/polytech/experimental data analysys/app/3/normal_15%_median");

    QImage resultImage3_g = transform::lowPassImageFilter(resultImage, 32, 0.04);
    resultImage3_g.save("/home/matt/polytech/experimental data analysys/app/3/normal_15%_lp.jpg");
    charttts(&resultImage3_g, "/home/matt/polytech/experimental data analysys/app/3/normal_15%_lp");

    max = 0, min = 255;
    for (int x = 0; x < zs.size(); x++) {
        if (zs[x] > max) max = zs[x];
        if (zs[x] < min) min = zs[x];
    }

    for (int y = 0; y < resultImage->height(); y++) {
        for (int x = 0; x < resultImage->width(); x++) {
            QColor c = image1.pixelColor(x, y);
            int v = rand() < RAND_MAX/20 ? (rand() > RAND_MAX/2 ? max : min) : c.value();
            c.setHsv(c.hsvHue(), c.hsvSaturation(), v);
            resultImage->setPixelColor(x, y, c);
        }
    }

    resultImage->save("/home/matt/polytech/experimental data analysys/app/4/impulse.jpg");
    charttts(resultImage, "/home/matt/polytech/experimental data analysys/app/4/impulse");

    QImage resultImage4 = transform::arithmeticMeanFilter(resultImage, 11);
    resultImage4.save("/home/matt/polytech/experimental data analysys/app/4/impulse_arithmetic.jpg");
    charttts(&resultImage4, "/home/matt/polytech/experimental data analysys/app/4/impulse_arithmetic");

    QImage resultImage4_a = transform::geometricMeanFilter(resultImage, 9);
    resultImage4_a.save("/home/matt/polytech/experimental data analysys/app/4/impulse_geometric.jpg");
    charttts(&resultImage4_a, "/home/matt/polytech/experimental data analysys/app/4/impulse_geometric");

    QImage resultImage4_b = transform::harmonicMeanFilter(resultImage, 9);
    resultImage4_b.save("/home/matt/polytech/experimental data analysys/app/4/impulse_harmonic.jpg");
    charttts(&resultImage4_b, "/home/matt/polytech/experimental data analysys/app/4/impulse_harmonic");

    QImage resultImage4_c = transform::midpointFilter(resultImage, 5);
    resultImage4_c.save("/home/matt/polytech/experimental data analysys/app/4/impulse_midpoint.jpg");
    charttts(&resultImage4_c, "/home/matt/polytech/experimental data analysys/app/4/impulse_midpoint");

    QImage resultImage4_d = transform::adaptiveMedianFilter(resultImage, 21);
    resultImage4_d.save("/home/matt/polytech/experimental data analysys/app/4/impulse_adaptive_median.jpg");
    charttts(&resultImage4_d, "/home/matt/polytech/experimental data analysys/app/4/impulse_adaptive_median");

    QImage resultImage4_e = transform::adaptiveArithmFilter(resultImage, 15);
    resultImage4_e.save("/home/matt/polytech/experimental data analysys/app/4/impulse_adaptive_arithm.jpg");
    charttts(&resultImage4_e, "/home/matt/polytech/experimental data analysys/app/4/impulse_adaptive_arithm");

    QImage resultImage4_f = transform::medianFilter(resultImage, 7);
    resultImage4_f.save("/home/matt/polytech/experimental data analysys/app/4/impulse_median.jpg");
    charttts(&resultImage4_f, "/home/matt/polytech/experimental data analysys/app/4/impulse_median");

    QImage resultImage4_g = transform::lowPassImageFilter(resultImage, 32, 0.08);
    resultImage4_g.save("/home/matt/polytech/experimental data analysys/app/4/impulse_lp.jpg");
    charttts(&resultImage4_g, "/home/matt/polytech/experimental data analysys/app/4/impulse_lp");

    level = 1.2;
    for (int y = 0; y < resultImage->height(); y++) {
        for (int x = 0; x < resultImage->width(); x++) {
            QColor c = image1.pixelColor(x, y);
            int v = c.value() + distribution(generator) * level;
            if (v < 0) v = 0;
            else if (v > 255) v = 255;
            // std::cout << v << std::endl;
            c.setHsv(c.hsvHue(), c.hsvSaturation(), v);
            resultImage->setPixelColor(x, y, c);
        }
    }
    max = 0, min = 255;
    for (int y = 0; y < resultImage->height(); y++) {
        for (int x = 0; x < resultImage->width(); x++) {
            QColor c = resultImage->pixelColor(x, y);
            int v = c.value();
            if (v > max) max = v;
            if (v < min) min = v;
        }
    }

    for (int y = 0; y < resultImage->height(); y++) {
        for (int x = 0; x < resultImage->width(); x++) {
            QColor c = resultImage->pixelColor(x, y);
            int v = rand() < RAND_MAX/20 ? (rand() > RAND_MAX/2 ? max : min) : c.value();
            c.setHsv(c.hsvHue(), c.hsvSaturation(), v);
            resultImage->setPixelColor(x, y, c);
        }
    }

    resultImage->save("/home/matt/polytech/experimental data analysys/app/5/normal_15%+impulse.jpg");
    charttts(resultImage, "/home/matt/polytech/experimental data analysys/app/normal_15%+impulse");

    QImage resultImage7 = transform::arithmeticMeanFilter(resultImage, 15);
    resultImage7.save("/home/matt/polytech/experimental data analysys/app/5/normal_15%+impulse_arithmetic.jpg");
    charttts(&resultImage7, "/home/matt/polytech/experimental data analysys/app/5/normal_15%+impulse_arithmetic");

    QImage resultImage7_a = transform::geometricMeanFilter(resultImage, 3);
    resultImage7_a.save("/home/matt/polytech/experimental data analysys/app/5/normal_15%+impulse_geometric.jpg");
    charttts(&resultImage7_a, "/home/matt/polytech/experimental data analysys/app/5/normal_15%+impulse_geometric");

    QImage resultImage7_b = transform::harmonicMeanFilter(resultImage, 3);
    resultImage7_b.save("/home/matt/polytech/experimental data analysys/app/5/normal_15%+impulse_harmonic.jpg");
    charttts(&resultImage7_b, "/home/matt/polytech/experimental data analysys/app/5/normal_15%+impulse_harmonic");

    QImage resultImage7_c = transform::midpointFilter(resultImage, 3);
    resultImage7_c.save("/home/matt/polytech/experimental data analysys/app/5/normal_15%+impulse_midpoint.jpg");
    charttts(&resultImage7_c, "/home/matt/polytech/experimental data analysys/app/5/normal_15%+impulse_midpoint");

    QImage resultImage7_d = transform::adaptiveMedianFilter(resultImage, 31);
    resultImage7_d.save("/home/matt/polytech/experimental data analysys/app/5/normal_15%+impuls_adaptive_median.jpg");
    charttts(&resultImage7_d, "/home/matt/polytech/experimental data analysys/app/5/normal_15%+impuls_adaptive_median");

    QImage resultImage7_e = transform::adaptiveArithmFilter(resultImage, 31);
    resultImage7_e.save("/home/matt/polytech/experimental data analysys/app/5/normal_15%+impuls_adaptive_arithm.jpg");
    charttts(&resultImage7_e, "/home/matt/polytech/experimental data analysys/app/5/normal_15%+impuls_adaptive_arithm");

    QImage resultImage7_f = transform::medianFilter(resultImage, 15);
    resultImage7_f.save("/home/matt/polytech/experimental data analysys/app/5/normal_15%+impuls_median.jpg");
    charttts(&resultImage7_f, "/home/matt/polytech/experimental data analysys/app/5/normal_15%+impuls_median");

    QImage resultImage7_g = transform::lowPassImageFilter(resultImage, 32, 0.15);
    resultImage7_g.save("/home/matt/polytech/experimental data analysys/app/5/normal_15%+impulse_lp.jpg");
    charttts(&resultImage7_g, "/home/matt/polytech/experimental data analysys/app/5/normal_15%+impulse_lp");
#endif
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
    case IN_CLASS_FILTER5:
        renderInClassFilters5();
        break;
    case IN_CLASS_FILTER6:
        renderInClassFilters6();
        break;
    case REVERSE_CARDIO:
        renderReverseCardio();
        break;
    case RW_JPG:
        renderRWJpg();
        break;
    case JPG_CORR:
        renderJpgCorr();
        break;
    case JPG_HIST:
        renderJpgHist();
        break;
    case JPG_STRIPS:
        renderFilterStrips();
        break;
    case JPG_STRIPS2:
        renderFilterStrips2();
        break;
    case JPG_NOISES:
        renderImageNoises();
        break;
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
