#include "camera.h"

Camera::Camera(QWidget *parent) : QWidget(parent)
{
        //创建摄像头对象
       camera = new QCamera(this);
       //创建摄像头视图对象（父对象为Widget容器）
       viewfinder = new QCameraViewfinder(this);
        //设置图像捕获内容
       imagecapture = new QCameraImageCapture(camera);
}

void Camera::setLabel(QLabel *label)
{
    //设置于与label标签一样大
    viewfinder->resize(label->size());
    //设置摄像头内容到label控件
    viewfinder->setParent(label);
     //自动填充背景框(警告：背景框不能有样式表内容，否则将失效)
//    viewfinder->autoFillBackground();
    viewfinder->show();
}

void Camera::startCamera()
{
     //设置摄像头试图对象到摄像头
    camera->setViewfinder(viewfinder);
    camera->setCaptureMode(QCamera::CaptureStillImage);
    imagecapture->capture();
    camera->start();
}

void Camera::stopCamera()
{
    camera->stop();
//    camera->setViewfinder(nullptr);
}

QImage Camera::screenShort()
{
    // 获取摄像头当前帧
    QImage image = viewfinder->grab().toImage();
    if(image.isNull()){
        QMessageBox::warning(this,"警告","请重新识别或截图","确认");
    }
    return image;
}

void Camera::setBtnStyle(QPushButton *pushbutton)
{
    pushbutton->setStyleSheet(//鼠标正常样式
                              "QPushButton{"
                              "backgroud-color:#8221e9;"//按钮背景颜色
                              "border-style:outset;"      //边框样式
                              "border-width:6px;"         //边框样式
                              "border-radius:20px;"     //边框圆角
                              "border-color:#3aef61;" //边框颜色
                              "color:white;"
                              "}"
                              //鼠标悬停样式
                              "QPushButton:hover{"
                              "backgroud-color:#21e2e9;"
                              "border-color:#8221e9;"
                              "color:black;"
                              "}"
                              //鼠标点击样式
                              "QPushButton:pressed{"
                              "backgroud-color:#1175ef;"
                              "border-color:#B362ca;"
                              "border-style:inset;"
                              "color:#3aef61;"
                              "}");

}
