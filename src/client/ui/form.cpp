#include "ui.hpp"

Form::Form(): 
    isClicked(false), 
    name(""), 
    category(""), 
    amount(""), 
    type(0),
    name_status("x"),
    category_status("x"),
    amount_status("x") {
    
    input_name_ = Input(&name, "insert transaction name");
    
    toggle_entries = {
        "Spending",
        "Earning"
    };
    
    toggle_type_ = Toggle(&toggle_entries, &type);    
    input_category_ = Input(&category, "insert category");
    input_amount_ = Input(&amount, "insert amount");
    
    // Only allow digits and backspace
    input_amount_ |= CatchEvent([](Event event) {
        if (!event.is_character()) return false;
        return !std::isdigit(event.character()[0]);
    });

    // Limit amount length to prevent integer overflow
    input_amount_ |= CatchEvent([this](Event event) {
        if (!event.is_character()) return false;
        return amount.size() >= 9;
    });

    button_submit_ = Button("Add Transaction", [this] {
        // Basic validation before setting isClicked
        if (!name.empty() && !category.empty() && !amount.empty()) {
            try {
                std::stoi(amount);
                isClicked = true;
            } catch (const std::exception&) {
                // Invalid amount - don't set isClicked
            }
        }
    }, ButtonOption::Animated(Color::Blue));

    content_component_ = Container::Vertical({
        input_name_,
        input_category_,
        input_amount_,
        toggle_type_,
        button_submit_
    });

    // Update();
}

// void Form::Update() {
//      name_status = name.empty() ? "❌" : "✓";
//      category_status = category.empty() ? "❌" : "✓";
//      amount_status = "❌";
    
//     try {
//         if (!amount.empty()) {
//             std::stoi(amount);
//             amount_status = "✓";
//         }
//     } catch (...) {}
// }

// void Form::Reset() {
//     name = "";
//     category = "";
//     amount = "";
//     type = 0;
//     isClicked = false;
//     Update();
// }

Component Form::GetComponent() {
    return content_component_;
}

Element Form::Render() {
    // Update();
    return vbox({
        text("Add New Transaction") | bold | center,
        separator(),
        hbox(
            text("Transaction   : "), 
            input_name_->Render() | flex,
            text(name != "" ? "✓" : "x")
        ),
        hbox(
            text("Type          : "), 
            toggle_type_->Render() | flex
        ),
        hbox(
            text("Amount        : "), 
            input_amount_->Render() | flex,
            text(amount  != "" ? "✓" : "x")
        ),
        hbox(
            text("Category      : "), 
            input_category_->Render() | flex,
            text(category != "" ? "✓" : "x")
        ),
        separator(),
        button_submit_->Render() | center
    }) | border;
}

std::string Form::getName() const {
    return this->name;
}

std::string Form::getCategory() const {
    return this->category;
}

int Form::getType() const {
    return this->type;
}

std::string Form::getAmount() const {
    return this->amount;
}

bool Form::checkIsClicked() {
    if (isClicked) {
        // Reset();
        isClicked = false;
        return true;
    }
    return false;
}

