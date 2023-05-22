#ifndef MACDPLUGIN_H
#define MACDPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include "indicatorinterface.h"
// FILE "echoplugin.json"
class MacdPlugin : public QObject,IndicatorInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "plugin.indicatorMacd")
    Q_INTERFACES(IndicatorInterface)
public:
    explicit MacdPlugin(QObject *parent = nullptr);
    QString getFileName() override;
    indicatorParam initIndicatorBuffer() override;
    abountIndiPlot initUiPlot(QCustomPlot *customplot,QHash<QString,QHash<int,QVector<double>>> &indicatorbuffer,QVector<double> &inputpram,int axisRectIndex,QCPMarginGroup *group) override;
    void calIndicatorData(KLine &kl,int period,QVector<double> &inputgram,QHash<QString,QHash<int,QVector<double>>> &indicatorbuffer,bool isallcal) override;
    void updatePlot(QHash<QString,QHash<int,QVector<double>>> &indicatorbuffer,QHash<QString,QVector<QCPAbstractPlottable *> > &plotOfIndicator, QVector<double> &inputgram,int period,bool isallupdated) override;

signals:

public slots:
};

#endif // MACDPLUGIN_H
