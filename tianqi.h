#ifndef TIANQI_H
#define TIANQI_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include<QJsonValue>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QNetworkAccessManager>
class tianqi : public QObject
{
    Q_OBJECT
public:
    explicit tianqi(QObject *parent = nullptr);
    ~tianqi();

    void jieShou_ip();
    void jieShou_diwei();
    void jieShou_tianQi();
    void souSuo(QString x);

    QJsonObject get_dinWei();
    QJsonObject get_tianQiShuJu();


private:

    QJsonObject dinWei;//保存要搜索的定位信息;
    QJsonObject tianQiShuJu;//保存百度天气api的数据
    QJsonObject sousuo_tianQiShuJu;//保存百度天气api的数据


    QString ip;
    QString adcode;
    QNetworkAccessManager *manager;
    QNetworkReply *reply_ip;
    QNetworkReply *reply_diwei;
    QNetworkReply *reply_tianQi;
    QNetworkReply *reply_sousuo;

    QNetworkRequest quest;

signals:
    void wanChen(QJsonObject x);
    void chazhao_ok(QJsonObject x);
};

#endif // TIANQI_H
