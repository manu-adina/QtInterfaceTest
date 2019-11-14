#ifndef MAVCOMMANDS_H
#define MAVCOMMANDS_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <mavlink.h>

#define BUFFER_LENGTH 2041

class MavCommands
{
    public:
        MavCommands();
        void SendGimbalCommands(uint16_t pan_value, uint16_t tilt_value, uint16_t roll_value);
        void SendSensorCoordsRequest();

    private:
        int _sock;
        uint16_t _len;
        ssize_t _bytes_sent;
        struct sockaddr_in _ground_station_addr;
        mavlink_message_t _mav_msg;
        uint8_t _buf[BUFFER_LENGTH];
};

#endif // MAVCOMMANDS_H
