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
    if(currenttime<starttime1 || currenttime>endtime1)//ֻ�ڰ����̿ڽ���
        return ret;
    int ksize = (p[1].close).size();//��ǰ3����K�ߵ���Ŀ p[0]->1min p[2]->5min,....
    if(ksize<=20)//K����Ŀ����
        return ret;
    //�ɶԲ��Եı������������м������ѱ����������洢��medbuf�У��������Ԥ���������򷵻�ret

    //��ʼ��������
    //��ͷ
    if(longposList.isEmpty())//û�ж�ͷ�ֲ�
    {
        //���㵱ǰK�߾����ϴν��ľ���
        int distancrossover=0;
        if(ksize!=barscountAtPrevTickList.at(1))//K����Ŀ�б仯ʱ���м���
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
        if(distancrossover<12 )//����������С��12�� ÿ�ο�����������ʱ�����бȽϼ���
        {
            double highprice = *(std::max_element(p[1].high.rbegin()+distancrossover+1,p[1].high.rbegin()+distancrossover+1+12))*1.01;//ȡ���12��K��߼۵�1.01����Ϊ��ͷ�볡��
            //��ͷ����
            if(tickdata.LastPrice>highprice)//���¼۸�ͻ���볡��۸�
            {
                /*
                //����Ƿ��п�ͷ�ֲ֣�����У�ͳͳƽ��
                if(!shortdetailList.isEmpty())
                {
                    int volshort =0;//��ͷ�ֲ�����
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
                med.VolumeTotalOriginal = 3;//��ʼ�µ���
                med.ContingentCondition = THOST_FTDC_CC_Immediately;//֧�ֽ��������еı�������
                med.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//ֻҪ������֧��Ҳ�������м۵�
                med.TimeCondition = THOST_FTDC_TC_GFD;//������֧�ֵ�fakfok������
                med.VolumeCondition = THOST_FTDC_VC_AV;
                //med.MinVolume = 1;//FAK��������ֵ
                med.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
                med.LimitPrice = tickdata.AskPrice1+PriceTick;//�Լۼ�1������

                ret.orderlist.push_back(med);
                static int cou=0;
                printf("buy open= %d\n",++cou);
            }
        }
        else
        {
            //���ò����ڱ���Լ��δ�ɽ��Ķ�ͷ����������У��򳷵�
            for(int i=0;i<pendList.size();++i)
            {
                //���򿪱������Ǳ�����id
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
        //���㵱ǰK�߾����ϴ�����ľ���
        int distancrossover=0;
        if(ksize!=barscountAtPrevTickList.at(1))//K����Ŀ�б仯ʱ���м���
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
        if(distancrossover<12 )//�������si��С��12�� ÿ�ο�����������ʱ�����бȽϼ���
        {
            double lowprice = *(std::max_element(p[1].low.rbegin()+distancrossover+1,p[1].low.rbegin()+distancrossover+1+12))*0.99;//ȡ���12��K��di�۵�0.99����Ϊkongͷ�볡��
            //kongͷ����
            if(tickdata.LastPrice<lowprice)//���¼۸�ͻ���볡��۸�
            {
                /*
                //����Ƿ��п�ͷ�ֲ֣�����У�ͳͳƽ��
                if(!shortdetailList.isEmpty())
                {
                    int volshort =0;//��ͷ�ֲ�����
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
                med.VolumeTotalOriginal = 3;//��ʼ�µ���
                med.ContingentCondition = THOST_FTDC_CC_Immediately;//֧�ֽ��������еı�������
                med.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//ֻҪ������֧��Ҳ�������м۵�
                med.TimeCondition = THOST_FTDC_TC_GFD;//������֧�ֵ�fakfok������
                med.VolumeCondition = THOST_FTDC_VC_AV;
                //med.MinVolume = 1;//FAK��������ֵ
                med.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
                med.LimitPrice = tickdata.BidPrice1-PriceTick;//�Լ�jian1������

                ret.orderlist.push_back(med);
                static int cou=0;
                printf("buy open= %d\n",++cou);
            }
        }
        else
        {
            //���ò����ڱ���Լ��δ�ɽ��Ķ�ͷ����������У��򳷵�
            for(int i=0;i<pendList.size();++i)
            {
                //�������������Ǳ�����id
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
    //׷��ֹͣ��  ÿ��tick����ʱ����
    //��ͷ�˳�
    if(!longposList.isEmpty())//���ж�ͷ
    {
        double stopprice = *(std::min_element(p[1].low.rbegin()+1,p[1].low.rbegin()+1+8));
        if(tickdata.LastPrice<stopprice)
        {
            int volshort =0;//��ͷ�ֲ�����
            for(int i=0;i<longposList.size();++i)
                volshort+=longposList.at(i).Position;
            userInsertOrder med;

            memset(&med,0,sizeof(userInsertOrder));
            med.Direction = THOST_FTDC_D_Sell;
            med.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
            med.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
            med.VolumeTotalOriginal = volshort;//
            med.ContingentCondition = THOST_FTDC_CC_Immediately;//֧�ֽ��������еı�������
            med.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//ֻҪ������֧��Ҳ�������м۵�
            med.TimeCondition = THOST_FTDC_TC_GFD;//������֧�ֵ�fakfok������
            med.VolumeCondition = THOST_FTDC_VC_AV;
            //med.MinVolume = 1;//FAK��������ֵ
            med.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
            med.LimitPrice = tickdata.BidPrice1-PriceTick;//�Լ�jian1������
            ret.isoredered = true;
            ret.orderlist.push_back(med);
            static int ga=0;
            printf("close sell = %d\n",++ga);
        }
    }
    //��ͷ�˳�
    if(!shortposList.isEmpty())//���п�ͷ
    {
        double stopprice = *(std::min_element(p[1].high.rbegin()+1,p[1].high.rbegin()+1+8));
        if(tickdata.LastPrice>=stopprice)
        {
            int volshort =0;//kongͷ�ֲ�����
            for(int i=0;i<shortposList.size();++i)
                volshort+=shortposList.at(i).Position;
            userInsertOrder med;

            memset(&med,0,sizeof(userInsertOrder));
            med.Direction = THOST_FTDC_D_Buy;
            med.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
            med.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
            med.VolumeTotalOriginal = volshort;//
            med.ContingentCondition = THOST_FTDC_CC_Immediately;//֧�ֽ��������еı�������
            med.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//ֻҪ������֧��Ҳ�������м۵�
            med.TimeCondition = THOST_FTDC_TC_GFD;//������֧�ֵ�fakfok������
            med.VolumeCondition = THOST_FTDC_VC_AV;
            //med.MinVolume = 1;//FAK��������ֵ
            med.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
            med.LimitPrice = tickdata.AskPrice1+PriceTick;//�Լ�jian1������
            ret.isoredered = true;
            ret.orderlist.push_back(med);
            static int ga=0;
            printf("close buy = %d\n",++ga);
        }
    }

    //�Ӳִ���
    /*
    if(longdetailList.isEmpty() && shortdetailList.isEmpty())
    {
        if(!closedeteaiList.isEmpty())
        {
            //�൥����
            if(closedeteaiList.last().Direction == THOST_FTDC_D_Buy)
            {
                //�������10��bar�����ֵ
                double highmax = *(std::max_element((p->high).rbegin(),(p->high).rbegin()+10));
                if(LastPrice>=highmax)
                {
                    //�����ϴζ�ͷƽ�־ݽ�ĸ���
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
                        med.VolumeTotalOriginal = 1;//�ӵ���
                        ret.orderlist.push_back(med);
                        static int ga=0;
                        printf("buy open again = %d\n",++ga);
                    }
                }
            }
            //�յ�����
            else if(closedeteaiList.last().Direction == THOST_FTDC_D_Sell)
            {
                //�������10��bar�����ֵ
                double lowmax = *(std::min_element((p->low).rbegin(),(p->low).rbegin()+10));
                if(LastPrice<=lowmax)
                {
                    //�����ϴζ�ͷƽ�־ݽ�ĸ���
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
                        med.VolumeTotalOriginal = 1;//�ӵ���
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
