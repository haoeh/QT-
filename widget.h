#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QLabel>
#include "tianqi.h"
#include "mytiem.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void time(QStringList x);
    void chuLiShuJu(QJsonObject x);//处理天气数据
    void tuBiao(QString x,QLabel *y);//图标的变化
    void beiJin(QString x);//背景图的变化
    QString xingqi(QString x);
    void forecasts(QJsonObject x);//处理后几天的天气情况
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
    tianqi *my_tianqi;
    QList<QLabel*> label_list1;
    QList<QLabel*> label_list2;
    QList<QLabel*> label_list3;

    QTimer *ti;
    mytiem *myt;
};
#endif // WIDGET_H
