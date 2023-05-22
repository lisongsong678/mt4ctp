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
    gram.append(0);//a21 1-��ͼ 0-��ͼ
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

//������ͼ��axisRectIndex��ֵ����ʹ��,groupҲ����ʹ�ã����Ƕ���X��ʹ�õ�
abountIndiPlot MAPlugin::initUiPlot(QCustomPlot *customPlot,QHash<QString,QHash<int,QVector<double>>> &indicatorbuffer,QVector<double> &inputpram,int axisRectIndex,QCPMarginGroup *group)
{
    abountIndiPlot tss;
    QPair<QString,QHash<QString,QVector<QCPAbstractPlottable *> > > retval;
    QHash<QString,QVector<QCPAbstractPlottable *> > t1;
    QVector<QCPAbstractPlottable *> pline;
    //QVector<QCPAbstractPlottable *> phistom;//����Ҫ��״ͼ
    //��ʽ����
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

    //�������
    for(int k=0;k<indiNameLis.count();++k)
    {
        int ksize = indicatorbuffer[indiNameLis.at(k)][0].count();//��ʼ��ʱʹ��1��������
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
    //��������ͼ���ʿɷ��ؿ�
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
                    indicatorbuffer[indiNameLis.at(i)][period].push_back(kl.close.at(j));//С�ڲ���ʱ�����̼�
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
            if(indicatorbuffer[indiNameLis.at(i)][period].count()==ksize)//����Ҫ����Ԫ�أ�ֻ��ȡ���Ԫ���޸ļ��� ���ݸ���
            {
                if(ksize<=start)//k����ĿС�ڲ��������� �����͵���0 ��ʲôҲ����
                    continue;
                else
                {
                    int output_length;
                    if(ksize<=(int)inputgram.at(i)*8)//k����Ŀ����start��С��8���Ĳ���ֵ  ȫ�����м���һ��
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
                    else//k����Ŀ����8���Ĳ���ֵ ֻȡ���8����Ŀ�����̼۽��м���
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
            else//��Ҫ����Ԫ��
            {
                if(ksize<=start)//k����ĿС�ڲ��������� �ڽ�β�ƽ�ȥ0
                {
                    indicatorbuffer[indiNameLis.at(i)][period].push_back(kl.close.at(ksize-1));
                }
                else
                {
                    int output_length;
                    if(ksize<=(int)inputgram.at(i)*8)//k����Ŀ����start��С��8���Ĳ���ֵ  ȫ�����м���һ��
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
                    else//k����Ŀ����8���Ĳ���ֵ ֻȡ���8����Ŀ�����̼۽��м���
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
           //�߸���
            if((qobject_cast<QCPGraph *>(plotOfIndicator["line"][k]))->dataCount()==ksize)//���ݸ���
            {
                QVector< QCPGraphData >::iterator it0 = (qobject_cast<QCPGraph *>(plotOfIndicator["line"][k]))->data()->end()-1;
                it0->value = indicatorbuffer[indiNameLis.at(k)][period][ksize-1];
            }
            else//����׷��
            {
                (qobject_cast<QCPGraph *>(plotOfIndicator["line"][k]))->addData(ksize-1,indicatorbuffer[indiNameLis.at(k)][period][ksize-1]);
            }
        }
    }
}
