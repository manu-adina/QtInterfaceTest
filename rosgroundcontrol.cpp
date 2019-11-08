#include "rosgroundcontrol.h"
#include "ui_rosgroundcontrol.h"

ROSGroundControl::ROSGroundControl(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ROSGroundControl)
{
    ui->setupUi(this);

    /* Setting up sliders max, min and initial valie */
    ui->panSlider->setMinimum(0);
    ui->panSlider->setMaximum(1023);
    ui->panSlider->setValue(200);

    ui->tiltSlider->setMinimum(0);
    ui->tiltSlider->setMaximum(1023);
    ui->tiltSlider->setValue(200);

    ui->rollSlider->setMinimum(0);
    ui->rollSlider->setMaximum(1023);
    ui->rollSlider->setValue(200);
}

WId ROSGroundControl::OpenGLWidgetWId() {
    return ui->openGLWidget->winId();
}

ROSGroundControl::~ROSGroundControl()
{
    delete ui;
}

void ROSGroundControl::on_pushButton_clicked()
{
    ui->panSlider->setValue(200);
    ui->tiltSlider->setValue(200);
    ui->rollSlider->setValue(200);

    _mav_send_obj.SendGimbalCommands(STOP_GIMBAL_COMMAND, 1023 + static_cast<uint16_t>(ui->tiltSlider->value()),
                                     STOP_GIMBAL_COMMAND);
}

void ROSGroundControl::on_upCommand_pressed()
{
    QString num;
    num.setNum(1023 + ui->tiltSlider->value());
    QString outputText = "Up command: " + num;
    ui->outputTextBox->setPlainText(outputText);

    _mav_send_obj.SendGimbalCommands(STOP_GIMBAL_COMMAND, 1023 + static_cast<uint16_t>(ui->tiltSlider->value()),
                                     STOP_GIMBAL_COMMAND);
}

void ROSGroundControl::on_upCommand_released()
{
    QString num;
    num.setNum(1023);
    QString outputText = "Back to: " + num;
    ui->outputTextBox->setPlainText(outputText);

    _mav_send_obj.SendGimbalCommands(STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND);

}

void ROSGroundControl::on_leftCommand_pressed()
{
    QString num;
    num.setNum(1023 + ui->panSlider->value());
    QString outputText = "Left command: " + num;
    ui->outputTextBox->setPlainText(outputText);

    _mav_send_obj.SendGimbalCommands(1023 + static_cast<uint16_t>(ui->panSlider->value()),
                                     STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND);
}

void ROSGroundControl::on_leftCommand_released()
{
    QString num;
    num.setNum(1023);
    QString outputText = "Back to: " + num;
    ui->outputTextBox->setPlainText(outputText);

    _mav_send_obj.SendGimbalCommands(STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND);
}

void ROSGroundControl::on_rightCommand_pressed()
{
    QString num;
    num.setNum(1023 + ui->panSlider->value());
    QString outputText = "Right command: " + num;
    ui->outputTextBox->setPlainText(outputText);

    _mav_send_obj.SendGimbalCommands(1023 - static_cast<uint16_t>(ui->panSlider->value()),
                                     STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND);
}

void ROSGroundControl::on_rightCommand_released()
{
    QString num;
    num.setNum(1023);
    QString outputText = "Back to: " + num;
    ui->outputTextBox->setPlainText(outputText);

    _mav_send_obj.SendGimbalCommands(STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND);
}



void ROSGroundControl::on_downCommand_pressed()
{
    QString num;
    num.setNum(1023 + ui->tiltSlider->value());
    QString outputText = "Down command: " + num;
    ui->outputTextBox->setPlainText(outputText);

    _mav_send_obj.SendGimbalCommands(STOP_GIMBAL_COMMAND, 1023 - static_cast<uint16_t>(ui->tiltSlider->value()),
                                     STOP_GIMBAL_COMMAND);
}

void ROSGroundControl::on_downCommand_released()
{
    QString num;
    num.setNum(1023);
    QString outputText = "Back to: " + num;
    ui->outputTextBox->setPlainText(outputText);

    _mav_send_obj.SendGimbalCommands(STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND);
}
