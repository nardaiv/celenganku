#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <stdexcept>

#pragma comment(lib, "ws2_32.lib")

class ClientSocket {
private:
    SOCKET clientSocket;
    WSADATA wsaData;
    struct sockaddr_in serverAddr;
    bool initialized;
    bool connected;

public:
    ClientSocket();
    ~ClientSocket();

    // Connection management
    void connect_to_server(const std::string& address, int port);
    void disconnect();
    bool is_connected() const { return connected; }

    // Data transmission
    void send_data(const char* data, int length);
    int receive_data(char* buffer, int buffer_size);

private:
    void initialize_winsock();
    void cleanup();
};
