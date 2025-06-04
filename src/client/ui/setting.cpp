#include "ui.hpp"

Settings::Settings() : export_clicked_(false) {
    // Initialize the export button
    export_button_ = Button("Export to JSON", [this] {
        export_clicked_ = true;
    }, ButtonOption::Animated(Color::Blue));

    // Create the main container with the button
    content_component_ = Container::Vertical({
        export_button_
    });
}

Component Settings::GetComponent() {
    return content_component_;
}

Element Settings::Render() {
    return vbox({
        text("Settings") | bold | hcenter,
        separator(),
        hbox({
            export_button_->Render() | center
        }) | center
    }) | border;
}

bool Settings::checkExportClicked() {
    bool was_clicked = export_clicked_;
    export_clicked_ = false;
    return was_clicked;
}

void Settings::resetExportClicked() {
    export_clicked_ = false;
} 