#include "macdplugin.h"
#include <indicators.h>



MacdPlugin::MacdPlugin(QObject *parent) : QObject(parent)
{

}

QString MacdPlugin::getFileName()
{
    return QString("macd");
}

indicatorParam MacdPlugin::initIndicatorBuffer()
{
    indicatorParam retval;
    QStringList indiNameLis;
    indiNameLis.append("macd");
    indiNameLis.append("diff");
    indiNameLis.append("dea");
    indiNameLis.append("macdv");
    QVector<double> gram;
    gram.append(1);//a21 1-��ͼ 0-��ͼ
    gram.append(12);
    gram.append(26);
    gram.append(9);
    QPair<QStringList,QVector<double>> nameOrParam;
    nameOrParam.first=indiNameLis;
    nameOrParam.second = gram;
    retval.nameOrParam = nameOrParam;
    return retval;
}

abountIndiPlot MacdPlugin::initUiPlot(QCustomPlot *customPlot,QHash<QString,QHash<int,QVector<double>>> &indicatorbuffer,QVector<double> &inputpram,int axisRectIndex,QCPMarginGroup *group)
{
    abountIndiPlot tss;
    QPair<QString,QHash<QString,QVector<QCPAbstractPlottable *> > > retval;
    QHash<QString,QVector<QCPAbstractPlottable *> > t1;
    QVector<QCPAbstractPlottable *> pline;
    QVector<QCPAbstractPlottable *> phistom;

    QCPAxisRect *macdAxisRect = new QCPAxisRect(customPlot);
    customPlot->plotLayout()->addElement(axisRectIndex, 0, macdAxisRect);
    //macdAxisRect->setMaximumSize(QSize(QWIDGETSIZE_MAX, 100));
    macdAxisRect->axis(QCPAxis::atBottom)->setLayer("axes");
    macdAxisRect->axis(QCPAxis::atBottom)->grid()->setLayer("grid");

    //group = new QCPMarginGroup(customPlot);//a37
    macdAxisRect->setRangeDrag(Qt::Horizontal);
    macdAxisRect->setRangeZoom(Qt::Horizontal);
    macdAxisRect->setAutoMargins(QCP::msLeft|QCP::msRight|QCP::msBottom);
    macdAxisRect->setMargins(QMargins(0, 0, 0, 0));
    macdAxisRect->setMarginGroup(QCP::msLeft|QCP::msRight, group);

    customPlot->plotLayout()->setRowStretchFactor(axisRectIndex,2);//��ͼ������������


    QCPGraph *diffline  = customPlot->addGraph(macdAxisRect->axis(QCPAxis::atBottom), macdAxisRect->axis(QCPAxis::atLeft));
    pline.append(diffline);
    diffline->setPen(QPen(Qt::blue));

    QCPGraph *dealine = customPlot->addGraph(macdAxisRect->axis(QCPAxis::atBottom), macdAxisRect->axis(QCPAxis::atLeft));
    pline.append(dealine);
    dealine->setPen(QPen(Qt::magenta));

    int ksize = indicatorbuffer["diff"][0].count();//��ʼ��ʱʹ��1��������
    double maxgram = *(std::max_element)(inputpram.constBegin(),inputpram.constEnd());
    if(ksize>(int)maxgram)//a36
    {
        QVector<double> ketd(ksize);
        for(int i=0;i<ksize;++i)
            ketd[i]=i;
        diffline->setData(ketd,indicatorbuffer["diff"][0],true);
        dealine->setData(ketd,indicatorbuffer["dea"][0],true);
    }

    QCPBars *redMACD = new QCPBars(macdAxisRect->axis(QCPAxis::atBottom), macdAxisRect->axis(QCPAxis::atLeft));
    QCPBars *whiteMACD = new QCPBars(macdAxisRect->axis(QCPAxis::atBottom), macdAxisRect->axis(QCPAxis::atLeft));
    QCPBars *greenMACD = new QCPBars(macdAxisRect->axis(QCPAxis::atBottom), macdAxisRect->axis(QCPAxis::atLeft));
    QCPBars *yellowMACD = new QCPBars(macdAxisRect->axis(QCPAxis::atBottom), macdAxisRect->axis(QCPAxis::atLeft));
    phistom.append(redMACD);
    phistom.append(whiteMACD);
    phistom.append(greenMACD);
    phistom.append(yellowMACD);

    redMACD->setPen(Qt::NoPen);
    redMACD->setBrush(Qt::red);
    whiteMACD->setBrush(Qt::white);
    greenMACD->setBrush(Qt::green);
    yellowMACD->setBrush(Qt::yellow);
    if(ksize>(int)maxgram)//a36
    {
        redMACD->addData(0,indicatorbuffer["macdv"][0][0]);//��һ��0����1��������
        for(int i=1;i<ksize;++i)
        {
            if(indicatorbuffer["macdv"][0][i]>0 && indicatorbuffer["macdv"][0][i]>=indicatorbuffer["macdv"][0][i-1])
                redMACD->addData(i,indicatorbuffer["macdv"][0][i]);
            if(indicatorbuffer["macdv"][0][i]>0 && indicatorbuffer["macdv"][0][i]<indicatorbuffer["macdv"][0][i-1])
                whiteMACD->addData(i,indicatorbuffer["macdv"][0][i]);
            if(indicatorbuffer["macdv"][0][i]<=0 && indicatorbuffer["macdv"][0][i]<indicatorbuffer["macdv"][0][i-1])
                greenMACD->addData(i,indicatorbuffer["macdv"][0][i]);
            if(indicatorbuffer["macdv"][0][i]<=0 && indicatorbuffer["macdv"][0][i]>=indicatorbuffer["macdv"][0][i-1])
                yellowMACD->addData(i,indicatorbuffer["macdv"][0][i]);
        }
    }
    t1.insert("line",pline);
    t1.insert("histogram",phistom);
    retval.first = "macd";
    retval.second = t1;
    tss.indicatorplots=retval;
    tss.updatAxisrect.first = macdAxisRect;
    tss.updatAxisrect.second = true;//y����Ҫ����ֵ�仯����
    return tss;
}



void MacdPlugin::calIndicatorData(KLine &kl,int period,QVector<double> &inputpramm,QHash<QString,QHash<int,QVector<double>>> &indicatorbuffer,bool isallcal)
{
    int ksize= kl.close.count();
    if(ksize==0)//a156
    {
        qDebug()<<"ksize==0";
        return;
    }
    const double options[] = {inputpramm.at(0),inputpramm.at(1),inputpramm.at(2)};

    int start = ti_macd_start(options);
    double *pinput0=kl.close.data();
        if(isallcal)//��������ʱ����
        {
            if(ksize<=start)//k����ĿС�ڲ������������
            {
                for(int j=0;j<ksize;++j)
                {
                    indicatorbuffer["diff"][period].push_back(0);
                    indicatorbuffer["dea"][period].push_back(0);
                    indicatorbuffer["macdv"][period].push_back(0);
                }
                return;
            }
            int output_length = ksize - start;
            double *all_inputs[] = {pinput0};
            QVector<double> outbuffer0(output_length,0);
            QVector<double> outbuffer1(output_length,0);
            QVector<double> outbuffer2(output_length,0);
            double *all_outputs[] = {outbuffer0.data(),outbuffer1.data(),outbuffer2.data()};
            int error = ti_macd(ksize, all_inputs, options, all_outputs);
            if(error==TI_INVALID_OPTION)
            {
                qDebug()<<"indicator param error";
                return;
            }
            for(int j=0;j<start;++j)
            {
                indicatorbuffer["diff"][period].push_back(0);
                indicatorbuffer["dea"][period].push_back(0);
                indicatorbuffer["macdv"][period].push_back(0);
            }
            for(int j=start;j<ksize;++j)
            {
                indicatorbuffer["diff"][period].push_back(outbuffer0[j-start]);
                indicatorbuffer["dea"][period].push_back(outbuffer1[j-start]);
                indicatorbuffer["macdv"][period].push_back( outbuffer2[j-start]*2);
            }
        }
        else//run�߳��е���
        {
            if(indicatorbuffer["macdv"][period].count()==ksize)//����Ҫ����Ԫ�أ�ֻ��ȡ���Ԫ���޸ļ��� ���ݸ���
            {
                if(ksize<=start)//k����ĿС�ڲ������������ �����͵���0 ��ʲôҲ����
                    return;
                else
                {
                   int output_length;
                   double maxgram = start *8 ;//����Ҫ���һ������ʱ��ֻ����8��������ڵ���Ŀ���м���
                   if(ksize<=(int)maxgram)//k����Ŀ����start��С��8���Ĳ������ֵ  ȫ�����м���һ��
                   {
                       output_length = ksize - start;
                       double *all_inputs[] = {pinput0};
                       QVector<double> outbuffer0(output_length,0);
                       QVector<double> outbuffer1(output_length,0);
                       QVector<double> outbuffer2(output_length,0);
                       double *all_outputs[] = {outbuffer0.data(),outbuffer1.data(),outbuffer2.data()};
                       int error = ti_macd(ksize, all_inputs, options, all_outputs);
                       if(error==TI_INVALID_OPTION)
                       {
                           qDebug()<<"indicator param error";
                           return;
                       }
                       double &diff = indicatorbuffer["diff"][period].last();
                       double &dea = indicatorbuffer["dea"][period].last();
                       double &macdv = indicatorbuffer["macdv"][period].last();
                       diff=outbuffer0[output_length-1];
                       dea=outbuffer1[output_length-1];
                       macdv=outbuffer2[output_length-1]*2;
                   }
                   else//k����Ŀ����8���Ĳ������ֵ ֻȡ���8����Ŀ�����̼۽��м���
                   {
                       int minsize = (int)maxgram;
                       output_length = minsize - start;
                       double *all_inputs[] = {&pinput0[ksize-minsize]};
                       QVector<double> outbuffer0(output_length,0);
                       QVector<double> outbuffer1(output_length,0);
                       QVector<double> outbuffer2(output_length,0);
                       double *all_outputs[] = {outbuffer0.data(),outbuffer1.data(),outbuffer2.data()};
                       int error = ti_macd(minsize, all_inputs, options, all_outputs);
                       if(error==TI_INVALID_OPTION)
                       {
                           qDebug()<<"indicator param error";
                           return;
                       }
                       double &diff = indicatorbuffer["diff"][period].last();
                       double &dea = indicatorbuffer["dea"][period].last();
                       double &macdv = indicatorbuffer["macdv"][period].last();
                       diff=outbuffer0[output_length-1];
                       dea=outbuffer1[output_length-1];
                       macdv=outbuffer2[output_length-1]*2;
                   }
                }
            }
            else//��Ҫ����Ԫ��
            {
                if(ksize<=start)//k����ĿС�ڲ������������ �ڽ�β�ƽ�ȥ0
                {
                    indicatorbuffer["diff"][period].push_back(0);
                    indicatorbuffer["dea"][period].push_back(0);
                    indicatorbuffer["macdv"][period].push_back(0);
                }
                else
                {
                    int output_length;
                    double maxgram = start*8 ;//����Ҫ���һ������ʱ��ֻ����8��������ڵ���Ŀ���м���
                   if(ksize<=(int)maxgram)//k����Ŀ����start��С��8���Ĳ������ֵ  ȫ�����м���һ��
                   {
                       output_length = ksize - start;
                       double *all_inputs[] = {pinput0};
                       QVector<double> outbuffer0(output_length,0);
                       QVector<double> outbuffer1(output_length,0);
                       QVector<double> outbuffer2(output_length,0);
                       double *all_outputs[] = {outbuffer0.data(),outbuffer1.data(),outbuffer2.data()};
                       int error = ti_macd(ksize, all_inputs, options, all_outputs);
                       if(error==TI_INVALID_OPTION)
                       {
                           qDebug()<<"indicator param error";
                           return;
                       }
                       indicatorbuffer["diff"][period].push_back(outbuffer0[output_length-1]);
                       indicatorbuffer["dea"][period].push_back(outbuffer1[output_length-1]);
                       indicatorbuffer["macdv"][period].push_back(outbuffer2[output_length-1]*2);
                   }
                   else//k����Ŀ����8���Ĳ������ֵ ֻȡ���8����Ŀ�����̼۽��м���
                   {
                       int minsize = (int)maxgram;
                       output_length = minsize - start;
                       double *all_inputs[] = {&pinput0[ksize-minsize]};
                       QVector<double> outbuffer0(output_length,0);
                       QVector<double> outbuffer1(output_length,0);
                       QVector<double> outbuffer2(output_length,0);
                       double *all_outputs[] = {outbuffer0.data(),outbuffer1.data(),outbuffer2.data()};
                       int error = ti_macd(minsize, all_inputs, options, all_outputs);
                       if(error==TI_INVALID_OPTION)
                       {
                           qDebug()<<"indicator param error";
                           return;
                       }
                       indicatorbuffer["diff"][period].push_back(outbuffer0[output_length-1]);
                       indicatorbuffer["dea"][period].push_back(outbuffer1[output_length-1]);
                       indicatorbuffer["macdv"][period].push_back(outbuffer2[output_length-1]*2);
                   }
                }
            }
        }

}


void MacdPlugin::updatePlot(QHash<QString,QHash<int,QVector<double>>> &indicatorbuffer,QHash<QString,QVector<QCPAbstractPlottable *> > &plotOfIndicator, QVector<double> &inputgram,int period,bool isallupdated)
{
    int ksize = indicatorbuffer["macdv"][period].count();
    double maxgram = *(std::max_element)(inputgram.constBegin(),inputgram.constEnd());
    if(ksize<(int)maxgram)//a36
        return;
    QVector<double> ketd(ksize);
    for(int i=0;i<ksize;++i)
        ketd[i]=i;
    if(isallupdated)
    {
        //���ػ�
        (qobject_cast<QCPGraph *>(plotOfIndicator["line"][0]))->setData(ketd,indicatorbuffer["diff"][period],true);
        (qobject_cast<QCPGraph *>(plotOfIndicator["line"][1]))->setData(ketd,indicatorbuffer["dea"][period],true);

        //���ػ�
        (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][0]))->data()->clear();//�����
        (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][1]))->data()->clear();
        (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][2]))->data()->clear();
        (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][3]))->data()->clear();
        (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][0]))->addData(0,indicatorbuffer["macdv"][period][0]);
        for(int i=1;i<ksize;++i)
        {
            if(indicatorbuffer["macdv"][period][i]>0 && indicatorbuffer["macdv"][period][i]>=indicatorbuffer["macdv"][period][i-1])
                (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][0]))->addData(i,indicatorbuffer["macdv"][period][i]);
            if(indicatorbuffer["macdv"][period][i]>0 && indicatorbuffer["macdv"][period][i]<indicatorbuffer["macdv"][period][i-1])
                (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][1]))->addData(i,indicatorbuffer["macdv"][period][i]);
            if(indicatorbuffer["macdv"][period][i]<=0 && indicatorbuffer["macdv"][period][i]<indicatorbuffer["macdv"][period][i-1])
                (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][2]))->addData(i,indicatorbuffer["macdv"][period][i]);
            if(indicatorbuffer["macdv"][period][i]<=0 && indicatorbuffer["macdv"][period][i]>=indicatorbuffer["macdv"][period][i-1])
                (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][3]))->addData(i,indicatorbuffer["macdv"][period][i]);
        }

    }
    else
    {
        //�߸���
        if((qobject_cast<QCPGraph *>(plotOfIndicator["line"][0]))->dataCount()==ksize)//���ݸ���
        {
           QVector< QCPGraphData >::iterator it0 = (qobject_cast<QCPGraph *>(plotOfIndicator["line"][0]))->data()->end()-1;
           QVector< QCPGraphData >::iterator it1 = (qobject_cast<QCPGraph *>(plotOfIndicator["line"][1]))->data()->end()-1;
           it0->value = indicatorbuffer["diff"][period][ksize-1];
           it1->value = indicatorbuffer["dea"][period][ksize-1];
        }
        else {// ����׷��
           (qobject_cast<QCPGraph *>(plotOfIndicator["line"][0]))->addData(ksize-1,indicatorbuffer["diff"][period][ksize-1]);
           (qobject_cast<QCPGraph *>(plotOfIndicator["line"][1]))->addData(ksize-1,indicatorbuffer["dea"][period][ksize-1]);
        }

        //������
        int k1 = (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][0]))->dataCount();
        int k2 = (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][1]))->dataCount();
        int k3 = (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][2]))->dataCount();
        int k4 = (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][3]))->dataCount();
        if(k1+k2+k3+k4==ksize)//���ݸ���
        {
            QVector< QCPBarsData >::iterator it0 = (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][0]))->data()->end()-1;
            QVector< QCPBarsData >::iterator it1 = (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][1]))->data()->end()-1;
            QVector< QCPBarsData >::iterator it2 = (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][2]))->data()->end()-1;
            QVector< QCPBarsData >::iterator it3 = (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][3]))->data()->end()-1;
            if(int(it0->key)==ksize-1)//�ҵ����һ��key�����ĸ�QCPBars���Ƴ�--------������Բ��Ƴ����ڵ�������ֱ���޸�--���У�һ��Ҫֱ���Ƴ�-�ڵ���K����ɫҲ�����ر仯���������
                (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][0]))->data()->remove(it0->key);
            if(int(it1->key)==ksize-1)
                (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][1]))->data()->remove(it1->key);
            if(int(it2->key)==ksize-1)
                (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][2]))->data()->remove(it2->key);
            if(int(it3->key)==ksize-1)
                (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][3]))->data()->remove(it3->key);

            if(indicatorbuffer["macdv"][period][ksize-1]>0 && indicatorbuffer["macdv"][period][ksize-1]>=indicatorbuffer["macdv"][period][ksize-1-1])
                (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][0]))->addData(ksize-1,indicatorbuffer["macdv"][period][ksize-1]);
            if(indicatorbuffer["macdv"][period][ksize-1]>0 && indicatorbuffer["macdv"][period][ksize-1]<indicatorbuffer["macdv"][period][ksize-1-1])
                (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][1]))->addData(ksize-1,indicatorbuffer["macdv"][period][ksize-1]);
            if(indicatorbuffer["macdv"][period][ksize-1]<=0 && indicatorbuffer["macdv"][period][ksize-1]<indicatorbuffer["macdv"][period][ksize-1-1])
                (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][2]))->addData(ksize-1,indicatorbuffer["macdv"][period][ksize-1]);
            if(indicatorbuffer["macdv"][period][ksize-1]<=0 && indicatorbuffer["macdv"][period][ksize-1]>=indicatorbuffer["macdv"][period][ksize-1-1])
                (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][3]))->addData(ksize-1,indicatorbuffer["macdv"][period][ksize-1]);
        }        
        else//����׷��
        {
            if(indicatorbuffer["macdv"][period][ksize-1]>0 && indicatorbuffer["macdv"][period][ksize-1]>=indicatorbuffer["macdv"][period][ksize-1-1])
                (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][0]))->addData(ksize-1,indicatorbuffer["macdv"][period][ksize-1]);
            if(indicatorbuffer["macdv"][period][ksize-1]>0 && indicatorbuffer["macdv"][period][ksize-1]<indicatorbuffer["macdv"][period][ksize-1-1])
                (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][1]))->addData(ksize-1,indicatorbuffer["macdv"][period][ksize-1]);
            if(indicatorbuffer["macdv"][period][ksize-1]<=0 && indicatorbuffer["macdv"][period][ksize-1]<indicatorbuffer["macdv"][period][ksize-1-1])
                (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][2]))->addData(ksize-1,indicatorbuffer["macdv"][period][ksize-1]);
            if(indicatorbuffer["macdv"][period][ksize-1]<=0 && indicatorbuffer["macdv"][period][ksize-1]>=indicatorbuffer["macdv"][period][ksize-1-1])
                (qobject_cast<QCPBars *>(plotOfIndicator["histogram"][3]))->addData(ksize-1,indicatorbuffer["macdv"][period][ksize-1]);
        }

    }
}
