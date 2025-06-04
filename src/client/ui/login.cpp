#include "ui.hpp"

Login::Login() : 
    username(""),
    password(""),
    isLoginClicked(false),
    isRegisterClicked(false),
    isError(false),
    error_message("") {

    // Initialize input components
    input_username_ = Input(&username, "Username");
    
    // Create password input with password masking
    InputOption password_option;
    password_option.password = true;
    input_password_ = Input(&password, "Password", password_option);

    // Initialize buttons
    button_login_ = Button("Login", [this] {
        if (!username.empty() && !password.empty()) {
            isLoginClicked = true;
        } else {
            setError("Please fill in all fields");
        }
    }, ButtonOption::Animated(Color::Blue));

    button_register_ = Button("Register", [this] {
        if (!username.empty() && !password.empty()) {
            isRegisterClicked = true;
        } else {
            setError("Please fill in all fields");
        }
    }, ButtonOption::Animated(Color::Green));

    // Create main container
    content_component_ = Container::Vertical({
        input_username_,
        input_password_,
        Container::Horizontal({
            button_login_,
            button_register_
        })
    });
}

Component Login::GetComponent() {
    return content_component_;
}

Element Login::Render() {
    auto title = text("CelenganKu") | bold | center;
    auto subtitle = text("Your Personal Finance Manager") | dim | center;

    auto form = vbox({
        text("Login or Register") | bold | center,
        separator(),
        hbox(text("Username: "), input_username_->Render() | flex),
        hbox(text("Password: "), input_password_->Render() | flex),
        separator(),
        hbox({
            button_login_->Render(),
            text(" "),  // spacer
            button_register_->Render()
        }) | center | flex
    });

    Elements elements;
    elements.push_back(title);
    elements.push_back(subtitle);
    elements.push_back(separator());
    elements.push_back(form | size(WIDTH, EQUAL, 100) | center);

    if (isError) {
        elements.push_back(separator());
        elements.push_back(text(error_message) | color(Color::Red) | center);
    }

    return vbox(std::move(elements)) | border | flex;
}

void Login::Update() {
    // Reset click states if they were true
    if (isLoginClicked || isRegisterClicked) {
        Reset();
    }
}

void Login::Reset() {
    username = "";
    password = "";
    isLoginClicked = false;
    isRegisterClicked = false;
    clearError();
}

std::string Login::getUsername() const {
    return username;
}

std::string Login::getPassword() const {
    return password;
}

bool Login::checkLoginClicked() {
    if (isLoginClicked) {
        isLoginClicked = false;
        return true;
    }
    return false;
}

bool Login::checkRegisterClicked() {
    if (isRegisterClicked) {
        isRegisterClicked = false;
        return true;
    }
    return false;
}

void Login::setError(const std::string& message) {
    isError = true;
    error_message = message;
}

void Login::clearError() {
    isError = false;
    error_message = "";
}
