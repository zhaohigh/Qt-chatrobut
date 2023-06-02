#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle("识别结果");
    this->setWindowIcon(QIcon(":/img/space.jpg"));
    ui->label_pixmap->setScaledContents(true);
    ui->textEdit->setPlainText("");
    Camera.setBtnStyle(ui->pushButton);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    this->close();
}

void Dialog::trueOrfalse(const QString data)
{
    if(data == "识别失败"){
        ui->label_pixmap->setPixmap(QPixmap(":/img/fault.jpg"));
        ui->textEdit->setText("识别失败");
        return;
    }else if(data == "识别成功"){
        ui->label_pixmap->setPixmap(QPixmap(":/img/right.jpg"));
        ui->textEdit->append("识别成功");
    }
}

void Dialog::setIoformation(const QString data)
{
    ui->textEdit->append(data);
}

void Dialog::startDialog()
{
    this->show();
}
