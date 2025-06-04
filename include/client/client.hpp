#ifndef CLIENT_HPP
#define CLIENT_HPP

// Windows headers must come first
#include "client_socket.hpp"

// FTXUI headers
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include "ftxui/util/ref.hpp"  // for Ref

#include <nlohmann/json.hpp>

// Project headers
#include "expense_tracker.hpp"
#include "ui.hpp"
#include "record.hpp"
#include "record_type.hpp"

// Standard headers
#include <string>
#include <vector>
#include <memory>

// Request/Response structures
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

using namespace ftxui;
using json = nlohmann::json;

class Client {
private:    
    ExpenseTracker localCollection;
    void saveLocalCollection();
    void loadLocalCollection();
    
    // UI Props
    Component app_renderer_;
    Component main_container_;
    ScreenInteractive screen_;
    int current_view = 0;
    Element current;
    std::shared_ptr<SideBarPanel> sidebar_;
    void HandleView();

    // std::shared_ptr<Home> Home_;
    std::shared_ptr<Form> Form_;
    std::shared_ptr<ViewExpenses> View_expenses_;
    std::shared_ptr<Login> Login_;
    std::shared_ptr<Settings> Settings_;
    // std::shared_ptr<Login> Login_;

    // Views
    Element LoginView();
    Element HomeView();
    Element FormView();
    Element SettingsView();
    Element ViewExpensesView();
    
    void ReinitializeContainer();
    void ExportData();

    // Input
    // std::string name;
    // RecordType type;
    // std::string category;
    // std::string amount;

    // Socket
    std::unique_ptr<ClientSocket> socket;
    bool connected;
    std::string current_user;
    bool connect_to_server(const std::string& address, int port);
    void disconnect();
    Response send_request(const Request& req);
    
public:
    Client();
    void Run();
    virtual ~Client();

    // Authentication methods
    bool login(const std::string& username, const std::string& password);
    bool register_user(const std::string& username, const std::string& password);
    bool is_connected() const { return connected; }
    const std::string& get_current_user() const { return current_user; }
};

#endif
