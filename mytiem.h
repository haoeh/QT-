#ifndef MYTIEM_H
#define MYTIEM_H

#include <QObject>
#include<QThread>
class mytiem : public QThread
{
    Q_OBJECT
public:
    explicit mytiem(QObject *parent = nullptr);
    void run();
signals:
    void fashe(QStringList x);
};

#endif // MYTIEM_H
