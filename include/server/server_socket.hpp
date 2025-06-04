#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <stdexcept>

#pragma comment(lib, "ws2_32.lib")

class ServerSocket {
private:
    SOCKET serverSocket;
    WSADATA wsaData;
    struct sockaddr_in serverAddr;
    bool initialized;

public:
    ServerSocket(int port);
    ~ServerSocket();
    
    void bind_and_listen(int backlog = 5);
    SOCKET accept_connection();
    void close_socket();
    
    // Helper methods
    static void send_data(SOCKET client_socket, const char* data, int length);
    static int receive_data(SOCKET client_socket, char* buffer, int buffer_size);
    
private:
    void initialize_winsock();
};
