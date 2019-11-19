#include "rosgroundcontrol.h"
#include "ui_rosgroundcontrol.h"
#include <QThread>
#include <Qt>
#include <QValueAxis>
#include "telemetryreceive.h"

ROSGroundControl::ROSGroundControl(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ROSGroundControl)
{
    ui->setupUi(this);

    QMainWindow::showFullScreen();

    QThread* telemetry_thread = new QThread;
    TelemetryReceive *telemetry_worker = new TelemetryReceive();
    telemetry_worker->moveToThread(telemetry_thread);

    connect(telemetry_thread, SIGNAL(started()), telemetry_worker, SLOT(run()));
    connect(telemetry_worker, SIGNAL(receivedMeasurement(float)), this, SLOT(receivedMeasurement(float)));
    connect(telemetry_worker, SIGNAL(receivedCoordinates(float*, float*)), this, SLOT(receivedCoordinates(float*, float*)));
    connect(telemetry_thread, SIGNAL(finished()), telemetry_thread, SLOT(deleteLater()));

    telemetry_thread->start();

    /* Setting up gimbal control button icons */
    QCommonStyle style;
    ui->upCommand->setIcon(style.standardIcon(QStyle::SP_ArrowUp));
    ui->downCommand->setIcon(style.standardIcon(QStyle::SP_ArrowDown));
    ui->leftCommand->setIcon(style.standardIcon(QStyle::SP_ArrowLeft));
    ui->rightCommand->setIcon(style.standardIcon(QStyle::SP_ArrowRight));

    /* Setting up button labels for the camera mode buttons */
    ui->cameraMode_1->setText(QString::number(camera_mode_1));
    ui->cameraMode_2->setText(QString::number(camera_mode_2));
    ui->cameraMode_3->setText(QString::number(camera_mode_3));

    chart = new QChart();
    chartView = new QChartView();
    series = new QScatterSeries();

    chart->legend()->hide();
    //chart->addSeries(series);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(640, 480);

    chart->setTitle("Sensor Scatter Plot");

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Wavelength");
    axisX->setRange(0,10);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Duration");
    axisY->setRange(0,10);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setBackgroundRoundness(0);

    this->layout()->addWidget(chartView);
    chartView->move(0, 0);
    ui->openGLWidget->move(0, 0);
    chartView->hide();
}

WId ROSGroundControl::OpenGLWidgetWId() {
    return ui->openGLWidget->winId();
}

ROSGroundControl::~ROSGroundControl()
{
    delete ui;
}

void ROSGroundControl::on_upCommand_pressed()
{
    _mav_send_obj.SendGimbalCommands(STOP_GIMBAL_COMMAND, 1023 + SPEED_OFFSET,
                                     STOP_GIMBAL_COMMAND);
}

void ROSGroundControl::on_upCommand_released()
{
    _mav_send_obj.SendGimbalCommands(STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND);

}

void ROSGroundControl::on_leftCommand_pressed()
{
    _mav_send_obj.SendGimbalCommands(1023 + SPEED_OFFSET,
                                     STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND);
}

void ROSGroundControl::on_leftCommand_released()
{
    _mav_send_obj.SendGimbalCommands(STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND);
}

void ROSGroundControl::on_rightCommand_pressed()
{
    _mav_send_obj.SendGimbalCommands(1023 - SPEED_OFFSET,
                                     STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND);
}

void ROSGroundControl::on_rightCommand_released()
{
    _mav_send_obj.SendGimbalCommands(STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND);
}



void ROSGroundControl::on_downCommand_pressed()
{
    _mav_send_obj.SendGimbalCommands(STOP_GIMBAL_COMMAND, 1023 - SPEED_OFFSET,
                                     STOP_GIMBAL_COMMAND);
}

void ROSGroundControl::on_downCommand_released()
{
    _mav_send_obj.SendGimbalCommands(STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND, STOP_GIMBAL_COMMAND);
}

void ROSGroundControl::receivedMeasurement(float measurement) {
    QString measurementStr = QString::number(measurement);
    ui->laserMeasurement->setText("Dist: " + measurementStr);
}

void ROSGroundControl::receivedCoordinates(float *coordinates_x, float *coordinates_y) {

    series->clear();

    for(int i = 0; i < 10; i++) {
        series->append(coordinates_x[i], coordinates_y[i]);
    }

    chart->addSeries(series);


    chartView->setChart(chart);
    chartView->repaint();
    chartView->show();
}

void ROSGroundControl::on_sensorTrigger_clicked()
{
    _mav_send_obj.SendSensorCoordsRequest();
}

void ROSGroundControl::on_chartCloseButton_clicked()
{
    chartView->hide();
}

void ROSGroundControl::on_quitButton_clicked()
{
    QApplication::quit();
}

void ROSGroundControl::on_cameraMode_1_clicked()
{
    camera_mode_1 = (camera_mode_1 % 3) + 1;
    _mav_send_obj.SendCameraCommands(camera_mode_1, camera_mode_2, camera_mode_3);

    ui->cameraMode_1->setText(QString::number(camera_mode_1));
}

void ROSGroundControl::on_cameraMode_2_clicked()
{
    camera_mode_2 = (camera_mode_2 % 3) + 1;
    _mav_send_obj.SendCameraCommands(camera_mode_2, camera_mode_2, camera_mode_3);

    ui->cameraMode_2->setText(QString::number(camera_mode_2));
}

void ROSGroundControl::on_cameraMode_3_clicked()
{
    camera_mode_3 = (camera_mode_3 % 3) + 1;
    _mav_send_obj.SendCameraCommands(camera_mode_2, camera_mode_2, camera_mode_3);

    ui->cameraMode_3->setText(QString::number(camera_mode_3));
}
