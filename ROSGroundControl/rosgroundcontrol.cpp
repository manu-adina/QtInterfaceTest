#include "rosgroundcontrol.h"
#include "ui_rosgroundcontrol.h"

ROSGroundControl::ROSGroundControl(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ROSGroundControl)
{
    ui->setupUi(this);
}

ROSGroundControl::~ROSGroundControl()
{
    delete ui;
}

void ROSGroundControl::on_pushButton_clicked()
{

}

void ROSGroundControl::on_panSlider_sliderReleased()
{

}
