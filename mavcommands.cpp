#include "mavcommands.h"

#include <QtGlobal>
#include <QApplication>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <iostream>

MavCommands::MavCommands()
{
    /* Set up the socket for sending commansd */
    std::string payload_address = "192.168.0.244";

    if((_sock = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        qCritical("Socket Failed (%d): %s", errno, strerror(errno));
        close(_sock);
        QApplication::quit();
    }

    _ground_station_addr.sin_family = AF_INET;
    _ground_station_addr.sin_addr.s_addr = inet_addr(payload_address.c_str());
    _ground_station_addr.sin_port = 14550;

    /* Make the socket non-blocking */
    if(fcntl(_sock, F_SETFL, O_NONBLOCK | O_ASYNC) < 0) {
        qCritical("Failed to set the sock to non-blocking");
        QApplication::quit();
    }
}

void MavCommands::SendGimbalCommands(uint16_t pan_value, uint16_t tilt_value, uint16_t roll_value){

    mavlink_message_t msg;
    uint8_t buf[BUFFER_LENGTH];

//    mavlink_msg_camera_mode_pack(1, 200, &msg, camera_mode);
//    len = mavlink_msg_to_send_buffer(buf, &msg);
//    bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr *)&ground_station_addr, sizeof(struct sockaddr_in));

    /* Send roll, pitch, pan values to the gimbal */
    mavlink_msg_gimbal_commands_pack(1, 200, &msg, pan_value, tilt_value, roll_value);
    _len = mavlink_msg_to_send_buffer(buf, &msg);
    _bytes_sent = sendto(_sock, buf, _len, 0, (struct sockaddr *)&_ground_station_addr, sizeof(struct sockaddr_in));
    std::cout << "Bytes Sent: " << _bytes_sent  << std::endl;
}
