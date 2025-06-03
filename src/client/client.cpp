#include "client.hpp"

Client::Client() : 
    screen_(ScreenInteractive::Fullscreen()), 
    Form_(std::make_shared<Form>()), 
    View_expenses_(std::make_shared<ViewExpenses>(localCollection.getCollection())) {
    
    // Initialize sidebar
    sidebar_ = std::make_shared<SideBarPanel>(std::vector<std::string>{
        "Home", "Add Expense", "View Expenses", "Settings", "Exit"
    }, &this->current_view);
    
    // Initialize main container
    main_container_ = Container::Vertical({
        sidebar_->GetComponent(),
        Form_->GetComponent()
    });
}

Element Client::LoginView(){
    return text("Login!") | center | flex;
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

void Client::HandleView(){
    if (Form_->checkIsClicked()){
        try {
            std::string name = Form_->getName();
            RecordType type = RecordType(Form_->getType());
            std::string category = Form_->getCategory();
            std::string amount = Form_->getAmount();

            if (!name.empty() && !category.empty() && !amount.empty()) {
                Record newRecord(
                    name,
                    type,
                    10,  // TODO: Generate proper ID
                    category,
                    std::stoi(amount),
                    std::time(0)
                );
                
                localCollection.addRecord(newRecord);
                
                // Create new form
                Form_ = std::make_shared<Form>();
                
                // Update the view expenses component
                View_expenses_ = std::make_shared<ViewExpenses>(localCollection.getCollection());
                
                // Switch to view expenses
                this->current_view = VIEW_EXPENSES;
                this->saveLocalCollection();
            }
        } catch (const std::exception& e) {
            // Handle any errors silently and keep the form open
            this->current_view = FORM;
        }
    }

    // Update the current view
    switch(this->current_view) {
        case HOME:
            current = hbox({
                sidebar_->Render() | size(WIDTH, EQUAL, 20),
                HomeView() | flex
            });
            break;
        case FORM:
            current = hbox({
                sidebar_->Render() | size(WIDTH, EQUAL, 20),
                FormView() | flex
            });
            break;
        case VIEW_EXPENSES:
            current = hbox({
                sidebar_->Render() | size(WIDTH, EQUAL, 20),
                ViewExpensesView() | flex
            });
            break;
        case SETTINGS:
            current = hbox({
                sidebar_->Render() | size(WIDTH, EQUAL, 20),
                SettingsView() | flex
            });
            break;
        case LOGIN:
            current = LoginView();
            break;
        case EXIT:
            screen_.Exit();
            break;
    }
}

void Client::saveLocalCollection(){
    std::ofstream outfile("data.bin", std::ios::binary);
        
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