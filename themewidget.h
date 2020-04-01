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

#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include "callout.h"

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <qlayoutitem.h>

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class Ui_ThemeWidgetForm;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_CHARTS_USE_NAMESPACE

class ThemeWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ThemeWidget(QWidget *parent = 0);
    ~ThemeWidget();

private Q_SLOTS:
    void updateUI();

private:
    DataTable generateRandomData(int listCount, int valueMax, int valueCount) const;
    void populateModeBox();
//    void populateThemeBox();
//    void populateAnimationBox();
//    void populateLegendBox();
    void connectSignals();
//    QChart *createAreaChart() const;
//    QChart *createBarChart(int valueCount) const;
//    QChart *createPieChart() const;
    QChart *createLineChart() const;
    QChart *createChart(const DataList &list, const char* name, bool normal, double s) const;
//    QChart *createSplineChart() const;
//    QChart *createScatterChart() const;

    void tooltip(QPointF point, bool state);

private:
    int m_listCount;
    int m_valueMax;
    Callout* m_tooltip;
    int m_valueCount;
    QList<QChartView *> m_charts;
    QList<QObject *> m_widgets;
    QList<QLayoutItem *> m_items;
    DataTable m_dataTable;

    Ui_ThemeWidgetForm *m_ui;

    static std::map< const int , std::function<void()> > getDispTable();

    void clearMode();

    void charttts(QImage *image, QString filename);

    void renderModeGraph();
    void renderModeGraphStar();
    void renderModeInClass();
    void renderModeInClass2();
    void renderModeInClass3();
    void renderModeInClass4();
    void renderModeInClass5();
    void renderModeInClassFourier();
    void renderModeInClassFourier2();
    void renderModeInClassFourier4();

    void renderModeInClassAntiShiftANtiSpykes();
    void renderModeInClassSlideAvg();

    void renderModeInClassSpectrAnalysis();
    void renderModeInClassSpectrAnalysis2();
    void renderModeInClassSpectrAnalysis3();
    void renderModeInClassSpectrAnalysis4();
    void renderModeInClassSpectrAnalysis5();
    void renderModeHomeWork();
    void renderReverseFourier();
    void renderWindow();
    void renderHomeWorkAutocorrelation();

    void renderInClassFilters1();
    void renderInClassFilters2();
    void renderInClassFilters3();
    void renderInClassFilters4();
    void renderInClassFilters5();
    void renderInClassFilters6();

    void renderReverseCardio();
    void renderRWJpg();
    void renderJpgCorr();
    void renderJpgHist();
    void renderFilterStrips();
    void renderFilterStrips2();

    void renderImageNoises();
};


#endif /* THEMEWIDGET_H */
