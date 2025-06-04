#include "client_socket.hpp"

ClientSocket::ClientSocket() : initialized(false), connected(false) {
    initialize_winsock();
}

ClientSocket::~ClientSocket() {
    if (connected) {
        disconnect();
    }
    if (initialized) {
        cleanup();
    }
}

void ClientSocket::initialize_winsock() {
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("WSAStartup failed");
    }
    initialized = true;
}

void ClientSocket::connect_to_server(const std::string& address, int port) {
    if (connected) {
        throw std::runtime_error("Already connected to server");
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        throw std::runtime_error("Failed to create socket");
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, address.c_str(), &serverAddr.sin_addr) <= 0) {
        closesocket(clientSocket);
        throw std::runtime_error("Invalid address");
    }

    // Connect to server
    if (::connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(clientSocket);
        throw std::runtime_error("Connection failed");
    }

    connected = true;
}

void ClientSocket::disconnect() {
    if (connected) {
        closesocket(clientSocket);
        connected = false;
    }
}

void ClientSocket::cleanup() {
    if (initialized) {
        WSACleanup();
        initialized = false;
    }
}

void ClientSocket::send_data(const char* data, int length) {
    if (!connected) {
        throw std::runtime_error("Not connected to server");
    }

    if (send(clientSocket, data, length, 0) == SOCKET_ERROR) {
        throw std::runtime_error("Send failed");
    }
}

int ClientSocket::receive_data(char* buffer, int buffer_size) {
    if (!connected) {
        throw std::runtime_error("Not connected to server");
    }

    int bytes_received = recv(clientSocket, buffer, buffer_size, 0);
    if (bytes_received == SOCKET_ERROR) {
        throw std::runtime_error("Receive failed");
    }

    return bytes_received;
} 