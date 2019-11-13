#ifndef ROSGROUNDCONTROL_H
#define ROSGROUNDCONTROL_H

#include <QMainWindow>
#include <QPushButton>
#include "mavcommands.h"

#include <gst/gst.h>
#include <gst/video/videooverlay.h> // For GST_VIDEO_OVERLAY variable.

#define STOP_GIMBAL_COMMAND 1023

namespace Ui {
class ROSGroundControl;
}

class ROSGroundControl : public QMainWindow
{
    Q_OBJECT

public:
    explicit ROSGroundControl(QWidget *parent = nullptr);
    WId OpenGLWidgetWId();
    ~ROSGroundControl();

private slots:
    void on_pushButton_clicked();

    void on_upCommand_pressed();

    void on_upCommand_released();

    void on_leftCommand_pressed();

    void on_leftCommand_released();

    void on_rightCommand_pressed();

    void on_rightCommand_released();

    void on_downCommand_pressed();

    void on_downCommand_released();

    void receivedMeasurement(float measurement);
    void receivedCoordinates(float *coordinates_x, float *coordinates_y);

private:
    Ui::ROSGroundControl *ui;
    int _sock;
    MavCommands _mav_send_obj;
};

#endif // ROSGROUNDCONTROL_H
