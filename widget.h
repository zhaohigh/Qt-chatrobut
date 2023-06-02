#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>
#include<QApplication>
#include<QPushButton>
#include<QNetworkAccessManager> //网络请求和应答对象
#include<QNetworkReply>         //
#include<QNetworkRequest>       //
#include<QJsonObject>           //json格式
#include<QJsonDocument>
#include<QJsonValue>
#include<QJsonArray>
#include<QTimer>            //计时器
#include<QDateTime>
#include<QDir>              //目录结构
#include<QBuffer>
#include<QString>
#include"register.h"
#include"camera.h"
#include"dialog.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:

    void Accesstokenreply(QNetworkReply *reply);

    void Baiduface();

    QString ImageToString(const QImage &);

    void RebackFacemessge(QNetworkReply *reply);

    void RebackFaceSearch(QNetworkReply *reply);

    void on_faceBtn_clicked();

    void on_quitBtn_clicked();

    void on_addUserBtn_clicked();

    void backRegister();

signals:
    void AccessTokenReady();


private:
    Ui::Widget *ui;
    QNetworkAccessManager *manager,*http,*faceSearch;
    QTimer timer;
    Register *fame;
    Camera camera;
    Dialog dialog;

    QString baiduUrl="https://aip.baidubce.com/oauth/2.0/token?";
    QString client_id="y0kaHZy6G5NjhZKXuldgVPDo";
    QString client_secret="OAtyLDxZLl8Lv8KDPFpXkyRuVigTSPUa";
    QString access_token="";
    QString ima;
    QString fileName;
    QString savePath;
    QImage image;
    bool flag = true;
};

#endif // WIDGET_H
