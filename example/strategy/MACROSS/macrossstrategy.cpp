#include "macrossstrategy.h"
#include <QDateTime>

MaCrossStrategy::MaCrossStrategy(QObject *parent) : QObject(parent)
{

}

QString MaCrossStrategy::getFileName()
{
    return QString("macrossstrategy");

}



strategyRet MaCrossStrategy::funStrategyCal(QHash<QString,QHash<QString,QHash<int,QVector<double>>>> &indicatordat,KLine *p,QVariant &medbuf,QVector<int> &barscountAtPrevTickList,
                                       TThostFtdcMoneyType	Balance,TThostFtdcMoneyType	CurrMargin,TThostFtdcMoneyType	Available,TThostFtdcMoneyType AllFrozenMargin,
                                       QList<pendingorder> &pendList,QList<postotalfieldfuture> &longposList,QList<postotalfieldfuture> &shortposList,
                                       QList<traderecord> &lastBuyOpenRecList,QList<traderecord> &lastSellOpenRecList,
                                       QString straid,TThostFtdcVolumeMultipleType	VolumeMultiple,TThostFtdcPriceType	PriceTick,TThostFtdcRatioType	OpenRatioByMoney,
                                       TThostFtdcMoneyType	OpenRatioByVolume,TThostFtdcRatioType	CloseRatioByMoney,TThostFtdcMoneyType	CloseRatioByVolume,
                                       TThostFtdcRatioType	CloseTodayRatioByMoney,TThostFtdcRatioType	CloseTodayRatioByVolume,
                                       CThostFtdcDepthMarketDataField &tickdata)
{
    //Q_UNUSED()
    strategyRet ret;
    ret.isoredered = false;
    ret.iscanceled = false;
    QTime starttime1(9, 0, 0);
    QTime endtime1(15, 0, 0);
    QTime currenttime = QTime::currentTime();
    if(currenttime<starttime1 || currenttime>endtime1)//只在白天盘口交易
        return ret;
    int ksize = (p[1].close).size();//当前3分钟K线的数目 p[0]->1min p[2]->5min,....
    if(ksize<=20)//K线数目不足
        return ret;
    //可对策略的报撤单次数进行计数并把报撤单次数存储在medbuf中，如果超过预定次数，则返回ret

    //初始报单处理
    //多头
    if(longposList.isEmpty())//没有多头持仓
    {
        //计算当前K线距离上次金叉的距离
        int distancrossover=0;
        if(ksize!=barscountAtPrevTickList.at(1))//K线数目有变化时进行计算
        {
            for(int i=ksize-1;i>0;--i)
            {
                if(indicatordat["MA"]["MA5"][1].at(i)>indicatordat["MA"]["MA21"][1].at(i) && indicatordat["MA"]["MA5"][1].at(i-1)<indicatordat["MA"]["MA21"][1].at(i-1))
                {
                    //crossoverindex = i;
                    break;
                }
                ++distancrossover;
            }
        }
        if(distancrossover<12 )//距离最近金叉小于12根 每次快照行情来临时都进行比较计算
        {
            double highprice = *(std::max_element(p[1].high.rbegin()+distancrossover+1,p[1].high.rbegin()+distancrossover+1+12))*1.01;//取最近12根K最高价的1.01倍作为多头入场点
            //多头报单
            if(tickdata.LastPrice>highprice)//最新价格突破入场点价格
            {
                /*
                //检查是否有空头持仓，如果有，统统平掉
                if(!shortdetailList.isEmpty())
                {
                    int volshort =0;//空头持仓总量
                    for(int i=0;i<shortdetailList.size();++i)
                        volshort+=shortdetailList.at(i).Volume;
                    userInsertOrder med;
                    med.Direction = THOST_FTDC_D_Buy;
                    med.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
                    med.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
                    med.VolumeTotalOriginal = volshort;
                    med.ContingentCondition = THOST_FTDC_CC_Immediately;
                    ret.isoredered = true;
                    ret.orderlist.push_back(med);
                }
                */
                ret.isoredered=true;
                userInsertOrder med;
                memset(&med,0,sizeof(userInsertOrder));
                med.Direction = THOST_FTDC_D_Buy;
                med.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
                med.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
                med.VolumeTotalOriginal = 3;//初始下单量
                med.ContingentCondition = THOST_FTDC_CC_Immediately;//支持交易所所有的报单类型
                med.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//只要交易所支持也可以是市价单
                med.TimeCondition = THOST_FTDC_TC_GFD;//交易所支持的fakfok均可以
                med.VolumeCondition = THOST_FTDC_VC_AV;
                //med.MinVolume = 1;//FAK可以填充此值
                med.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
                med.LimitPrice = tickdata.AskPrice1+PriceTick;//对价加1跳报单

                ret.orderlist.push_back(med);
                static int cou=0;
                printf("buy open= %d\n",++cou);
            }
        }
        else
        {
            //检查该策略在本合约上未成交的多头报单，如果有，则撤单
            for(int i=0;i<pendList.size();++i)
            {
                //是买开报单且是本策略id
                if(pendList.at(i).Direction==THOST_FTDC_D_Buy && pendList.at(i).OffsetFlag==THOST_FTDC_OF_Open && QString(pendList.at(i).OrderRef).right(2)==straid)
                {
                    ret.iscanceled = true;
                    userCancelOrder tt;
                    tt.InstrumentID=QString(tickdata.InstrumentID);
                    tt.OrderSysID=QString(pendList.at(i).OrderSysID);
                    ret.cancelorderlist.push_back(tt);
                }
            }
        }
    }
    if(shortposList.isEmpty())
    {
        //计算当前K线距离上次死叉的距离
        int distancrossover=0;
        if(ksize!=barscountAtPrevTickList.at(1))//K线数目有变化时进行计算
        {
            for(int i=ksize-1;i>0;--i)
            {
                if(indicatordat["MA"]["MA5"][1].at(i)<indicatordat["MA"]["MA21"][1].at(i) && indicatordat["MA"]["MA5"][1].at(i-1)>indicatordat["MA"]["MA21"][1].at(i-1))
                {
                    //crossoverindex = i;
                    break;
                }
                ++distancrossover;
            }
        }
        if(distancrossover<12 )//距离最近si叉小于12根 每次快照行情来临时都进行比较计算
        {
            double lowprice = *(std::max_element(p[1].low.rbegin()+distancrossover+1,p[1].low.rbegin()+distancrossover+1+12))*0.99;//取最近12根K最di价的0.99倍作为kong头入场点
            //kong头报单
            if(tickdata.LastPrice<lowprice)//最新价格突破入场点价格
            {
                /*
                //检查是否有空头持仓，如果有，统统平掉
                if(!shortdetailList.isEmpty())
                {
                    int volshort =0;//空头持仓总量
                    for(int i=0;i<shortdetailList.size();++i)
                        volshort+=shortdetailList.at(i).Volume;
                    userInsertOrder med;
                    med.Direction = THOST_FTDC_D_Buy;
                    med.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
                    med.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
                    med.VolumeTotalOriginal = volshort;
                    med.ContingentCondition = THOST_FTDC_CC_Immediately;
                    ret.isoredered = true;
                    ret.orderlist.push_back(med);
                }
                */
                ret.isoredered=true;
                userInsertOrder med;
                memset(&med,0,sizeof(userInsertOrder));
                med.Direction = THOST_FTDC_D_Sell;
                med.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
                med.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
                med.VolumeTotalOriginal = 3;//初始下单量
                med.ContingentCondition = THOST_FTDC_CC_Immediately;//支持交易所所有的报单类型
                med.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//只要交易所支持也可以是市价单
                med.TimeCondition = THOST_FTDC_TC_GFD;//交易所支持的fakfok均可以
                med.VolumeCondition = THOST_FTDC_VC_AV;
                //med.MinVolume = 1;//FAK可以填充此值
                med.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
                med.LimitPrice = tickdata.BidPrice1-PriceTick;//对价jian1跳报单

                ret.orderlist.push_back(med);
                static int cou=0;
                printf("buy open= %d\n",++cou);
            }
        }
        else
        {
            //检查该策略在本合约上未成交的多头报单，如果有，则撤单
            for(int i=0;i<pendList.size();++i)
            {
                //是卖开报单且是本策略id
                if(pendList.at(i).Direction==THOST_FTDC_D_Sell && pendList.at(i).OffsetFlag==THOST_FTDC_OF_Open && QString(pendList.at(i).OrderRef).right(2)==straid)
                {
                    ret.iscanceled = true;
                    userCancelOrder tt;
                    tt.InstrumentID=QString(tickdata.InstrumentID);
                    tt.OrderSysID=QString(pendList.at(i).OrderSysID);
                    ret.cancelorderlist.push_back(tt);
                }
            }
        }
    }
    //追踪停止单  每个tick到来时计算
    //多头退出
    if(!longposList.isEmpty())//持有多头
    {
        double stopprice = *(std::min_element(p[1].low.rbegin()+1,p[1].low.rbegin()+1+8));
        if(tickdata.LastPrice<stopprice)
        {
            int volshort =0;//多头持仓总量
            for(int i=0;i<longposList.size();++i)
                volshort+=longposList.at(i).Position;
            userInsertOrder med;

            memset(&med,0,sizeof(userInsertOrder));
            med.Direction = THOST_FTDC_D_Sell;
            med.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
            med.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
            med.VolumeTotalOriginal = volshort;//
            med.ContingentCondition = THOST_FTDC_CC_Immediately;//支持交易所所有的报单类型
            med.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//只要交易所支持也可以是市价单
            med.TimeCondition = THOST_FTDC_TC_GFD;//交易所支持的fakfok均可以
            med.VolumeCondition = THOST_FTDC_VC_AV;
            //med.MinVolume = 1;//FAK可以填充此值
            med.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
            med.LimitPrice = tickdata.BidPrice1-PriceTick;//对价jian1跳报单
            ret.isoredered = true;
            ret.orderlist.push_back(med);
            static int ga=0;
            printf("close sell = %d\n",++ga);
        }
    }
    //空头退出
    if(!shortposList.isEmpty())//持有空头
    {
        double stopprice = *(std::min_element(p[1].high.rbegin()+1,p[1].high.rbegin()+1+8));
        if(tickdata.LastPrice>=stopprice)
        {
            int volshort =0;//kong头持仓总量
            for(int i=0;i<shortposList.size();++i)
                volshort+=shortposList.at(i).Position;
            userInsertOrder med;

            memset(&med,0,sizeof(userInsertOrder));
            med.Direction = THOST_FTDC_D_Buy;
            med.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
            med.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
            med.VolumeTotalOriginal = volshort;//
            med.ContingentCondition = THOST_FTDC_CC_Immediately;//支持交易所所有的报单类型
            med.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//只要交易所支持也可以是市价单
            med.TimeCondition = THOST_FTDC_TC_GFD;//交易所支持的fakfok均可以
            med.VolumeCondition = THOST_FTDC_VC_AV;
            //med.MinVolume = 1;//FAK可以填充此值
            med.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
            med.LimitPrice = tickdata.AskPrice1+PriceTick;//对价jian1跳报单
            ret.isoredered = true;
            ret.orderlist.push_back(med);
            static int ga=0;
            printf("close buy = %d\n",++ga);
        }
    }

    //加仓处理
    /*
    if(longdetailList.isEmpty() && shortdetailList.isEmpty())
    {
        if(!closedeteaiList.isEmpty())
        {
            //多单重入
            if(closedeteaiList.last().Direction == THOST_FTDC_D_Buy)
            {
                //计算最近10根bar的最高值
                double highmax = *(std::max_element((p->high).rbegin(),(p->high).rbegin()+10));
                if(LastPrice>=highmax)
                {
                    //计算上次多头平仓据今的根数
                    int barco = 0;
                    QDateTime lastcloseTime = QDateTime::fromString(QString(closedeteaiList.last().CloseDate)+QString(closedeteaiList.last().CloseTime),"yyyyMMddhh:mm:ss");
                    for(int i=(p->close).size()-1;i>=0;--i)
                    {
                        QDateTime bartime = QDateTime::fromString(p->curDate.at(i),"yyyyMMddhh:mm");
                        if(lastcloseTime<=bartime)
                        {
                            ++barco;
                            break;
                        }
                    }
                    if(barco<15)
                    {
                        ret.isoredered=true;
                        userInsertOrder med;
                        med.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
                        med.Direction = THOST_FTDC_D_Buy;
                        med.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
                        med.ContingentCondition = THOST_FTDC_CC_Immediately;
                        med.VolumeTotalOriginal = 1;//加单量
                        ret.orderlist.push_back(med);
                        static int ga=0;
                        printf("buy open again = %d\n",++ga);
                    }
                }
            }
            //空单重入
            else if(closedeteaiList.last().Direction == THOST_FTDC_D_Sell)
            {
                //计算最近10根bar的最高值
                double lowmax = *(std::min_element((p->low).rbegin(),(p->low).rbegin()+10));
                if(LastPrice<=lowmax)
                {
                    //计算上次多头平仓据今的根数
                    int barco = 0;
                    QDateTime lastcloseTime = QDateTime::fromString(QString(closedeteaiList.last().CloseDate)+QString(closedeteaiList.last().CloseTime),"yyyyMMddhh:mm:ss");
                    for(int i=(p->close).size()-1;i>=0;--i)
                    {
                        QDateTime bartime = QDateTime::fromString(p->curDate.at(i),"yyyyMMddhh:mm");
                        if(lastcloseTime<=bartime)
                        {
                            ++barco;
                            break;
                        }
                    }
                    if(barco<15)
                    {
                        ret.isoredered=true;
                        userInsertOrder med;
                        med.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
                        med.Direction = THOST_FTDC_D_Sell;
                        med.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
                        med.ContingentCondition = THOST_FTDC_CC_Immediately;
                        med.VolumeTotalOriginal = 1;//加单量
                        ret.orderlist.push_back(med);
                        static int ga=0;
                        printf("sell open again = %d\n",++ga);
                    }
                }
            }
        }
    }
    */
    return ret;
}
