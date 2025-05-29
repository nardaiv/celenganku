#include "ui.hpp"

Home::Home(){

    this->input_name_ = Input(&name, "insert transation name");
    
    this->toggle_selected = 0;
    toggle_entries = {
        "Spending",
        "Earning"
    };
    
    toggle_type_ = Toggle(&toggle_entries, &toggle_selected);    
    input_category_ = Input(&category, "insert category");

    input_amount_ = Input(&amount, "phone number");
    input_amount_ |= CatchEvent([&](Event event) {
        return event.is_character() && !std::isdigit(event.character()[0]);
    });

    input_amount_ |= CatchEvent([&](Event event) {
        return event.is_character() && amount.size() > 10;
    });

    content_component_ = Container::Vertical({
        input_name_,
        input_category_,
        input_amount_,
        toggle_type_,
    });
}

Component Home::GetComponent() {
    return content_component_;
}

Element Home::Render(){
    return 
        hbox({
            
            vbox({
                text("Home") | bold |center,
                separator(),
                hbox(text("Transation : "), input_name_->Render()),
                hbox(text("Type       : "), toggle_type_->Render()),
                hbox(text("Amount     : "), input_amount_->Render()),
                hbox(text("Category   : "), input_category_->Render()),
            })| flex,
            
            
        }) | border ;
}

void Home::setSelectedContent(int index) {
    selected_content_ = index;
}