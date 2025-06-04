#include "server.hpp"
#include "logger.hpp"
#include <iostream>
#include <csignal>

Server* server_instance = nullptr;

void signal_handler(int signal) {
    if (server_instance) {
        Logger::info("Shutting down server...");
        server_instance->stop();
    }
}

int main() {
    const int PORT = 8080;
    
    try {
        // Set up signal handling
        signal(SIGINT, signal_handler);
        
        // Create and start server
        Server server(PORT);
        server_instance = &server;
        
        Logger::info("Starting server on port " + std::to_string(PORT));
        server.start();
        
    } catch (const std::exception& e) {
        Logger::error("Server error: " + std::string(e.what()));
        return 1;
    }
    
    return 0;
}