#ifndef CAMERA_H
#define CAMERA_H

#include <QWidget>
#include<QCamera>               //摄像头信息
#include<QCameraImageCapture>   //图片捕获
#include<QCameraViewfinder>     //摄像头对象
#include<QLabel>
#include<QImage>
#include<QMessageBox>
#include<QPushButton>
class Camera : public QWidget
{
    Q_OBJECT
public:
    explicit Camera(QWidget *parent = nullptr);

    void setLabel(QLabel*label);

    void startCamera();

    void stopCamera();

    QImage screenShort();

    void setBtnStyle(QPushButton *);

private:
    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QCameraImageCapture *imagecapture;
    QLabel *label;

};

#endif // CAMERA_H
