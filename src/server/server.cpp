#include "server.hpp"
#include "logger.hpp"
#include <iostream>
#include <thread>

Server::Server(int port) : socket(port), running(false) {}

void Server::start() {
    running = true;
    socket.bind_and_listen();
    Logger::info("Server started. Waiting for connections...");

    while (running) {
        try {
            SOCKET client_socket = socket.accept_connection();
            Logger::info("Client connected!");
            
            // Handle client in a new thread
            std::thread client_thread(&Server::handle_client, this, client_socket);
            client_thread.detach();
        } catch (const std::exception& e) {
            Logger::error("Error: " + std::string(e.what()));
        }
    }
}

void Server::stop() {
    running = false;
    socket.close_socket();
    Logger::info("Server stopped");
}

void Server::handle_client(SOCKET client_socket) {
    try {
        while (running) {
            Request req = receive_request(client_socket);
            Response resp;

            switch (req.type) {
                case Request::LOGIN:
                    Logger::info("Processing login request for user: " + std::string(req.username));
                    resp = handle_login(req);
                    break;
                case Request::REGISTER:
                    Logger::info("Processing registration request for user: " + std::string(req.username));
                    resp = handle_register(req);
                    break;
                default:
                    Logger::error("Invalid request type received");
                    resp.status = Response::FAILED;
                    strncpy(resp.message, "Invalid request type", sizeof(resp.message));
            }

            send_response(client_socket, resp);
            Logger::info("Response sent to client: " + std::string(resp.message));
        }
    } catch (const std::exception& e) {
        Logger::error("Client handling error: " + std::string(e.what()));
    }

    closesocket(client_socket);
    Logger::info("Client disconnected");
}

Response Server::handle_login(const Request& req) {
    Response resp;
    auto it = users.find(req.username);
    
    if (it != users.end() && it->second == req.password) {
        resp.status = Response::SUCCESS;
        strncpy(resp.message, "Login successful", sizeof(resp.message));
        Logger::info("Login successful for user: " + std::string(req.username));
    } else {
        resp.status = Response::INVALID_CREDENTIALS;
        strncpy(resp.message, "Invalid username or password", sizeof(resp.message));
        Logger::error("Login failed for user: " + std::string(req.username));
    }
    
    return resp;
}

Response Server::handle_register(const Request& req) {
    Response resp;
    auto it = users.find(req.username);
    
    if (it != users.end()) {
        resp.status = Response::USER_EXISTS;
        strncpy(resp.message, "Username already exists", sizeof(resp.message));
        Logger::error("Registration failed - user already exists: " + std::string(req.username));
    } else {
        users[req.username] = req.password;
        resp.status = Response::SUCCESS;
        strncpy(resp.message, "Registration successful", sizeof(resp.message));
        Logger::info("New user registered: " + std::string(req.username));
    }
    
    return resp;
}

void Server::send_response(SOCKET client_socket, const Response& response) {
    ServerSocket::send_data(client_socket, (const char*)&response, sizeof(Response));
}

Request Server::receive_request(SOCKET client_socket) {
    Request req;
    int bytes_received = ServerSocket::receive_data(client_socket, (char*)&req, sizeof(Request));
    
    if (bytes_received != sizeof(Request)) {
        Logger::error("Invalid request size received");
        throw std::runtime_error("Invalid request size");
    }
    
    return req;
} 