#include "ui.hpp"
#include <iomanip>
#include <sstream>
#include <algorithm>

ViewExpenses::ViewExpenses(std::shared_ptr<std::vector<Record>> collection) : collection(collection) {
    // Sort records by date (newest first)
    std::sort(collection->begin(), collection->end(), 
        [](const Record& a, const Record& b) {
            return a.getDate() > b.getDate();
        });
}

Component ViewExpenses::GetComponent() {
    return Container::Vertical({});
}

Element ViewExpenses::Render() {
    Elements children;
    
    // Calculate total income and expenses
    int total_income = 0;
    int total_expenses = 0;
    for(const auto& record : *collection) {
        if(record.getType() == EARNING) {
            total_income += record.getAmount();
        } else {
            total_expenses += record.getAmount();
        }
    }
    
    // Add summary section
    children.push_back(
        vbox({
            text("Summary") | bold | center,
            hbox({
                text("Total Income: ") | bold,
                text(std::to_string(total_income)) | color(Color::Green)
            }),
            hbox({
                text("Total Expenses: ") | bold,
                text(std::to_string(total_expenses)) | color(Color::Red)
            }),
            hbox({
                text("Balance: ") | bold,
                text(std::to_string(total_income - total_expenses)) | 
                    color(total_income >= total_expenses ? Color::Green : Color::Red)
            })
        }) | border
    );
    
    children.push_back(separator());
    
    // Add headers
    children.push_back(
        hbox({
            text("Date") | size(WIDTH, EQUAL, 20) | bold,
            text("Name") | size(WIDTH, EQUAL, 20) | bold,
            text("Amount") | size(WIDTH, EQUAL, 15) | bold,
            text("Category") | size(WIDTH, EQUAL, 15) | bold,
            text("Type") | size(WIDTH, EQUAL, 10) | bold
        })
    );
    
    children.push_back(separator());

    // Add records
    for(const auto& record : *collection) {
        // Format amount with commas
        std::stringstream ss;
        ss << std::fixed << std::setprecision(0) << record.getAmount();
        std::string amount = ss.str();
        
        // Format date
        char date_str[26];
        time_t date = record.getDate();
        ctime_s(date_str, sizeof(date_str), &date);
        std::string formatted_date(date_str);
        if (!formatted_date.empty() && formatted_date[formatted_date.length()-1] == '\n') {
            formatted_date = formatted_date.substr(0, formatted_date.length()-1);
        }

        // Create record row
        children.push_back(
            hbox({
                text(formatted_date) | size(WIDTH, EQUAL, 20),
                text(record.getName()) | size(WIDTH, EQUAL, 20),
                text(amount) | 
                    size(WIDTH, EQUAL, 15) | 
                    color(record.getType() == EARNING ? Color::Green : Color::Red) |
                    align_right,
                text(record.getCategory()) | size(WIDTH, EQUAL, 15),
                text(record.getType() == EARNING ? "Income" : "Expense") | 
                    size(WIDTH, EQUAL, 10) |
                    color(record.getType() == EARNING ? Color::Green : Color::Red)
            }) | flex
        );
    }

    if (collection->empty()) {
        children.push_back(
            vbox({
                text("No records found.") | dim | center,
                text("Add your first transaction using the 'Add Expense' menu.") | dim | center
            })
        );
    }

    return vbox({
        text("Transaction History") | bold | center,
        separator(),
        vbox(std::move(children)) | yframe | flex
    }) | flex | border;
}
