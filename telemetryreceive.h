#ifndef TELEMETRYRECEIVE_H
#define TELEMETRYRECEIVE_H

#include <QObject>
#include <netinet/in.h>
#include <mavlink.h>

#define BUFFER_SIZE 2041

class TelemetryReceive : public QObject
{
    Q_OBJECT
public:
    TelemetryReceive();
    int setUpUDPCommunication();
    void processMavlinkMessage(mavlink_message_t msg);

signals:
    void receivedMeasurement(float measurement);
    void receivedCoordinates(float *coordinates_x, float *coordinates_y);

public slots:
    void run();
    
private:
    int _sock;
    struct sockaddr_in _local_addr;
};

#endif // TELEMETRYRECEIVE_H
