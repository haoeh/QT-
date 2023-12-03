#include "tianqi.h"

#include <QHostInfo>

tianqi::tianqi(QObject *parent)
    : QObject{parent}
{
    //初始化，进行获取ip等信息；
    manager=new QNetworkAccessManager(this);
    quest.setUrl(QUrl("http://ip-api.com/json"));//先获取ip
    reply_ip=manager->get(quest);

    connect(reply_ip,&QNetworkReply::readyRead,this,&tianqi::jieShou_ip);
}



void tianqi::jieShou_ip()
{

    ip=QJsonDocument::fromJson(reply_ip->readAll()).object().value("query").toString();
    qDebug()<<ip;

    //获取到IP后进行定位
    quest.setUrl(QUrl("https://api.map.baidu.com/location/ip?ip="+ip+"&coor=bd09ll&ak=CZCQUhkUEspvwlDiotde7E1SGmvh3w3d"));//提供ip获取定位
    reply_diwei=manager->get(quest);
    connect(reply_diwei,&QNetworkReply::readyRead,this,&tianqi::jieShou_diwei);

}

void tianqi::jieShou_diwei()
{
    //对获取的信息进行处理
    dinWei=QJsonDocument::fromJson(reply_diwei->readAll()).object();
    QString binma=dinWei.value("content")["address_detail"]["adcode"].toString();//获取编码
    qDebug()<<binma;

    //定位编码获取好了之后，经行接收天气数据
    quest.setUrl(QUrl("https://api.map.baidu.com/weather/v1/?district_id="+binma+"&data_type=all&ak=CZCQUhkUEspvwlDiotde7E1SGmvh3w3d"));//提供地址编码获取天气
    reply_tianQi=manager->get(quest);
    connect(reply_tianQi,&QNetworkReply::readyRead,this,&tianqi::jieShou_tianQi);


}

void tianqi::jieShou_tianQi()
{
    tianQiShuJu=QJsonDocument::fromJson(reply_tianQi->readAll()).object();
    //基本信息处理好后，发出信号
    emit wanChen(tianQiShuJu);
}

void tianqi::souSuo(QString x)
{
    quest.setUrl(QUrl("https://restapi.amap.com/v3/config/district?keywords="+x+"&subdistrict=2&key=10c0bfa76564d10b27d25f5b69883f9e"));//提供地址编码获取天气
    reply_sousuo=manager->get(quest);
    //获取行政编码
    connect(reply_sousuo,&QNetworkReply::readyRead,this,[=](){
        QJsonDocument a=QJsonDocument::fromJson(reply_sousuo->readAll());
        adcode=a.object().value("districts")[0]["adcode"].toString();
        qDebug()<<adcode;
        if(adcode!="")
        {
            quest.setUrl(QUrl("https://api.map.baidu.com/weather/v1/?district_id="+adcode+"&data_type=all&ak=CZCQUhkUEspvwlDiotde7E1SGmvh3w3d"));
            //获取天气数据
            reply_sousuo=manager->get(quest);
            connect(reply_sousuo,&QNetworkReply::readyRead,[=](){
                sousuo_tianQiShuJu= QJsonDocument::fromJson(reply_sousuo->readAll()).object();
                if(sousuo_tianQiShuJu.value("message").toString()!="查询的区域编码无效")
                    emit chazhao_ok(sousuo_tianQiShuJu);
            });
        }
    });

}

QJsonObject tianqi::get_dinWei()
{
    return dinWei;
}

QJsonObject tianqi::get_tianQiShuJu()
{
    return tianQiShuJu;
}

tianqi::~tianqi()
{
    reply_ip->abort();//断开连接
    reply_tianQi->abort();
    reply_diwei->abort();
    manager->deleteLater();
    reply_ip->deleteLater();
    reply_diwei->deleteLater();
    reply_tianQi->deleteLater();
    //reply_sousuo->deleteLater();
}
