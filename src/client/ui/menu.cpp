#include "ui.hpp"

SideBarPanel::SideBarPanel(const std::vector<std::string>& items) : menu_items_(items) {
    menu_component_ = Menu(&menu_items_, &selected_menu_);
    // menu_component_->set_on_enter([this] {
    //     // Handle menu selection logic here
    //     // For example, you can update the content panel based on the selected item
    // });
}

Component SideBarPanel::GetComponent(){
    return menu_component_;
}

Element SideBarPanel::Render(){
    return 
        vbox({
            text("Menu") | bold | bgcolor(Color::Blue),
            separator() | bgcolor(Color::Blue),
            menu_component_->Render()
        }) | border | size(WIDTH, EQUAL, 30);
}

int SideBarPanel::getSelectedItem() const { return selected_menu_; }
