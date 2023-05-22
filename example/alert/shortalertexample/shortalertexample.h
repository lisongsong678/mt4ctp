#ifndef SHORTKDMALERT_H
#define SHORTKDMALERT_H

#include <QObject>
#include <QtPlugin>
#include "AlertInterface.h"

class ShortKdMalert : public QObject,AlertInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "plugin.alertshortmaexzaple")
    Q_INTERFACES(AlertInterface)
public:
    explicit ShortKdMalert(QObject *parent = nullptr);
    QString getFileName() override;
    QList<alertRet> funAlertCal(QHash<QString,QHash<QString,QHash<int,QVector<double>>>> &indicatordat,QVariant &medbuff) override;

signals:

};

#endif // SHORTKDMALERT_H
