#include "client.hpp"

Client::Client() : 
    screen_(ScreenInteractive::Fullscreen()), 
    Form_(std::make_shared<Form>()), 
    View_expenses_(std::make_shared<ViewExpenses>(localCollection.getCollection())),
    Login_(std::make_shared<Login>()),
    current_view(LOGIN) {  // Start with login view
    
    // Initialize sidebar
    sidebar_ = std::make_shared<SideBarPanel>(std::vector<std::string>{
        "Home", "Add Expense", "View Expenses", "Settings", "Exit"
    }, &this->current_view);
    
    // Initialize main container with only login component initially
    main_container_ = Container::Vertical({
        Login_->GetComponent()
    });
}

void Client::ReinitializeContainer(){
    main_container_ = Container::Vertical({
        this->sidebar_->GetComponent(),
        this->Form_->GetComponent(),
        this->Login_->GetComponent()
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
    return text("Setting!") | center;
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
            
            // TODO: Add proper authentication
            if (username == "admin" && password == "admin") {
                current_view = HOME;
            } else {
                Login_->setError("Invalid username or password");
            }
        } else if (Login_->checkRegisterClicked()) {
            std::string username = Login_->getUsername();
            std::string password = Login_->getPassword();
            
            // TODO: Add proper registration
            if (username == "admin") {
                Login_->setError("Username already exists");
            } else {
                // Registration successful
                current_view = HOME;
            }
        }
        
        current = Login_->Render();
        return;
    }

    // Handle form submission
    if (Form_->checkIsClicked()) {
        // try {
            std::string name = Form_->getName();
            RecordType type = RecordType(Form_->getType());
            std::string category = Form_->getCategory();
            std::string amount = Form_->getAmount();

            if (!name.empty() && !category.empty() && !amount.empty()) {
                // Generate a random ID between 1 and 1000000
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
                
                localCollection.addRecord(newRecord);
                
                // Create new form and update expenses view
                Form_ = std::make_shared<Form>();
                View_expenses_ = std::make_shared<ViewExpenses>(localCollection.getCollection());
                
                // Save to file and switch view
                saveLocalCollection();
                this->current_view = VIEW_EXPENSES;
            }
        // } catch (const std::exception& e) {
        //     std::cerr << "Error processing form: " << e.what() << std::endl;
        // }
    }
    // ReinitializeContainer();

    // Update the current view
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
            content = LoginView();
            break;
        case EXIT:
            screen_.Exit();
            return;
    }

    // Combine sidebar with content
    current = hbox({
        sidebar_->Render() | size(WIDTH, EQUAL, 20),
        content
    });
}

void Client::saveLocalCollection() {
    std::ofstream outfile("data.bin", std::ios::binary);
    if (!outfile) {
        std::cerr << "Error: Could not open data.bin for writing" << std::endl;
        return;
    }
        
    size_t size = this->localCollection.getCollection()->size();
    outfile.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    
    // Write the vector data
    outfile.write(reinterpret_cast<const char*>(this->localCollection.getCollection()->data()), size * sizeof(Record));
    outfile.close();
}

Client::~Client() {
    saveLocalCollection();
    screen_.Exit();
}