#include "rosgroundcontrol.h"
#include "ui_rosgroundcontrol.h"
#include <QThread>
//include <QtCharts>
#include "telemetryreceive.h"

ROSGroundControl::ROSGroundControl(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ROSGroundControl)
{
    ui->setupUi(this);

    QThread* telemetry_thread = new QThread;
    TelemetryReceive *telemetry_worker = new TelemetryReceive();
    telemetry_worker->moveToThread(telemetry_thread);

    connect(telemetry_thread, SIGNAL(started()), telemetry_worker, SLOT(run()));
    connect(telemetry_worker, SIGNAL(receivedMeasurement(float)), this, SLOT(receivedMeasurement(float)));
    connect(telemetry_worker, SIGNAL(receivedCoordinates(float*, float*)), this, SLOT(receivedCoordinates(float*, float*)));
    connect(telemetry_thread, SIGNAL(finished()), telemetry_thread, SLOT(deleteLater()));

    telemetry_thread->start();

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


    series = new QScatterSeries();
    series->setName("Sensor Data");
    series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series->setMarkerSize(5.0);

    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Sensor Scatter Plot");
    chart->createDefaultAxes();
    chart->axes(Qt::Vertical).first()->setRange(0, 10);
    chart->axes(Qt::Horizontal).first()->setRange(0, 10);
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(331, 281);
    chartView->move(665, 360);
    this->layout()->addWidget(chartView);
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

void ROSGroundControl::receivedMeasurement(float measurement) {
    QString measurementStr = QString::number(measurement);
    ui->laserMeasurement->setText("Measurement: " + measurementStr);
}

void ROSGroundControl::receivedCoordinates(float *coordinates_x, float *coordinates_y) {

    chart->removeAllSeries();
    QScatterSeries *newSeries = new QScatterSeries();
    newSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    newSeries->setMarkerSize(10.0);

    for(int i = 0; i < 10; i++) {
        newSeries->append(coordinates_x[i], coordinates_y[i]);
    }

    chart->addSeries(newSeries);
    chartView->setChart(chart);
    chartView->repaint();
}

void ROSGroundControl::on_sensorTrigger_clicked()
{
    _mav_send_obj.SendSensorCoordsRequest();
}
