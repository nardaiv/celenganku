#pragma once
#include "server_socket.hpp"
#include "expense_tracker.hpp"
#include "record.hpp"
#include "record_type.hpp"

#include <map>
#include <string>
#include <vector>

// Request and Response structures
struct Request {
    enum Type {
        LOGIN,
        REGISTER
    } type;
    
    char username[32];
    char password[32];
};

struct Response {
    enum Status {
        SUCCESS,
        FAILED,
        USER_EXISTS,
        INVALID_CREDENTIALS
    } status;
    
    char message[128];
};

class Server {
private:
    ServerSocket socket;
    std::map<std::string, std::string> users;
    bool running;

public:
    Server(int port);
    void start();
    void stop();

private:
    void handle_client(SOCKET client_socket);
    Response handle_login(const Request& req);
    Response handle_register(const Request& req);
    void send_response(SOCKET client_socket, const Response& response);
    Request receive_request(SOCKET client_socket);
};
