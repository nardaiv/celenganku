#include "ui.hpp"

Form::Form(): isClicked(false) {
    this->input_name_ = Input(&name, "insert transaction name");
    
    this->type = 0;
    toggle_entries = {
        "Spending",
        "Earning"
    };
    
    toggle_type_ = Toggle(&toggle_entries, &type);    
    input_category_ = Input(&category, "insert category");

    // Improved amount input validation
    input_amount_ = Input(&amount, "insert amount");
    
    // Only allow digits and backspace
    input_amount_ |= CatchEvent([](Event event) {
        if (event.is_character()) {
            return !std::isdigit(event.character()[0]);
        }
        return false;
    });

    // Limit amount length to prevent integer overflow
    input_amount_ |= CatchEvent([this](Event event) {
        if (event.is_character() && amount.size() >= 9) {
            return true;
        }
        return false;
    });

    button_submit_ = Button("Add Transaction", [this] {
        // Basic validation before setting isClicked
        if (!name.empty() && !category.empty() && !amount.empty()) {
            try {
                // Validate amount can be converted to integer
                std::stoi(amount);
                isClicked = true;
            } catch (const std::exception& e) {
                // Invalid amount - don't set isClicked
            }
        }
    }, ButtonOption::Animated(Color::Blue));

    content_component_ = Container::Vertical({
        input_name_,
        input_category_,
        input_amount_,
        toggle_type_,
        button_submit_,
    });
}

Component Form::GetComponent() {
    return content_component_;
}

Element Form::Render() {
    // Add validation status indicators
    std::string name_status = name.empty() ? "❌" : "✓";
    std::string category_status = category.empty() ? "❌" : "✓";
    std::string amount_status = "❌";
    try {
        if (!amount.empty()) {
            std::stoi(amount);
            amount_status = "✓";
        }
    } catch (...) {}

    return 
        vbox({
            text("Add New Transaction") | bold | center,
            separator(),
            hbox(
                text("Transaction: "), 
                input_name_->Render() | flex,
                text(name_status)
            ),
            hbox(
                text("Type: "), 
                toggle_type_->Render() | flex
            ),
            hbox(
                text("Amount: "), 
                input_amount_->Render() | flex,
                text(amount_status)
            ),
            hbox(
                text("Category: "), 
                input_category_->Render() | flex,
                text(category_status)
            ),
            separator(),
            button_submit_->Render() | center,
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
        isClicked = false;  // Reset after checking
        return true;
    }
    return false;
}

