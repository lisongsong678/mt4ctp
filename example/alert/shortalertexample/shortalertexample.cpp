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
            //ȥ������0������
            if(indicatordat["macd"]["macdv"][5][msize-1]<indicatordat["macd"]["macdv"][5][msize-2])//����һ��Ϊ�� //indicatordat["macd"]["macdv"][5][msize-1]>0 &&
            {
                if(indicatordat["macd"]["macdv"][5][msize-2]>=indicatordat["macd"]["macdv"][5][msize-3])//�����ڶ���Ϊ�� //indicatordat["macd"]["macdv"][5][msize-2]>0 &&
                {
                    //ret.flag =true;
                    //ret.alertPeriod.append(QString::fromLocal8Bit("15��������"));
                    //ret.alertText.append(QString::fromLocal8Bit("MAԤ��-"));
                    alertRet med;
                    med.context.append(QString::fromLocal8Bit("�տ�"));
                    med.context.append(QString::fromLocal8Bit("15��������"));
                    med.context.append(QString::fromLocal8Bit("MaԤ��-"));
                    med.ksize = msize;
                    ret.append(med);
                }
            }
        }
        //30���Ӵ���

        //�ж�MACDָ���Ƿ��ɺ���
        msize=indicatordat["macd"]["macdv"][6].size();
        if(msize>=26)
        {
            //ȥ������0������
            if(indicatordat["macd"]["macdv"][6][msize-1]<indicatordat["macd"]["macdv"][6][msize-2])//����һ��Ϊ�� //indicatordat["macd"]["macdv"][6][msize-1]>0 &&
            {
                if(indicatordat["macd"]["macdv"][6][msize-2]>=indicatordat["macd"]["macdv"][6][msize-3])//�����ڶ���Ϊ�� //indicatordat["macd"]["macdv"][6][msize-2]>0 &&
                {
                    //ret.flag =true;
                    //ret.alertPeriod.append(QString::fromLocal8Bit("30��������+"));
                    //ret.alertText.append(QString::fromLocal8Bit("MAԤ��-"));
                    alertRet med;
                    med.context.append(QString::fromLocal8Bit("�տ�"));
                    med.context.append(QString::fromLocal8Bit("30��������+"));
                    med.context.append(QString::fromLocal8Bit("MaԤ��-"));
                    med.ksize = msize;
                    ret.append(med);
                }
            }
        }

        //60���Ӵ���

        //�ж�MACDָ���Ƿ����̱��
        msize=indicatordat["macd"]["macdv"][7].size();
        if(msize>=26)
        {
            //ȥ������0������
            if(indicatordat["macd"]["macdv"][7][msize-1]<indicatordat["macd"]["macdv"][7][msize-2])//����һ��Ϊ�� //indicatordat["macd"]["macdv"][7][msize-1]>0 &&
            {
                if(indicatordat["macd"]["macdv"][7][msize-2]>=indicatordat["macd"]["macdv"][7][msize-3])//�����ڶ���Ϊ�� //indicatordat["macd"]["macdv"][7][msize-2]>0 &&
                {
                    //ret.flag =true;
                    //ret.alertPeriod.append(QString::fromLocal8Bit("1Сʱ����+"));
                    //ret.alertText.append(QString::fromLocal8Bit("MAԤ��-"));
                    alertRet med;
                    med.context.append(QString::fromLocal8Bit("�տ�"));
                    med.context.append(QString::fromLocal8Bit("1Сʱ����+"));
                    med.context.append(QString::fromLocal8Bit("MaԤ��-"));
                    med.ksize = msize;
                    ret.append(med);
                }
            }
        }

        //120���Ӵ���

        //�ж�MACDָ���Ƿ����̱��
        msize=indicatordat["macd"]["macdv"][8].size();
        if(msize>=26)
        {
            //ȥ������0������
            if(indicatordat["macd"]["macdv"][8][msize-1]<indicatordat["macd"]["macdv"][8][msize-2])//����һ��Ϊ�� //indicatordat["macd"]["macdv"][8][msize-1]>0 &&
            {
                if(indicatordat["macd"]["macdv"][8][msize-2]>=indicatordat["macd"]["macdv"][8][msize-3])//�����ڶ���Ϊ�� //indicatordat["macd"]["macdv"][8][msize-2]>0 &&
                {
                    //ret.flag =true;
                    //ret.alertPeriod.append(QString::fromLocal8Bit("2Сʱ����+"));
                    //ret.alertText.append(QString::fromLocal8Bit("MAԤ��-"));
                    alertRet med;
                    med.context.append(QString::fromLocal8Bit("�տ�"));
                    med.context.append(QString::fromLocal8Bit("2Сʱ����+"));
                    med.context.append(QString::fromLocal8Bit("MaԤ��-"));
                    med.ksize = msize;
                    ret.append(med);
                }
            }
        }

        //240���Ӵ���
        //�ж�MACDָ���Ƿ����̱��
        msize=indicatordat["macd"]["macdv"][9].size();
        if(msize>=26)
        {
            //ȥ������0������
            if(indicatordat["macd"]["macdv"][9][msize-1]>0 && indicatordat["macd"]["macdv"][9][msize-1]<indicatordat["macd"]["macdv"][9][msize-2])//����һ��Ϊ��
            {
                if(indicatordat["macd"]["macdv"][9][msize-2]>0 && indicatordat["macd"]["macdv"][9][msize-2]>=indicatordat["macd"]["macdv"][9][msize-3])//�����ڶ���Ϊ��
                {
                    //ret.flag =true;
                    //ret.alertPeriod.append(QString::fromLocal8Bit("4Сʱ����+"));
                    //ret.alertText.append(QString::fromLocal8Bit("MAԤ��-"));
                    alertRet med;
                    med.context.append(QString::fromLocal8Bit("�տ�"));
                    med.context.append(QString::fromLocal8Bit("4Сʱ����+"));
                    med.context.append(QString::fromLocal8Bit("MaԤ��-"));
                    med.ksize = msize;
                    ret.append(med);
                }
            }
        }

        //���ߴ���
        //�ж�MACDָ���Ƿ����̱��
        msize=indicatordat["macd"]["macdv"][10].size();
        if(msize>=25)
        {
            //ȥ������0������
            if(indicatordat["macd"]["macdv"][10][msize-1]>0 && indicatordat["macd"]["macdv"][10][msize-1]<indicatordat["macd"]["macdv"][10][msize-2])//����һ��Ϊ��
            {
                if(indicatordat["macd"]["macdv"][10][msize-2]>0 && indicatordat["macd"]["macdv"][10][msize-2]>=indicatordat["macd"]["macdv"][10][msize-3])//�����ڶ���Ϊ��
                {
                    //ret.flag =true;
                    //ret.alertPeriod.append(QString::fromLocal8Bit("��������+"));
                    //ret.alertText.append(QString::fromLocal8Bit("MAԤ��-"));
                    alertRet med;
                    med.context.append(QString::fromLocal8Bit("�տ�"));
                    med.context.append(QString::fromLocal8Bit("��������+"));
                    med.context.append(QString::fromLocal8Bit("MaԤ��-"));
                    med.ksize = msize;
                    ret.append(med);
                }
            }
        }
    }
    return ret;
}

