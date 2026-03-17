#include "networkcode.h"

NetworkInterface *_defaultSystemNetwork;

bool connect()
{
    _defaultSystemNetwork = NetworkInterface::get_default_instance();
    if (_defaultSystemNetwork == nullptr) {
        printf("No network interface found\n\r");
        return false;
    }

    _defaultSystemNetwork->set_network("192.168.137.2","255.255.255.0","192.168.137.1");
    int ret = _defaultSystemNetwork->connect();
    if (ret != 0) {
        printf("Connection error: %d\n\r", ret);
        return false;
    }
    printf("Connection success, MAC: %s\n\r", _defaultSystemNetwork->get_mac_address());

    SocketAddress a;
    _defaultSystemNetwork->get_ip_address(&a);
    printf("IP address: %s\n\r", a.get_ip_address() ? a.get_ip_address() : "None");

    return true;
}

bool setTime()
{
    puts("Getting time from the NTP server\n\r");

    NTPClient ntp(_defaultSystemNetwork);
    ntp.set_server("time.google.com", 123);
    time_t timestamp = ntp.get_timestamp();
    if (timestamp < 0) {
        printf("Failed to get the current time, error: %ud\n\r", timestamp);
        return false;
    }
    printf("Time: %s\n\r", ctime(&timestamp));
    set_time(timestamp);
    return true;
}
