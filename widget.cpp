#include "widget.h"
#include "ui_widget.h"
#include<QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("人脸识别考勤");
    this->setWindowIcon(QIcon(":/img/space.jpg"));
    ui->label->setFixedSize(ui->widget->size());
    ui->textEdit->insertPlainText("");
    //设置按钮样式
   camera.setBtnStyle(ui->faceBtn);
   camera.setBtnStyle(ui->addUserBtn);
   camera.setBtnStyle(ui->quitBtn);

    //创建设置摄像头
    camera.setLabel(ui->label);
    camera.startCamera();

    //    /*创建新的文件夹保存截图*/
    //    // 获取当前执行文件路径
    //    QString projectPath = QCoreApplication::applicationDirPath();
    //    // 创建文件夹，在当前可执行文件目录中
    //    QString folderPath = projectPath + "/Screenshots";
    //    if (!QDir(folderPath).exists()) {
    //        if (QDir().mkpath(folderPath)) {
    //            qDebug() << "文件夹创建成功：" << folderPath;
    //        } else {
    //            qDebug() << "文件夹创建失败：" << folderPath;
    //        }
    //    } else {
    //        qDebug() << "文件夹已存在：" << folderPath;
    //    }
    //    // 创建截图定时器，每隔4秒触发一次
    //    timer.setInterval(4000);
    //    // 自定义截图保存路径
    //     savePath = folderPath;
    //    connect(&timer,&QTimer::timeout,this,&Widget::ScreenShort);
    //    timer.start();



    //获取百度人脸识别access_token
    manager = new QNetworkAccessManager;
    QByteArray url =QString("client_id=%1&client_secret=%2&grant_type=client_credentials")
            .arg(client_id)
            .arg(client_secret).toLatin1();
    QNetworkRequest request;
    request.setUrl(QUrl(baiduUrl+url));
    connect(manager,&QNetworkAccessManager::finished,this,&Widget::Accesstokenreply);
    manager->get(request);


}

Widget::~Widget()
{
    delete ui;
}
/*获取access_token*/
void Widget::Accesstokenreply(QNetworkReply *reply)
{
    QByteArray byte = reply->readAll();
    QByteArray bytearry= QString(byte).toUtf8();
    QJsonDocument document= QJsonDocument::fromJson(bytearry);
    if(document.isObject()){
        QJsonObject json = document.object();
        if(json.contains("access_token")){
            QJsonValue jsonval = json.value("access_token");
            access_token = jsonval.toString();
            //             qDebug()<<access_token;
        }
    }
}

void Widget::Baiduface()
{
    //发送图片给百度人脸识别，获取json信息
    http = new QNetworkAccessManager;
    QString Url= "https://aip.baidubce.com/rest/2.0/face/v3/detect?access_token=";
    QNetworkRequest Request;
    //    qDebug()<<access_token;
    Request.setUrl(QUrl(Url+access_token));
    //设置头部
    Request.setRawHeader("Content-Type", "application/json");
    //请求body参数
    QJsonObject json;
    json["image"]=ima;
    json["image_type"]="BASE64";
    json["face_field"]="age,beauty,expression,face_shape,gender,glasses,emotion";
    json["max_face_num"]=2;
    json["face_type"]="LIVE";
    QByteArray data = QJsonDocument(json).toJson();
    connect(http,&QNetworkAccessManager::finished,this,&Widget::RebackFacemessge);
    http->post(Request,data);

    //百度人脸库内检测，并返回结果
    faceSearch = new QNetworkAccessManager;
    QString url = "https://aip.baidubce.com/rest/2.0/face/v3/search?access_token=";
    QNetworkRequest request;
    request.setUrl(QUrl(url+access_token));
    //设置头部
    request.setRawHeader("Content-Type", "application/json");
    //请求body参数
    QJsonObject json2;
    json2["image"] = ima;
    json2["image_type"] = "BASE64";
    json2["group_id_list"] = "zxn_1311740678";
    json2["quality_control"] = "NORMAL";
    json2["match_threshold"] = 80;
    QByteArray data2 = QJsonDocument(json2).toJson();
    connect(faceSearch,&QNetworkAccessManager::finished,this,&Widget::RebackFaceSearch);
    faceSearch->post(request,data2);
}

QString Widget::ImageToString(const QImage &image)
{
    // 将 QImage 转换为 QString
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");  // 保存为 PNG 格式的字节数组
    QString imageString = QString(byteArray.toBase64());
    qDebug()<<"imagestring:"<<imageString;
    buffer.close();
    return imageString;
}

/*获取百度人脸检测结果*/
void Widget::RebackFacemessge(QNetworkReply *reply)
{
    QByteArray byte = reply->readAll();
    QByteArray bytearry = QString(byte).toUtf8();
    QJsonDocument docunment = QJsonDocument::fromJson(bytearry);
    if(docunment.isObject()){
        QJsonObject json = docunment.object();
        //        qDebug()<<"json:"<<json;
        if(json.contains("result")){
            QJsonObject obj = json.take("result").toObject();
            if(obj.contains("face_list")){
                //                qDebug()<<"obj:"<<obj;
                QJsonArray array = obj.value("face_list").toArray();
                //                qDebug()<<"array"<<array;
                for(int i=0;i<array.size();i++){
                    QJsonObject obj1 = array.at(i).toObject();
                    if(obj1.contains("age")){
                        double age = obj1.value("age").toDouble();
                        ui->textEdit->append(QString("年龄：%1").arg(age));
                    }
                    if(obj1.contains("gender")){
                        QJsonObject obj2 = obj1.value("gender").toObject();
                        if(obj2.contains("type")){
                            QString type = obj2.value("type").toString();
                            if(type == "male"){
                                ui->textEdit->append("性别：男");
                            }else{
                                ui->textEdit->append("性别：女");
                            }
                        }
                    }
                    if(obj1.contains("beauty")){
                        double beauty = obj1.value("beauty").toDouble();
                        ui->textEdit->append(QString("颜值得分：%1").arg(beauty));
                    }
                    if(obj1.contains("emotion")){
                        QJsonObject obj2 = obj1.value("emotion").toObject();
                        if(obj2.contains("type")){
                            QString type = obj2.value("type").toString();
                            if(type == "sad"){
                                ui->textEdit->append("情绪：伤心");
                            }else if(type == "happy"){
                                ui->textEdit->append("情绪：高兴");
                            }else if(type == "neutral"){
                                ui->textEdit->append("情绪：不带感情的");
                            }else if(type == "angry"){
                                ui->textEdit->append("情绪：愤怒");
                            }else if(type == "disgust"){
                                ui->textEdit->append("情绪：厌恶");
                            }else if(type == "fear"){
                                ui->textEdit->append("情绪：恐惧");
                            }else if(type == "surprise"){
                                ui->textEdit->append("情绪：惊讶");
                            }else if(type == "pouty"){
                                ui->textEdit->append("情绪：撅嘴");
                            }else if(type == "grimace"){
                                ui->textEdit->append("情绪：鬼脸");
                            }
                        }
                    }
                    if(obj1.contains("glasses")){
                        QJsonObject obj2 = obj1.value("glasses").toObject();
                        if(obj2.contains("type")){
                            QString type = obj2.value("type").toString();
                            if(type == "none"){
                                ui->textEdit->append("没戴眼镜");
                            }else {
                                ui->textEdit->append("戴了眼镜");
                            }
                        }
                    }
                    if(obj1.contains("expression")){
                        QJsonObject obj2 = obj1.value("expression").toObject();
                        if(obj2.contains("type")){
                            QString type = obj2.value("type").toString();
                            if(type == "none"){
                                ui->textEdit->append("表情：无表情");
                            }else if(type == "smile"){
                                ui->textEdit->append("表情：微笑");
                            }else if(type == "laugh"){
                                ui->textEdit->append("表情：大笑");
                            }
                        }
                    }
                    if(obj1.contains("face_shape")){
                        QJsonObject obj2 = obj1.value("face_shape").toObject();
                        if(obj2.contains("type")){
                            QString type = obj2.value("type").toString();
                            if(type == "oval"){
                                ui->textEdit->append("脸型：椭圆脸");
                            }else if(type == "square"){
                                ui->textEdit->append("脸型：方形脸");
                            }else if(type == "triangle"){
                                ui->textEdit->append("脸型：尖脸");
                            }else if(type == "heart"){
                                ui->textEdit->append("脸型：鹅蛋脸");
                            }else if(type == "round"){
                                ui->textEdit->append("脸型：圆脸");
                            }
                        }
                    }
                }
            }
        }
    }
}
/*获取百度人脸库检索结果*/
void Widget::RebackFaceSearch(QNetworkReply *reply)
{
    QByteArray byte = reply->readAll();
    QByteArray bytearry = QString(byte).toUtf8();
    QJsonDocument docunment = QJsonDocument::fromJson(bytearry);
    if(docunment.isObject()){
        QJsonObject json = docunment.object();
        qDebug()<<"json:"<<json;
        if(json.contains("error_msg")){
            QString data = json.value("error_msg").toString();
            if(data == "Open api daily request limit reached"){
                QMessageBox::critical(this,"错误","今日识别次数已用完","确认");
                flag=false;
            }else if(data =="Open api request limit reached"){
                QMessageBox::critical(this,"错误","请求速率过快，请重新识别","确认");
                flag=false;
            }/*else if(data =="Open api qps request limit reached"){
                QMessageBox::critical(this,"错误","请求速率过快，请重新识别","确认");
                 flag=false;
            }*/else if(data =="Open api total request limit reached"){
                QMessageBox::critical(this,"错误","免费次数已用完，请付费开通","确认");
                flag=false;
            }else if(data =="Invalid parameter"){
                QMessageBox::critical(this,"错误","无效接口","确认");
                flag=false;
            }else if(data =="match user is not found"){
                QMessageBox::critical(this,"错误","用户未注册，请注册后识别","确认");
                flag=false;
            }
        }
        if(flag == false)
            return;
        if(json.contains("result")){
            QJsonObject obj = json.value("result").toObject();
            if(obj.contains("user_list")){
                QJsonArray array = obj.value("user_list").toArray();
                for(int i=0;i<array.size();i++){
                    QJsonObject obj2 = array.at(i).toObject();
                    if(obj2.contains("score")){
                        double score = obj2.value("score").toDouble();
                        if(score<80){
                            qDebug()<<"识别失败";
                            dialog.trueOrfalse("识别失败");
                            return;
                        }else {
                            qDebug()<<"识别成功";
                            dialog.trueOrfalse("识别成功");
                        }
                    }
                    if(obj2.contains("user_id")){
                        QString user = obj2.value("user_id").toString();
                        QStringList parts = user.split("_");
                        qDebug()<<"用户名："<<parts.at(0);
                        qDebug()<<"工号："<<parts.at(1);
                        dialog.setIoformation(QString("用户名：%1").arg(parts.at(0)));
                        dialog.setIoformation(QString("工号：%1").arg(parts.at(1)));
                        dialog.show();
                    }
                }
            }
        }
    }
}

/*截图*/
void Widget::on_faceBtn_clicked()
{
    //获取当前时间作为文件名
    //         fileName = savePath +"/"+ QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".png";
    //         qDebug()<<"保存截图"<<fileName;
    //清除image
    image.fill(Qt::white);
    // 调用函数获取摄像头当前帧
    image = camera.screenShort();
    //iamge转换成string类型
    ima = ImageToString(image);
    //          qDebug()<<"ima"<<ima;
    // 保存当前帧为图片文件
    //          if(image.save(fileName)){
    //              qDebug()<<"成功";
    ui->label_image->clear();
    //上传的截图放在label标签上
    QPixmap pixmap = QPixmap::fromImage(image);
    ui->label_image->setPixmap(pixmap);
    ui->label_image->setScaledContents(true);

    //          }else{
    //              qDebug()<<"失败"<< image.save(fileName, "PNG") << image.save(fileName, "JPEG");
    //          }
    ui->textEdit->clear();
    connect(this,&Widget::AccessTokenReady,this,&Widget::Baiduface);
    emit AccessTokenReady();

}

/*退出*/
void Widget::on_quitBtn_clicked()
{
    this->close();
}

void Widget::on_addUserBtn_clicked()
{
    camera.stopCamera();
    //创建第二界面
    fame = new Register(access_token);
    connect(fame,&Register::backRegister,this,&Widget::backRegister);
    this->hide();
    fame->show();
}

void Widget::backRegister()
{
    disconnect(fame,&Register::backRegister,this,&Widget::backRegister);
    //    fame->hide();
    this->show();
    camera.startCamera();
}
