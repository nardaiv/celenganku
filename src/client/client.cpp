#include "client.hpp"
#include <stdexcept>
#include <iostream>
#include <cstring>

Client::Client() : 
    screen_(ScreenInteractive::Fullscreen()), 
    Form_(std::make_shared<Form>()), 
    View_expenses_(std::make_shared<ViewExpenses>(localCollection.getCollection())),
    Login_(std::make_shared<Login>()),
    Settings_(std::make_shared<Settings>()),
    current_view(LOGIN),
    socket(std::make_unique<ClientSocket>()),
    connected(false) {
    
    // Load saved data
    loadLocalCollection();
    
    // Initialize sidebar
    sidebar_ = std::make_shared<SideBarPanel>(std::vector<std::string>{
        "Home", "Add Expense", "View Expenses", "Settings", "Exit", "Login"
    }, &this->current_view);
    
    // Initialize main container
    main_container_ = Container::Vertical({
        sidebar_->GetComponent(),
        Form_->GetComponent(),
        Login_->GetComponent(),
        Settings_->GetComponent()
    });

    // Try to connect to server
    try {
        if (connect_to_server("127.0.0.1", 8080)) {
            std::cout << "Connected to server successfully" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to connect to server: " << e.what() << std::endl;
    }
}

void Client::ExportData() {
    try {
        json data;
        data["user"] = this->current_user.empty() ? "anonymous" : this->current_user;
        data["records"] = json::array();

        auto records = localCollection.getCollection();
        std::cout << "Exporting " << records->size() << " records..." << std::endl;

        if (records->empty()) {
            std::cout << "Warning: No records found to export!" << std::endl;
        }

        for (const auto& record : *records) {
            json recordData;
            recordData["id"] = record.getId();
            recordData["name"] = record.getName();
            recordData["type"] = static_cast<int>(record.getType());
            recordData["category"] = record.getCategory();
            recordData["amount"] = record.getAmount();
            recordData["timestamp"] = record.getDate();
            
            std::cout << "Exporting record: " << record.getName() 
                      << " (ID: " << record.getId() 
                      << ", Amount: " << record.getAmount() << ")" << std::endl;
            
            data["records"].push_back(recordData);
        }

        std::ofstream file("data.json");
        if (!file.is_open()) {
            throw std::runtime_error("Could not open data.json for writing");
        }

        file << data.dump(4);
        file.close();

        if (file.fail()) {
            throw std::runtime_error("Error while writing to data.json");
        }

        std::cout << "Successfully exported data to data.json" << std::endl;
        // Settings_->setMessage("Data exported successfully to data.json");
    } catch (const std::exception& e) {
        std::cerr << "Error exporting data: " << e.what() << std::endl;
        // Settings_->setMessage("Error exporting data: " + std::string(e.what()));
    }
}

void Client::ReinitializeContainer() {
    main_container_ = Container::Vertical({
        this->sidebar_->GetComponent(),
        this->Form_->GetComponent(),
        this->Login_->GetComponent(),
        this->Settings_->GetComponent()
    });
}

Element Client::LoginView() {
    return Login_->Render();
}

Element Client::FormView() {
    return Form_->Render();
}

Element Client::HomeView() {
    return text("Home!") | center;
}

Element Client::SettingsView() {
    return Settings_->Render();
}

Element Client::ViewExpensesView() {
    return View_expenses_->Render();
}

void Client::Run() {
    app_renderer_ = Renderer(main_container_, [this]{
        this->HandleView();
        return this->current;
    });

    screen_.Loop(app_renderer_);
}

void Client::HandleView() {
    // Handle login first
    if (current_view == LOGIN) {
        if (Login_->checkLoginClicked()) {
            std::string username = Login_->getUsername();
            std::string password = Login_->getPassword();
            
            if (login(username, password)) {
                current_view = HOME;
            } else {
                // Login_->setError("Invalid username or password");
            }
        } else if (Login_->checkRegisterClicked()) {
            std::string username = Login_->getUsername();
            std::string password = Login_->getPassword();
            
            if (register_user(username, password)) {
                current_view = HOME;
                // Login_->Reset();
                // Login_ = std::make_shared<Login>();
                // ReinitializeContainer();
                
            } else {
                // Login_->setError("Registration failed - username might already exist");
            }
        }
        
        current = Login_->Render();
        return;
    }

    // Handle form submission
    if (Form_->checkIsClicked()) {
        std::string name = Form_->getName();
        RecordType type = RecordType(Form_->getType());
        std::string category = Form_->getCategory();
        std::string amount = Form_->getAmount();

        std::cout << "Form submitted with values:" << std::endl
                  << "Name: " << name << std::endl
                  << "Type: " << static_cast<int>(type) << std::endl
                  << "Category: " << category << std::endl
                  << "Amount: " << amount << std::endl;

        if (!name.empty() && !category.empty() && !amount.empty()) {
            try {
                std::srand(static_cast<unsigned int>(std::time(nullptr)));
                int id = 1 + (std::rand() % 1000000);
                
                Record newRecord(
                    name,
                    type,
                    id,
                    category,
                    std::stoi(amount),
                    std::time(0)
                );
                
                std::cout << "Adding new record:" << std::endl
                          << "ID: " << newRecord.getId() << std::endl
                          << "Name: " << newRecord.getName() << std::endl
                          << "Type: " << static_cast<int>(newRecord.getType()) << std::endl
                          << "Category: " << newRecord.getCategory() << std::endl
                          << "Amount: " << newRecord.getAmount() << std::endl;
                
                localCollection.addRecord(newRecord);
                
                // Verify record was added
                auto records = localCollection.getCollection();
                std::cout << "Current collection size: " << records->size() << std::endl;
                
                // Form_ = std::make_shared<Form>();
                View_expenses_ = std::make_shared<ViewExpenses>(localCollection.getCollection());
                
                saveLocalCollection();
                std::cout << "Record saved to local collection" << std::endl;
                
                this->current_view = VIEW_EXPENSES;
            } catch (const std::exception& e) {
                std::cerr << "Error adding record: " << e.what() << std::endl;
                // Form_->setError("Error adding record: " + std::string(e.what()));
            }
        } else {
            std::cout << "Form validation failed - some fields are empty" << std::endl;
            // Form_->setError("Please fill in all fields");
        }
    }

    if(Settings_->checkExportClicked()) {
        ExportData();
        Settings_->resetExportClicked();
    }

    Element content;
    switch(this->current_view) {
        case HOME:
            content = HomeView() | flex;
            break;
        case FORM:
            content = FormView() | flex;
            break;
        case VIEW_EXPENSES:
            content = ViewExpensesView() | flex;
            break;
        case SETTINGS:
            content = SettingsView() | flex;
            break;
        case LOGIN:
            content = LoginView() | flex;
            break;
        case EXIT:
            screen_.Exit();
            return;
    }

    current = hbox({
        sidebar_->Render() | size(WIDTH, EQUAL, 20),
        content
    });
}

void Client::loadLocalCollection() {
    struct Data {
        std::vector<Record> records;
        std::string current_user;
    };

    try {
        std::cout << "Loading local collection..." << std::endl;

        std::ifstream infile("data.bin", std::ios::binary);
        if (!infile) {
            std::cout << "No existing data file found. Starting fresh." << std::endl;
            return;
        }

        Data data;

        // Read current user
        size_t userLen;
        infile.read(reinterpret_cast<char*>(&userLen), sizeof(size_t));
        
        std::vector<char> userBuffer(userLen);
        infile.read(userBuffer.data(), userLen);
        data.current_user = std::string(userBuffer.data(), userLen);

        // Read records
        size_t recordCount;
        infile.read(reinterpret_cast<char*>(&recordCount), sizeof(size_t));
        
        // Read records one by one
        for (size_t i = 0; i < recordCount; ++i) {
            // Read individual fields
            int id;
            RecordType type;
            int amount;
            time_t date;
            
            // Read fixed-size fields
            infile.read(reinterpret_cast<char*>(&id), sizeof(int));
            infile.read(reinterpret_cast<char*>(&type), sizeof(RecordType));
            infile.read(reinterpret_cast<char*>(&amount), sizeof(int));
            infile.read(reinterpret_cast<char*>(&date), sizeof(time_t));
            
            // Read name length and string
            size_t nameLen;
            infile.read(reinterpret_cast<char*>(&nameLen), sizeof(size_t));
            std::vector<char> nameBuffer(nameLen);
            infile.read(nameBuffer.data(), nameLen);
            std::string name(nameBuffer.data(), nameLen);
            
            // Read category length and string
            size_t categoryLen;
            infile.read(reinterpret_cast<char*>(&categoryLen), sizeof(size_t));
            std::vector<char> categoryBuffer(categoryLen);
            infile.read(categoryBuffer.data(), categoryLen);
            std::string category(categoryBuffer.data(), categoryLen);
            
            // Create record with all fields
            data.records.emplace_back(name, type, id, category, amount, date);
        }

        if (infile.fail()) {
            throw std::runtime_error("Error while reading data.bin");
        }

        infile.close();

        // Update the local collection and current user
        localCollection.setCollection(std::make_shared<std::vector<Record>>(std::move(data.records)));
        current_user = std::move(data.current_user);

        std::cout << "Successfully loaded " << recordCount << " records" << std::endl;
        std::cout << "Loaded user: " << current_user << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error loading collection: " << e.what() << std::endl;
    }
}

void Client::saveLocalCollection() {
    struct Data {
        std::vector<Record> records;
        std::string current_user;
    };

    try {
        std::cout << "Saving local collection..." << std::endl;
        auto records = localCollection.getCollection();

        Data data;
        data.records = *records;
        data.current_user = current_user;

        std::cout << "Number of records to save: " << data.records.size() << std::endl;
        std::cout << "Current user: " << data.current_user << std::endl;

        std::ofstream outfile("data.bin", std::ios::binary);
        if (!outfile) {
            throw std::runtime_error("Could not open data.bin for writing");
        }

        // Write current user string length and data
        size_t userLen = data.current_user.length();
        outfile.write(reinterpret_cast<const char*>(&userLen), sizeof(size_t));
        outfile.write(data.current_user.c_str(), userLen);

        // Write records count
        size_t recordCount = data.records.size();
        outfile.write(reinterpret_cast<const char*>(&recordCount), sizeof(size_t));

        // Write records one by one
        for (const auto& record : data.records) {
            // Write fixed-size fields
            int id = record.getId();
            RecordType type = record.getType();
            int amount = record.getAmount();
            time_t date = record.getDate();
            
            outfile.write(reinterpret_cast<const char*>(&id), sizeof(int));
            outfile.write(reinterpret_cast<const char*>(&type), sizeof(RecordType));
            outfile.write(reinterpret_cast<const char*>(&amount), sizeof(int));
            outfile.write(reinterpret_cast<const char*>(&date), sizeof(time_t));
            
            // Write name length and string
            std::string name = record.getName();
            size_t nameLen = name.length();
            outfile.write(reinterpret_cast<const char*>(&nameLen), sizeof(size_t));
            outfile.write(name.c_str(), nameLen);
            
            // Write category length and string
            std::string category = record.getCategory();
            size_t categoryLen = category.length();
            outfile.write(reinterpret_cast<const char*>(&categoryLen), sizeof(size_t));
            outfile.write(category.c_str(), categoryLen);
        }
        
        outfile.close();
        if (outfile.fail()) {
            throw std::runtime_error("Error while writing to data.bin");
        }
        
        std::cout << "Successfully saved " << recordCount << " records and user data to data.bin" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error saving collection: " << e.what() << std::endl;
    }
}

Client::~Client() {
    saveLocalCollection();
    screen_.Exit();
    if (connected) {
        disconnect();
    }
}

bool Client::connect_to_server(const std::string& address, int port) {
    try {
        socket->connect_to_server(address, port);
        connected = true;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
        connected = false;
        return false;
    }
}

void Client::disconnect() {
    if (connected) {
        socket->disconnect();
        connected = false;
        current_user.clear();
    }
}

bool Client::login(const std::string& username, const std::string& password) {
    if (!connected) {
        std::cerr << "Not connected to server" << std::endl;
        return false;
    }

    Request req;
    req.type = Request::LOGIN;
    strncpy(req.username, username.c_str(), sizeof(req.username) - 1);
    strncpy(req.password, password.c_str(), sizeof(req.password) - 1);

    try {
        Response resp = send_request(req);
        if (resp.status == Response::SUCCESS) {
            current_user = username;
            return true;
        }
    } catch (const std::exception& e) {
        std::cerr << "Login error: " << e.what() << std::endl;
    }
    return false;
}

bool Client::register_user(const std::string& username, const std::string& password) {
    if (!connected) {
        std::cerr << "Not connected to server" << std::endl;
        return false;
    }

    Request req;
    req.type = Request::REGISTER;
    strncpy(req.username, username.c_str(), sizeof(req.username) - 1);
    strncpy(req.password, password.c_str(), sizeof(req.password) - 1);

    try {
        Response resp = send_request(req);
        current_user = username;
        return resp.status == Response::SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "Registration error: " << e.what() << std::endl;
        return false;
    }
}

Response Client::send_request(const Request& req) {
    if (!connected) {
        throw std::runtime_error("Not connected to server");
    }

    socket->send_data(reinterpret_cast<const char*>(&req), sizeof(Request));

    Response resp;
    int bytes_received = socket->receive_data(reinterpret_cast<char*>(&resp), sizeof(Response));

    if (bytes_received != sizeof(Response)) {
        throw std::runtime_error("Invalid response size");
    }

    return resp;
}