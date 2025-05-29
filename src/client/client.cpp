#include "client.hpp"

Client::Client() : 
    screen_(ScreenInteractive::Fullscreen()), 
    home_(std::make_shared<Home>()) {
    // Initialize the local collection
    // this->localCollection = ExpenseTracker();

    // auto sidebar_ = std::make_shared<SideBarPanel>(std::vector<std::string>{
    //     "Home", "Add Expense", "View Expenses", "Settings", "Exit"
    // });

    // auto content_ = std::make_shared<ContentPanel>();

    // this->main_container_ =  Container::Horizontal({
    //         sidebar_->GetComponent(),
    // });


    // // Set the initial focus to the sidebar
    // app_renderer_ = Renderer(main_container_, [this, sidebar_, content_]() {

    //     int selected_item = sidebar_->getSelectedItem();
    //     if (selected_item != -1) {
    //         switch (selected_item) {
    //             case 0: 
    //                 content_->setSelectedContent(0);
    //                 break;
    //             case 1: 
    //                 content_->setSelectedContent(1);
    //                 break;
    //             case 2: 
    //                 content_->setSelectedContent(2);
    //                 break;
    //             case 3: 
    //                 content_->setSelectedContent(3);
    //                 break;
    //             case 4: 
    //                 screen_.Exit();
                    
    //             default:
    //                 content_->setSelectedContent(0);
    //         }
    //     }

    //     // Layout
    //     return vbox({
    //         text("Expense Tracker") | bold | center,
    //         separator(),
    //         hflow({
    //             // main_container_->Render(),
    //             sidebar_->Render(),
    //             content_->Render()
    //         }) | border,
    //     });
    // });
}

Element Client::LoginView(){
    return text("Login!") | center |flex;
}

Element Client::HomeView() {
    // auto home = std::make_shared<Home>();
    // main_container_ = Container::Vertical({sidebar_->GetComponent(), home->GetComponent()});
    // return home->Render();


    
    // main_container_ = Container::Vertical({sidebar_->GetComponent(), home_->GetComponent()});

    return home_->Render();
}

Element Client::FormView() {
    return text("Form!") | center;
}

Element Client::SettingsView() {
    return text("Setting!") | center;
}

Element Client::ViewExpenses() {
    return text("View Expenses!") | center;
}

void Client::saveLocalCollection(){

}

void Client::Run() {
    sidebar_ = std::make_shared<SideBarPanel>(std::vector<std::string>{
        "Home", "Add Expense", "View Expenses", "Settings", "Exit"
    });

    home_ = std::make_shared<Home>();
    main_container_ = Container::Vertical({sidebar_->GetComponent(), home_->GetComponent()});

    // temporary
    this->current_view = HOME;

    app_renderer_ = Renderer(main_container_, [this]{
        this->HandleView();
        return this->current;
    } );

    screen_.Loop(app_renderer_);
}

void Client::HandleView(){
    if(current_view != LOGIN)
        this->current_view = sidebar_.get()->getSelectedItem();

    switch(this->current_view) {
        case HOME:
            // current = hbox(this->sidebar_->Render(), this->HomeView());
            current = hbox({
                this->sidebar_->Render(), 
                this->HomeView() | flex
            });
            // main_container_ = Container::Vertical({sidebar_->GetComponent(), });
            break;
        case FORM:
            current =hbox(this->sidebar_->Render(), this->FormView());
            break;
        case VIEW_EXPENSES:
            current = hbox(this->sidebar_->Render(),this->ViewExpenses());
            break;
        case SETTINGS:
            current = hbox(this->sidebar_->Render(), this->SettingsView());
            break;
        case LOGIN:
            current = this->LoginView();
            break;
        case EXIT:
            this->screen_.Exit();
            break;
    }
}

Client::~Client() {
    this->saveLocalCollection();
    this->screen_.Exit();
};