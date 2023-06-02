#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QStringList>
#include<QPixmap>
#include"camera.h"
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
public:
    void trueOrfalse(const QString);
    void setIoformation(const QString);
    void startDialog();
private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
    Camera Camera;
};

#endif // DIALOG_H
