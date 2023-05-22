#include "shortalertexample.h"

ShortKdMalert::ShortKdMalert(QObject *parent) : QObject(parent)
{

}

QString ShortKdMalert::getFileName()
{
    return "shortkdmalert_bigperiod";
}

QList<alertRet> ShortKdMalert::funAlertCal(QHash<QString,QHash<QString,QHash<int,QVector<double>>>> &indicatordat,QVariant &medbuff)
{

    QList<alertRet> ret;
    if(!indicatordat.isEmpty())
    {
        if(!indicatordat.contains("macd"))
            return ret;
        int msize=indicatordat["macd"]["macdv"][5].size();
        if(msize>=26)
        {
            //去除大于0的限制
            if(indicatordat["macd"]["macdv"][5][msize-1]<indicatordat["macd"]["macdv"][5][msize-2])//最新一根为黄 //indicatordat["macd"]["macdv"][5][msize-1]>0 &&
            {
                if(indicatordat["macd"]["macdv"][5][msize-2]>=indicatordat["macd"]["macdv"][5][msize-3])//倒数第二根为绿 //indicatordat["macd"]["macdv"][5][msize-2]>0 &&
                {
                    //ret.flag =true;
                    //ret.alertPeriod.append(QString::fromLocal8Bit("15分钟周期"));
                    //ret.alertText.append(QString::fromLocal8Bit("MA预警-"));
                    alertRet med;
                    med.context.append(QString::fromLocal8Bit("空空"));
                    med.context.append(QString::fromLocal8Bit("15分钟周期"));
                    med.context.append(QString::fromLocal8Bit("Ma预警-"));
                    med.ksize = msize;
                    ret.append(med);
                }
            }
        }
        //30分钟处理

        //判断MACD指标是否由红变白
        msize=indicatordat["macd"]["macdv"][6].size();
        if(msize>=26)
        {
            //去除大于0的限制
            if(indicatordat["macd"]["macdv"][6][msize-1]<indicatordat["macd"]["macdv"][6][msize-2])//最新一根为白 //indicatordat["macd"]["macdv"][6][msize-1]>0 &&
            {
                if(indicatordat["macd"]["macdv"][6][msize-2]>=indicatordat["macd"]["macdv"][6][msize-3])//倒数第二根为红 //indicatordat["macd"]["macdv"][6][msize-2]>0 &&
                {
                    //ret.flag =true;
                    //ret.alertPeriod.append(QString::fromLocal8Bit("30分钟周期+"));
                    //ret.alertText.append(QString::fromLocal8Bit("MA预警-"));
                    alertRet med;
                    med.context.append(QString::fromLocal8Bit("空空"));
                    med.context.append(QString::fromLocal8Bit("30分钟周期+"));
                    med.context.append(QString::fromLocal8Bit("Ma预警-"));
                    med.ksize = msize;
                    ret.append(med);
                }
            }
        }

        //60分钟处理

        //判断MACD指标是否由绿变黄
        msize=indicatordat["macd"]["macdv"][7].size();
        if(msize>=26)
        {
            //去除大于0的限制
            if(indicatordat["macd"]["macdv"][7][msize-1]<indicatordat["macd"]["macdv"][7][msize-2])//最新一根为黄 //indicatordat["macd"]["macdv"][7][msize-1]>0 &&
            {
                if(indicatordat["macd"]["macdv"][7][msize-2]>=indicatordat["macd"]["macdv"][7][msize-3])//倒数第二根为绿 //indicatordat["macd"]["macdv"][7][msize-2]>0 &&
                {
                    //ret.flag =true;
                    //ret.alertPeriod.append(QString::fromLocal8Bit("1小时周期+"));
                    //ret.alertText.append(QString::fromLocal8Bit("MA预警-"));
                    alertRet med;
                    med.context.append(QString::fromLocal8Bit("空空"));
                    med.context.append(QString::fromLocal8Bit("1小时周期+"));
                    med.context.append(QString::fromLocal8Bit("Ma预警-"));
                    med.ksize = msize;
                    ret.append(med);
                }
            }
        }

        //120分钟处理

        //判断MACD指标是否由绿变黄
        msize=indicatordat["macd"]["macdv"][8].size();
        if(msize>=26)
        {
            //去除大于0的限制
            if(indicatordat["macd"]["macdv"][8][msize-1]<indicatordat["macd"]["macdv"][8][msize-2])//最新一根为黄 //indicatordat["macd"]["macdv"][8][msize-1]>0 &&
            {
                if(indicatordat["macd"]["macdv"][8][msize-2]>=indicatordat["macd"]["macdv"][8][msize-3])//倒数第二根为绿 //indicatordat["macd"]["macdv"][8][msize-2]>0 &&
                {
                    //ret.flag =true;
                    //ret.alertPeriod.append(QString::fromLocal8Bit("2小时周期+"));
                    //ret.alertText.append(QString::fromLocal8Bit("MA预警-"));
                    alertRet med;
                    med.context.append(QString::fromLocal8Bit("空空"));
                    med.context.append(QString::fromLocal8Bit("2小时周期+"));
                    med.context.append(QString::fromLocal8Bit("Ma预警-"));
                    med.ksize = msize;
                    ret.append(med);
                }
            }
        }

        //240分钟处理
        //判断MACD指标是否由绿变黄
        msize=indicatordat["macd"]["macdv"][9].size();
        if(msize>=26)
        {
            //去除大于0的限制
            if(indicatordat["macd"]["macdv"][9][msize-1]>0 && indicatordat["macd"]["macdv"][9][msize-1]<indicatordat["macd"]["macdv"][9][msize-2])//最新一根为黄
            {
                if(indicatordat["macd"]["macdv"][9][msize-2]>0 && indicatordat["macd"]["macdv"][9][msize-2]>=indicatordat["macd"]["macdv"][9][msize-3])//倒数第二根为绿
                {
                    //ret.flag =true;
                    //ret.alertPeriod.append(QString::fromLocal8Bit("4小时周期+"));
                    //ret.alertText.append(QString::fromLocal8Bit("MA预警-"));
                    alertRet med;
                    med.context.append(QString::fromLocal8Bit("空空"));
                    med.context.append(QString::fromLocal8Bit("4小时周期+"));
                    med.context.append(QString::fromLocal8Bit("Ma预警-"));
                    med.ksize = msize;
                    ret.append(med);
                }
            }
        }

        //日线处理
        //判断MACD指标是否由绿变黄
        msize=indicatordat["macd"]["macdv"][10].size();
        if(msize>=25)
        {
            //去除大于0的限制
            if(indicatordat["macd"]["macdv"][10][msize-1]>0 && indicatordat["macd"]["macdv"][10][msize-1]<indicatordat["macd"]["macdv"][10][msize-2])//最新一根为黄
            {
                if(indicatordat["macd"]["macdv"][10][msize-2]>0 && indicatordat["macd"]["macdv"][10][msize-2]>=indicatordat["macd"]["macdv"][10][msize-3])//倒数第二根为绿
                {
                    //ret.flag =true;
                    //ret.alertPeriod.append(QString::fromLocal8Bit("日线周期+"));
                    //ret.alertText.append(QString::fromLocal8Bit("MA预警-"));
                    alertRet med;
                    med.context.append(QString::fromLocal8Bit("空空"));
                    med.context.append(QString::fromLocal8Bit("日线周期+"));
                    med.context.append(QString::fromLocal8Bit("Ma预警-"));
                    med.ksize = msize;
                    ret.append(med);
                }
            }
        }
    }
    return ret;
}

