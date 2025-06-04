#include "server_socket.hpp"
#include <stdexcept>

ServerSocket::ServerSocket(int port) : initialized(false) {
    initialize_winsock();
    
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        throw std::runtime_error("Failed to create socket");
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
}

ServerSocket::~ServerSocket() {
    if (initialized) {
        closesocket(serverSocket);
        WSACleanup();
    }
}

void ServerSocket::initialize_winsock() {
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("WSAStartup failed");
    }
    initialized = true;
}

void ServerSocket::bind_and_listen(int backlog) {
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        throw std::runtime_error("Bind failed");
    }

    if (listen(serverSocket, backlog) == SOCKET_ERROR) {
        throw std::runtime_error("Listen failed");
    }
}

SOCKET ServerSocket::accept_connection() {
    SOCKET clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        throw std::runtime_error("Accept failed");
    }
    return clientSocket;
}

void ServerSocket::close_socket() {
    closesocket(serverSocket);
    WSACleanup();
    initialized = false;
}

void ServerSocket::send_data(SOCKET client_socket, const char* data, int length) {
    if (send(client_socket, data, length, 0) == SOCKET_ERROR) {
        throw std::runtime_error("Send failed");
    }
}

int ServerSocket::receive_data(SOCKET client_socket, char* buffer, int buffer_size) {
    int bytes_received = recv(client_socket, buffer, buffer_size, 0);
    if (bytes_received == SOCKET_ERROR) {
        throw std::runtime_error("Receive failed");
    }
    return bytes_received;
} 