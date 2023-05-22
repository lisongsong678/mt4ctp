#include "maplugin.h"
#include <indicators.h>

MAPlugin::MAPlugin(QObject *parent) : QObject(parent)
{

}

QString MAPlugin::getFileName()
{
    return QString("MA");
}

indicatorParam MAPlugin::initIndicatorBuffer()
{
    indicatorParam retval;
    QStringList indiNameLis;
    indiNameLis.append("MA");
    indiNameLis.append("MA5");
    //indiNameLis.append("MA8");
    indiNameLis.append("MA10");
    indiNameLis.append("MA21");
    indiNameLis.append("MA40");
    indiNameLis.append("MA60");
    //indiNameLis.append("MA89");
    //indiNameLis.append("MA144");
    //indiNameLis.append("MA233");
    QVector<double> gram;
    gram.append(0);//a21 1-副图 0-主图
    //gram.append(5);
    gram.append(5);
    gram.append(10);
    gram.append(21);
    gram.append(40);
    gram.append(60);
    //gram.append(89);
    //gram.append(144);
    //gram.append(233);
    QPair<QStringList,QVector<double>> nameOrParam;
    nameOrParam.first=indiNameLis;
    nameOrParam.second = gram;
    retval.nameOrParam = nameOrParam;
    return retval;
}

//对于主图，axisRectIndex的值不被使用,group也不被使用，这是对齐X轴使用的
abountIndiPlot MAPlugin::initUiPlot(QCustomPlot *customPlot,QHash<QString,QHash<int,QVector<double>>> &indicatorbuffer,QVector<double> &inputpram,int axisRectIndex,QCPMarginGroup *group)
{
    abountIndiPlot tss;
    QPair<QString,QHash<QString,QVector<QCPAbstractPlottable *> > > retval;
    QHash<QString,QVector<QCPAbstractPlottable *> > t1;
    QVector<QCPAbstractPlottable *> pline;
    //QVector<QCPAbstractPlottable *> phistom;//不需要柱状图
    //样式设置
    QPen pen;
    QCPGraph *MA5 = customPlot->addGraph(customPlot->xAxis,customPlot->yAxis);
    pen.setStyle(Qt::DashDotLine);
    pen.setColor(Qt::black);
    MA5->setPen(pen);//ma8
    QCPGraph *MA10 = customPlot->addGraph(customPlot->xAxis,customPlot->yAxis);
    pen.setColor(QColor(33,136,104));
    MA10->setPen(pen);//ma13
    QCPGraph *MA21 = customPlot->addGraph(customPlot->xAxis,customPlot->yAxis);
    pen.setColor(QColor(176,48,96));
    MA21->setPen(pen);//ma21
    QCPGraph *MA40 = customPlot->addGraph(customPlot->xAxis,customPlot->yAxis);
    pen.setColor(QColor(255,0,255));
    MA40->setPen(pen);//ma34
    QCPGraph *MA60 = customPlot->addGraph(customPlot->xAxis,customPlot->yAxis);
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(178,34,34));
    /*
    MA55->setPen(pen);//ma55
    QCPGraph *MA89 = customPlot->addGraph(customPlot->xAxis,customPlot->yAxis);
    pen.setColor(QColor(178,58,238));
    MA89->setPen(pen);//ma89
    QCPGraph *MA144 = customPlot->addGraph(customPlot->xAxis,customPlot->yAxis);
    pen.setColor(QColor(139,117,0));
    MA144->setPen(pen);//ma144
    QCPGraph *MA233 = customPlot->addGraph(customPlot->xAxis,customPlot->yAxis);
    pen.setColor(QColor(0,0,238));
    MA233->setPen(pen);//ma233
    */
    pline.append(MA5);
    pline.append(MA10);
    pline.append(MA21);
    pline.append(MA40);
    pline.append(MA60);
    /*
    pline.append(MA89);
    pline.append(MA144);
    pline.append(MA233);
    */
    QStringList indiNameLis;
    indiNameLis<<"MA5"<<"MA10"<<"MA21"<<"MA40"<<"MA60";

    //数据填充
    for(int k=0;k<indiNameLis.count();++k)
    {
        int ksize = indicatorbuffer[indiNameLis.at(k)][0].count();//初始化时使用1分钟数据
        if(ksize>(int)inputpram[k]) //if(ksize>=(int)inputpram[k])--->if(ksize>(int)inputpram[k])
        {
            QVector<double> ketd(ksize);
            for(int i=0;i<ksize;++i)
                ketd[i]=i;
            (qobject_cast<QCPGraph *>(pline[k]))->setData(ketd,indicatorbuffer[indiNameLis.at(k)][0],true);
        }
    }

    t1.insert("line",pline);
    retval.first = "MA";
    retval.second = t1;
    tss.indicatorplots=retval;
    //由于是主图，故可返回空
    tss.updatAxisrect.first = nullptr;
    tss.updatAxisrect.second = false;
    return tss;
}

void MAPlugin::calIndicatorData(KLine &kl,int period,QVector<double> &inputgram,QHash<QString,QHash<int,QVector<double>>> &indicatorbuffer,bool isallcal)
{
    int ksize= kl.close.count();
    if(ksize==0)//a156
    {
        qDebug()<<"ksize==0";
        return;
    }
    QStringList indiNameLis;
    indiNameLis<<"MA5"<<"MA10"<<"MA21"<<"MA40"<<"MA60";
    double *pinput0 = kl.close.data();
    if(isallcal)
    {
        for(int i=0;i<indiNameLis.count();++i)
        {
            const double options[] = {inputgram.at(i)};
            int start = ti_sma_start(options);
            int output_length = ksize - start;
            if(ksize<=start)
            {
                for(int j=0;j<ksize;++j)
                {
                    indicatorbuffer[indiNameLis.at(i)][period].push_back(kl.close.at(j));//小于参数时推收盘价
                }
                continue;
            }
            double *all_inputs[] = {pinput0};
            QVector<double> outbuffer0(output_length,0);
            double *all_outputs[] = {outbuffer0.data()};
            int error = ti_sma(ksize, all_inputs, options, all_outputs);
            if(error==TI_INVALID_OPTION)
            {
                qDebug()<<"indicator param error"<<indiNameLis.at(i);
                continue;
            }
            for(int j=0;j<start;++j)
                indicatorbuffer[indiNameLis.at(i)][period].push_back(kl.close.at(j));
            for(int j=start;j<ksize;++j)
                indicatorbuffer[indiNameLis.at(i)][period].push_back(outbuffer0[j-start]);
        }
    }
    else
    {
        for(int i=0;i<indiNameLis.count();++i)
        {
            const double options[] = {inputgram.at(i)};
            int start = ti_sma_start(options);
            if(indicatorbuffer[indiNameLis.at(i)][period].count()==ksize)//不需要增加元素，只需取最后元素修改即可 数据更新
            {
                if(ksize<=start)//k线数目小于参数的周期 本来就等于0 则什么也不做
                    continue;
                else
                {
                    int output_length;
                    if(ksize<=(int)inputgram.at(i)*8)//k线数目大于start且小于8倍的参数值  全部进行计算一遍
                    {
                        output_length = ksize - start;
                        double *all_inputs[] = {pinput0};
                        QVector<double> outbuffer0(output_length,0);
                        double *all_outputs[] = {outbuffer0.data()};
                        int error = ti_sma(ksize, all_inputs, options, all_outputs);
                        if(error==TI_INVALID_OPTION)
                        {
                            qDebug()<<"indicator param error"<<indiNameLis.at(i);
                            continue;
                        }
                        double &ma = indicatorbuffer[indiNameLis.at(i)][period].last();
                        ma=outbuffer0[output_length-1];
                    }
                    else//k线数目大于8倍的参数值 只取最后8倍数目的收盘价进行计算
                    {
                        int minsize = (int)inputgram.at(i)*8;
                        output_length = minsize - start;
                        double *all_inputs[] = {&pinput0[ksize-minsize]};
                        QVector<double> outbuffer0(output_length,0);
                        double *all_outputs[] = {outbuffer0.data()};
                        int error = ti_sma(minsize, all_inputs, options, all_outputs);
                        if(error==TI_INVALID_OPTION)
                        {
                            qDebug()<<"indicator param error"<<indiNameLis.at(i);
                            continue;
                        }
                        double &ma = indicatorbuffer[indiNameLis.at(i)][period].last();
                        ma=outbuffer0[output_length-1];
                    }
                }

            }
            else//需要增加元素
            {
                if(ksize<=start)//k线数目小于参数的周期 在结尾推进去0
                {
                    indicatorbuffer[indiNameLis.at(i)][period].push_back(kl.close.at(ksize-1));
                }
                else
                {
                    int output_length;
                    if(ksize<=(int)inputgram.at(i)*8)//k线数目大于start且小于8倍的参数值  全部进行计算一遍
                    {
                        output_length = ksize - start;
                        double *all_inputs[] = {pinput0};
                        QVector<double> outbuffer0(output_length,0);
                        double *all_outputs[] = {outbuffer0.data()};
                        int error = ti_sma(ksize, all_inputs, options, all_outputs);
                        if(error==TI_INVALID_OPTION)
                        {
                            qDebug()<<"indicator param error"<<indiNameLis.at(i);
                            continue;
                        }
                        indicatorbuffer[indiNameLis.at(i)][period].push_back(outbuffer0[output_length-1]);
                    }
                    else//k线数目大于8倍的参数值 只取最后8倍数目的收盘价进行计算
                    {
                        int minsize = (int)inputgram.at(i)*8;
                        output_length = minsize - start;
                        double *all_inputs[] = {&pinput0[ksize-minsize]};
                        QVector<double> outbuffer0(output_length,0);
                        double *all_outputs[] = {outbuffer0.data()};
                        int error = ti_sma(minsize, all_inputs, options, all_outputs);
                        if(error==TI_INVALID_OPTION)
                        {
                            qDebug()<<"indicator param error"<<indiNameLis.at(i);
                            continue;
                        }
                        indicatorbuffer[indiNameLis.at(i)][period].push_back(outbuffer0[output_length-1]);
                    }
                }
            }
        }
    }
}

void MAPlugin::updatePlot(QHash<QString,QHash<int,QVector<double>>> &indicatorbuffer,QHash<QString,QVector<QCPAbstractPlottable *> > &plotOfIndicator, QVector<double> &inputgram,int period,bool isallupdated)
{
    QStringList indiNameLis;
    //indiNameLis<<"MA8"<<"MA13"<<"MA21"<<"MA34"<<"MA55"<<"MA89"<<"MA144"<<"MA233";
    indiNameLis<<"MA5"<<"MA10"<<"MA21"<<"MA40"<<"MA60";
    for(int k=0;k<indiNameLis.count();++k)
    {
        int ksize = indicatorbuffer[indiNameLis.at(k)][period].count();
        if(ksize<(int)inputgram[k])//a36
            continue;
        QVector<double> ketd(ksize);
        for(int i=0;i<ksize;++i)
            ketd[i]=i;
        if(isallupdated)
        {
           (qobject_cast<QCPGraph *>(plotOfIndicator["line"][k]))->setData(ketd,indicatorbuffer[indiNameLis.at(k)][period],true);
        }
        else
        {
           //线更新
            if((qobject_cast<QCPGraph *>(plotOfIndicator["line"][k]))->dataCount()==ksize)//数据更新
            {
                QVector< QCPGraphData >::iterator it0 = (qobject_cast<QCPGraph *>(plotOfIndicator["line"][k]))->data()->end()-1;
                it0->value = indicatorbuffer[indiNameLis.at(k)][period][ksize-1];
            }
            else//数据追加
            {
                (qobject_cast<QCPGraph *>(plotOfIndicator["line"][k]))->addData(ksize-1,indicatorbuffer[indiNameLis.at(k)][period][ksize-1]);
            }
        }
    }
}
