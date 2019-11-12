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

public slots:
    void run();
    
private:
    int _sock;
    struct sockaddr_in _local_addr;
};

#endif // TELEMETRYRECEIVE_H
