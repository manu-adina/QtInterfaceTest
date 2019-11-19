#include "mavcommands.h"

#include <QtGlobal>
#include <QApplication>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>

MavCommands::MavCommands()
{
    /* Set up the socket for sending commansd */
    char target_ip[] = "192.168.0.245";

    if((_sock = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        qCritical("Socket Failed (%d): %s", errno, strerror(errno));
        close(_sock);
        QApplication::quit();
    }

    _ground_station_addr.sin_family = AF_INET;
    _ground_station_addr.sin_addr.s_addr = inet_addr(target_ip);
    _ground_station_addr.sin_port = htons(14550);

    /* Make the socket non-blocking */
    if(fcntl(_sock, F_SETFL, O_NONBLOCK | O_ASYNC) < 0) {
        qCritical("Failed to set the sock to non-blocking");
        close(_sock);
        QApplication::quit();
    }
}

/* Send roll, pitch, pan values to the gimbal */
void MavCommands::SendGimbalCommands(uint16_t pan_value, uint16_t tilt_value, uint16_t roll_value){
    mavlink_msg_gimbal_commands_pack(1, 200, &_mav_msg, pan_value, tilt_value, roll_value);
    _len = mavlink_msg_to_send_buffer(_buf, &_mav_msg);
    _bytes_sent = sendto(_sock, _buf, _len, 0, (struct sockaddr *)&_ground_station_addr, sizeof(struct sockaddr_in));
}

/* Send camera mode 1, 2, and 3 to the gimbal. */
void MavCommands::SendCameraCommands(uint8_t mode_1, uint8_t mode_2, uint8_t mode_3) {
    mavlink_msg_camera_commands_pack(1, 200, &_mav_msg, mode_1, mode_2, mode_3);
    _len = mavlink_msg_to_send_buffer(_buf, &_mav_msg);
    _bytes_sent = sendto(_sock, _buf, _len, 0, (struct sockaddr *)&_ground_station_addr, sizeof(struct sockaddr_in));
}

/* Send request to receive sensor coordinates */
void MavCommands::SendSensorCoordsRequest() {
    mavlink_msg_sensor_coords_request_pack(1, 200, &_mav_msg, 1);
    _len = mavlink_msg_to_send_buffer(_buf, &_mav_msg);
    _bytes_sent = sendto(_sock, _buf, _len, 0, (struct sockaddr *)&_ground_station_addr, sizeof(struct sockaddr_in));
}
