#ifndef ROSGROUNDCONTROL_H
#define ROSGROUNDCONTROL_H

#include <QMainWindow>

namespace Ui {
class ROSGroundControl;
}

class ROSGroundControl : public QMainWindow
{
    Q_OBJECT

public:
    explicit ROSGroundControl(QWidget *parent = nullptr);
    ~ROSGroundControl();

private slots:
    void on_pushButton_clicked();

    void on_panSlider_sliderReleased();

private:
    Ui::ROSGroundControl *ui;
};

#endif // ROSGROUNDCONTROL_H
