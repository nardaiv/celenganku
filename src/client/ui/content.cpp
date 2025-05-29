#include "ui.hpp"

ContentPanel::ContentPanel(): content_component_(){
    
}

Component ContentPanel::GetComponent() {
    return Container::Vertical({});
}

Element ContentPanel::Render(){
    return selected_content_ == 0 ? text("Home") | center :
           selected_content_ == 1 ? text("Add Expense") | center :
           selected_content_ == 2 ? text("View Expenses") | center :
           selected_content_ == 3 ? text("Settings") | center :
           text("Exit") | center;
}

void ContentPanel::setSelectedContent(int index) {
    selected_content_ = index;
}