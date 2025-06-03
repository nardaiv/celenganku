#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "expense_tracker.hpp"
#include "ui.hpp"
#include "record.hpp"
#include "record_type.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include "ftxui/util/ref.hpp"  // for Ref

#include <string>
#include <vector>
#include <memory>

using namespace ftxui;

class Client{
private:
    // Data
    ExpenseTracker localCollection;
    void saveLocalCollection();
    
    // UI Props
    Component app_renderer_;
    Component main_container_;
    ScreenInteractive screen_;
    int current_view = 0;
    // int current_view2 = 0;
    Element current;
    std::shared_ptr<SideBarPanel> sidebar_;
    void HandleView();

    // std::shared_ptr<Home> Home_;
    std::shared_ptr<Form> Form_;
    std::shared_ptr<ViewExpenses> View_expenses_;
    // std::shared_ptr<Settings> Settings_;
    // std::shared_ptr<Login> Login_;

    // Views
    Element LoginView();
    Element HomeView();
    Element FormView();
    Element SettingsView();
    Element ViewExpensesView();
    

public:
    Client();
    void Run();
    virtual ~Client();
};


#endif