#ifndef MACROSSSTRATEGY_H
#define MACROSSSTRATEGY_H

#include <QObject>
#include <QtPlugin>
#include "strategyinterface.h"

class MaCrossStrategy : public QObject,StrategyInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "plugin.strategymacross")
    Q_INTERFACES(StrategyInterface)
public:
    explicit MaCrossStrategy(QObject *parent = nullptr);
    QString getFileName() override;
    strategyRet funStrategyCal(QHash<QString,QHash<QString,QHash<int,QVector<double>>>> &indicatordat,KLine *p,QVariant &medbuf,QVector<int> &barscountAtPrevTickList,
                                           TThostFtdcMoneyType	Balance,TThostFtdcMoneyType	CurrMargin,TThostFtdcMoneyType	Available,TThostFtdcMoneyType AllFrozenMargin,
                                           QList<pendingorder> &pendList,QList<postotalfieldfuture> &longposList,QList<postotalfieldfuture> &shortposList,
                                           QList<traderecord> &lastBuyOpenRecList,QList<traderecord> &lastSellOpenRecList,
                                           QString straid,TThostFtdcVolumeMultipleType	VolumeMultiple,TThostFtdcPriceType	PriceTick,TThostFtdcRatioType	OpenRatioByMoney,
                                           TThostFtdcMoneyType	OpenRatioByVolume,TThostFtdcRatioType	CloseRatioByMoney,TThostFtdcMoneyType	CloseRatioByVolume,
                                           TThostFtdcRatioType	CloseTodayRatioByMoney,TThostFtdcRatioType	CloseTodayRatioByVolume,
                                           CThostFtdcDepthMarketDataField &tickdata) override;

signals:

};

#endif // MACROSSSTRATEGY_H
