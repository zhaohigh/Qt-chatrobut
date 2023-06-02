#include "register.h"
#include "ui_register.h"
#include<QDebug>
Register::Register(QString access_token, QWidget* parent ) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    this->setWindowTitle("添加人脸识别");
    this->setWindowIcon(QIcon(":/img/space.jpg"));
    token = access_token;
    //创建设置摄像头
    camera.setLabel(ui->label);
    camera.startCamera();
    //设置按钮样式
    camera.setBtnStyle(ui->screenShortBtn);
    camera.setBtnStyle(ui->screenShortBtn_2);
    camera.setBtnStyle(ui->loginBtn);
    camera.setBtnStyle(ui->rebackBtn);

}

Register::~Register()
{
    delete ui;
}

/*截图*/
void Register::on_screenShortBtn_clicked()
{
    //清除image
    image.fill(Qt::white);
    image = camera.screenShort();
    //iamge转换成string类型
    imastring= ImageToString(image);
    //    qDebug()<<"ima"<<imastring;
    camera.stopCamera();
    ui->label->clear();
    //上传的截图放在label标签上
    QPixmap pixmap = QPixmap::fromImage(image);
    ui->label->setPixmap(pixmap);
    ui->label->setScaledContents(true);
}

QString Register::ImageToString(const QImage &)
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
/*取消截图*/
void Register::on_screenShortBtn_2_clicked()
{
    ui->label->clear();
    camera.startCamera();
}


void Register::on_loginBtn_clicked()
{
    //判断用户名和工号是否为空
    if(ui->lineEdit_name->text().isEmpty() || ui->lineEdit_port->text().isEmpty()){
        QMessageBox::warning(this,"警告","请输入姓名或账号","确认");
        return;
    }
    //判断用户名是否字母
    for(const QChar&c:ui->lineEdit_name->text()){
        if(!c.isLetter()){
            QMessageBox::warning(this,"非法操作","用户名格式不正确，请重新输入","确认");
            return;
        }
    }
    //判断工号是否是数字
    bool isNumber=false;
    ui->lineEdit_port->text().toInt(&isNumber);
    if(isNumber == false){
        QMessageBox::warning(this,"非法操作","工号格式不正确，请重新输入","确认");
        return;
    }

    http = new QNetworkAccessManager;
    //百度人脸识别注册url
    QString url = "https://aip.baidubce.com/rest/2.0/face/v3/faceset/user/add?access_token=";
    QNetworkRequest request;
    request.setUrl(QUrl(url+token));
    request.setRawHeader("Content-Type", "application/json");
    QJsonObject json;
    json["image"]=imastring;
    json["image_type"]="BASE64";
    json["group_id"]="zxn_1311740678";
    json["user_id"]=QString("%1_%2").arg(ui->lineEdit_name->text()).arg(ui->lineEdit_port->text().toInt());
    json["quality_control"]="NORMAL";
    QByteArray data = QJsonDocument(json).toJson();
    connect(http,&QNetworkAccessManager::finished,this,&Register::RebackFacemessge);
    http->post(request,data);

}
/*注册获取json信息*/
void Register::RebackFacemessge(QNetworkReply *reply)
{
    ui->lineEdit_name->clear();
    ui->lineEdit_port->clear();

    QByteArray byte = reply->readAll();
    QByteArray bytearry = QString(byte).toUtf8();
    QJsonDocument docunment = QJsonDocument::fromJson(bytearry);
    if(docunment.isObject()){
        QJsonObject json = docunment.object();
        qDebug()<<"json"<<json;
        if(json.contains("error_msg")){
            QString data = json.value("error_msg").toString();
            if(data == "SUCCESS"){
                QMessageBox::information(this,"注册","恭喜！人脸库添加成功");
                return;
            }else if(data == "param[user_id] format error"){
                QMessageBox::warning(this,"警告","用户名或账号格式不正确","确认");
                return;
            }else if(data == "param[quality_control]format error"){
                QMessageBox::critical(this,"错误","人脸质量过低，请重新注册");
                return;
            }else if(data == "add face fail"){
                QMessageBox::critical(this,"错误","人脸图片添加失败，请重新注册");
                return;
            }else if(data == "user is already exist"){
                QMessageBox::critical(this,"错误","用户已存在，请不要重复注册");
                return;
            }else {
                QMessageBox::critical(this,"错误","人脸库注册失败，请重新注册");
                return;
            }
        }
    }
}

void Register::on_rebackBtn_clicked()
{
    camera.stopCamera();
    this->hide();
    emit backRegister();
}
