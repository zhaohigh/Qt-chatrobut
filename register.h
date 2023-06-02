#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include<QNetworkAccessManager> //网络请求和应答对象
#include<QNetworkReply>         //
#include<QNetworkRequest>       //
#include<QJsonObject>           //json格式
#include<QJsonDocument>
#include<QJsonValue>
#include<QJsonArray>
#include"camera.h"
#include<QImage>
#include<QPixmap>
#include<QBuffer>
#include<QMessageBox>
#include<QFocusEvent>
namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QString access_token, QWidget* parent = nullptr);
    ~Register();
signals:
    void backRegister();

private slots:
    void on_screenShortBtn_clicked();

    QString ImageToString(const QImage &);

    void on_screenShortBtn_2_clicked();

    void on_loginBtn_clicked();

    void RebackFacemessge(QNetworkReply *reply);

    void on_rebackBtn_clicked();

private:
    Ui::Register *ui;
    Camera camera;
    QImage image;
    QString imastring;
    QNetworkAccessManager *http;

    QString baiduFaceLogin = "https://aip.baidubce.com/rest/2.0/face/v3/faceset/user/add";
    QString token;
};

#endif // REGISTER_H
