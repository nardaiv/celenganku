#include "ui.hpp"
#include <iomanip>
#include <sstream>
#include <algorithm>

ViewExpenses::ViewExpenses(std::shared_ptr<std::vector<Record>> collection) : 
    collection(collection),
    total_income(0),
    total_expenses(0),
    balance(0),
    sort_by_date(true),
    sort_button_clicked(false) {
    
    // Initialize sort button
    sort_button_ = Button("Sort by: Date", [this] {
        sort_by_date = !sort_by_date;
        sort_button_clicked = true;
        // Recreate button with new label
        sort_button_ = Button(
            "Sort by: " + std::string(sort_by_date ? "Date" : "Name"),
            [this] {
                sort_by_date = !sort_by_date;
                sort_button_clicked = true;
            }
        );
    });

    // Initialize cached elements
    updateHeaders();
    calculateTotals();
    updateSummary();
    updateRecordRows();
}

Component ViewExpenses::GetComponent() {
    return Container::Vertical({
        sort_button_
    });
}

void ViewExpenses::calculateTotals() {
    total_income = 0;
    total_expenses = 0;
    
    for(const auto& record : *collection) {
        if(record.getType() == EARNING) {
            total_income += record.getAmount();
        } else {
            total_expenses += record.getAmount();
        }
    }
    
    balance = total_income - total_expenses;
}

void ViewExpenses::updateHeaders() {
    table_headers = Elements();
    table_headers.push_back(
        hbox({
            text("Date") | size(WIDTH, EQUAL, 20) | bold,
            text("Name") | size(WIDTH, EQUAL, 20) | bold,
            text("Amount") | size(WIDTH, EQUAL, 15) | bold,
            text("Category") | size(WIDTH, EQUAL, 15) | bold,
            text("Type") | size(WIDTH, EQUAL, 10) | bold
        })
    );
}

void ViewExpenses::updateSummary() {
    summary_section = Elements();
    summary_section.push_back(
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
                text(std::to_string(balance)) | 
                    color(balance >= 0 ? Color::Green : Color::Red)
            })
        }) | border
    );
}

void ViewExpenses::sortRecords(std::vector<Record>& records) {
    if (sort_by_date) {
        // Sort by date (newest first)
        std::sort(records.begin(), records.end(), 
            [](const Record& a, const Record& b) {
                return a.getDate() > b.getDate();
            });
    } else {
        // Sort alphabetically by name
        std::sort(records.begin(), records.end(), 
            [](const Record& a, const Record& b) {
                return a.getName() < b.getName();
            });
    }
}

void ViewExpenses::updateRecordRows() {
    record_rows = Elements();
    
    // Create a copy of records for sorting
    std::vector<Record> sorted_records = *collection;
    
    // Sort records based on current sort mode
    sortRecords(sorted_records);
    
    // Recreate button with updated label
    sort_button_ = Button(
        "Sort by: " + std::string(sort_by_date ? "Date" : "Name"),
        [this] {
            sort_by_date = !sort_by_date;
            sort_button_clicked = true;
        }
    );
    
    if (sorted_records.empty()) {
        record_rows.push_back(
            vbox({
                text("No records found.") | dim | center,
                text("Add your first transaction using the 'Add Expense' menu.") | dim | center
            })
        );
        return;
    }

    for(const auto& record : sorted_records) {
        // Format amount
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

        record_rows.push_back(
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
}

Element ViewExpenses::Render() {
    // Check if sort button was clicked
    if (sort_button_clicked) {
        updateRecordRows();
        sort_button_clicked = false;
    }

    return vbox({
        // Sort button at the top
        hbox({
            text("") | flex,
            sort_button_->Render(),
            text("") | flex
        }),
        separator(),
        
        // Summary section
        vbox(summary_section),
        separator(),
        
        // Table headers
        vbox(table_headers),
        separator(),
        
        // Record rows in a scrollable container
        vbox(record_rows) | yframe | flex
    }) | flex | border;
}

void ViewExpenses::Update() {
    calculateTotals();
    updateSummary();
    updateRecordRows();
}
