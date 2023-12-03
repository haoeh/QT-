#include "widget.h"
#include "ui_widget.h"

#include <qtimer.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    my_tianqi=new tianqi();
    myt=new mytiem();
    //获取天气等信息；
    connect(my_tianqi,&tianqi::wanChen,this,&Widget::chuLiShuJu);
    //获取当前时间
    connect(myt,&mytiem::fashe,this,&Widget::time);
    //时间每30秒更新一次时间
    ti=new QTimer(this);
    ti->start(30000);
    myt->start();
    connect(ti,&QTimer::timeout,this,[=](){
        myt->start();
    });

    //相关控件加入管理链表
    label_list1.append(ui->label_12);
    label_list1.append(ui->label_15);
    label_list1.append(ui->label_18);
    label_list1.append(ui->label_21);
    //温度
    label_list2.append(ui->label_13);
    label_list2.append(ui->label_16);
    label_list2.append(ui->label_19);
    label_list2.append(ui->label_22);
    //星期
    label_list3.append(ui->label_14);
    label_list3.append(ui->label_17);
    label_list3.append(ui->label_20);
    label_list3.append(ui->label_23);
    connect(my_tianqi,&tianqi::chazhao_ok,this,&Widget::chuLiShuJu);


}

Widget::~Widget()
{
    delete ui;
    delete my_tianqi;
    myt->deleteLater();
}

void Widget::time(QStringList x)
{
    ui->label_2->setText(x[0]+" "+x[1]);

    ui->label_3->setText(xingqi(x[2]));
}

void Widget::chuLiShuJu(QJsonObject x)
{
    //定位数据的处理
    QString text=x.value("result")["location"]["name"].toString();
    ui->label->setText("      "+text);

    //进行天气数据的处理
    QString text_tianqi=x.value("result")["now"]["text"].toString();//天气情况
    ui->label_5->setText(QString::number(x.value("result")["now"]["temp"].toDouble())+"℃");//温度
    ui->label_6->setText(text_tianqi);
    ui->label_7->setText(QString::number(x.value("result")["forecasts"][0]["low"].toInt())+"~"+QString::number(x.value("result")["forecasts"][0]["high"].toInt()));
    ui->label_8->setText("风向："+x.value("result")["now"]["wind_dir"].toString()+x.value("result")["now"]["wind_class"].toString());
    ui->label_10->setText("体感温度:"+QString::number(x.value("result")["now"]["feels_like"].toInt())+"℃");
    ui->label_11->setText("湿度:"+QString::number(x.value("result")["now"]["rh"].toInt())+"％");

    //根据天气情况更换图标和背景
    tuBiao(text_tianqi,ui->label_4);
    beiJin(text_tianqi);

    forecasts(x);
}



void Widget::tuBiao(QString x,QLabel *y)
{
    if(x=="多云"||x=="阴")
    {
        y->setStyleSheet("border-image: url(:/clouds.png);");
    }
    else if(x=="晴")
    {
        y->setStyleSheet("border-image: url(:/sun.png);");
    }
    else
    {
        y->setStyleSheet("border-image: url(:/rainy.png);");
    }
}

void Widget::beiJin(QString x)
{
    if(x=="多云"||x=="阴")
    {
        ui->widget_2->setStyleSheet("#widget_2{border-image: url(:/yin.jpg)};");
    }
    else if(x=="晴")
    {
        ui->widget_2->setStyleSheet("#widget_2{border-image: url(:/qin.jpg)};");
    }
    else
    {
        ui->widget_2->setStyleSheet("#widget_2{border-image: url(:/yu.jpg)};");
    }
}

QString Widget::xingqi(QString x)
{
    if (x == "Sun") {
        return  "星期天   ";
    }
    else if (x == "Mon") {
        return  "星期一   ";
    }
    else if (x == "Tue") {
        return  "星期二   ";
    }
    else if (x == "Wed") {
        return  "星期三   ";
    }
    else if (x == "Thu") {
        return  "星期四   ";
    }
    else if (x == "Fri") {
        return  "星期五   ";
    }
    else {
        return  "星期六   ";
    }

}

void Widget::forecasts(QJsonObject x)
{
   //获取预报数据
    for(int i=1;i<=4;i++)
    {
        QJsonValue text=x.value("result")["forecasts"][i];
        tuBiao(text["text_day"].toString(),label_list1[i-1]);
        //更改星期
        QLabel *a=label_list3[i-1];
        a->setText(text["week"].toString());
        //更改温度区间
        QLabel *b=label_list2[i-1];
        b->setText(QString::number(text["low"].toInt())+"~"+QString::number(text["high"].toInt()));
    }
}


void Widget::on_pushButton_clicked()
{
    my_tianqi->souSuo(ui->lineEdit->text());
    ui->lineEdit->setText("");
}


void Widget::on_pushButton_2_clicked()
{
    chuLiShuJu(my_tianqi->get_tianQiShuJu());
}

