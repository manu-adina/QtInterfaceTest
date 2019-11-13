#include "telemetryreceive.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <QThread>
#include <iostream>

TelemetryReceive::TelemetryReceive() : QObject()
{
    setUpUDPCommunication();
}

int TelemetryReceive::setUpUDPCommunication() {
    _sock = socket(AF_INET, SOCK_DGRAM, 0);

    _local_addr.sin_family = AF_INET;
    _local_addr.sin_addr.s_addr = INADDR_ANY;
    _local_addr.sin_port = htons(14551);

    if(bind(_sock, (struct sockaddr *)&_local_addr, sizeof(struct sockaddr)) < 0) {
        qCritical("Error Binding to the Socket");
        close(_sock);
        return -1;
    }

    if(fcntl(_sock, F_SETFL, O_NONBLOCK | O_ASYNC) < 0) {
        qCritical("Error Setting the Socket to Non-Blocking");
        close(_sock);
        return -1;
    }
    return 0;
}

void TelemetryReceive::processMavlinkMessage(mavlink_message_t msg) {
    switch(msg.msgid) {
    case MAVLINK_MSG_ID_LASER_MEASUREMENT:
        {
            mavlink_laser_measurement_t mav_laser_measurement_msg;
            mavlink_msg_laser_measurement_decode(&msg, &mav_laser_measurement_msg);
            float measurement = mav_laser_measurement_msg.rangefinder_reading;
            std::cout << "Received a message lol ?" << std::endl;
            emit receivedMeasurement(measurement);

        }
        break;
    }
}

void TelemetryReceive::run() {
    uint8_t buf[BUFFER_SIZE];
    ssize_t recsize;
    socklen_t fromlen = sizeof(_local_addr);
    emit receivedMeasurement(0.0);

    for(;;) {
        recsize = recvfrom(_sock, (void *)buf, BUFFER_SIZE, 0, (struct sockaddr *)&_local_addr, &fromlen);
        std:: cout << "Recsize " << recsize << std::endl;
        if(recsize > 0) {
            // Something is received.
            mavlink_message_t msg;
            mavlink_status_t status;

            for(int i = 0; i < recsize; i++) {
                if(mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status)) {
                    std::cout << "Got here" << std::endl;
                    processMavlinkMessage(msg);
                }
            }
        }
        usleep(20000);
    }
}

