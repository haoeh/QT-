#include "mytiem.h"

#include <QDateTime>
#include<QTimer>
mytiem::mytiem(QObject *parent)
    : QThread{parent}
{

}

void mytiem::run()
{
    QString time=QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm ddd");

    emit fashe(time.split(' '));

}
