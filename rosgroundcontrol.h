#ifndef ROSGROUNDCONTROL_H
#define ROSGROUNDCONTROL_H

#include <QMainWindow>
#include <QPushButton>
#include "mavcommands.h"

#define STOP_GIMBAL_COMMAND 1023

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

    void on_upCommand_pressed();

    void on_upCommand_released();

    void on_leftCommand_pressed();

private:
    Ui::ROSGroundControl *ui;
    int _sock;
    MavCommands _mav_send_obj;
};

#endif // ROSGROUNDCONTROL_H
