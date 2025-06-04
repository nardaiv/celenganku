#include "ui.hpp"
#include <iomanip>
#include <sstream>
#include <algorithm>

Home::Home(ExpenseTracker& collection) : 
    collection(collection),
    total_income(0),
    total_expenses(0),
    balance(0) {
    calculateTotals();
    updateSummary();
    updateRecentTransactions();
}

Component Home::GetComponent() {
    return Container::Vertical({});
}

void Home::calculateTotals() {
    total_income = 0;
    total_expenses = 0;
    
    auto records = collection.getCollection();
    for(const auto& record : *records) {
        if(record.getType() == EARNING) {
            total_income += record.getAmount();
        } else {
            total_expenses += record.getAmount();
        }
    }
    
    balance = total_income - total_expenses;
}

void Home::updateSummary() {
    summary_section = Elements();
    
    // Create a more visually appealing summary box
    summary_section.push_back(
        vbox({
            text("Financial Overview") | bold | center,
            separator(),
            hbox({
                vbox({
                    text("Total Income") | dim,
                    text("Rp " + std::to_string(total_income)) | color(Color::Green) | bold
                }) | flex,
                separator(),
                vbox({
                    text("Total Expenses") | dim,
                    text("Rp " + std::to_string(total_expenses)) | color(Color::Red) | bold
                }) | flex,
                separator(),
                vbox({
                    text("Current Balance") | dim,
                    text("Rp " + std::to_string(balance)) | 
                        color(balance >= 0 ? Color::Green : Color::Red) | bold
                }) | flex
            }) | flex
        }) | border
    );
}

void Home::updateRecentTransactions() {
    record_rows = Elements();
    
    auto records = collection.getCollection();
    
    // Sort records by date (newest first)
    std::vector<Record> sorted_records = *records;
    std::sort(sorted_records.begin(), sorted_records.end(), 
        [](const Record& a, const Record& b) {
            return a.getDate() > b.getDate();
        });
    
    // Take only the 5 most recent transactions
    size_t count = 0;
    const size_t MAX_RECENT = 5;
    
    // Add header for recent transactions
    record_rows.push_back(text("Recent Transactions") | bold | center);
    record_rows.push_back(separator());
    
    // Add column headers
    record_rows.push_back(
        hbox({
            text("Date") | size(WIDTH, EQUAL, 20) | dim,
            text("Name") | size(WIDTH, EQUAL, 20) | dim,
            text("Amount") | size(WIDTH, EQUAL, 15) | dim,
            text("Category") | size(WIDTH, EQUAL, 15) | dim
        })
    );
    record_rows.push_back(separator());
    
    if (sorted_records.empty()) {
        record_rows.push_back(
            vbox({
                text("No transactions yet") | dim | center,
                text("Add your first transaction using the 'Add Expense' menu") | dim | center
            })
        );
    } else {
        for(const auto& record : sorted_records) {
            if (count >= MAX_RECENT) break;
            
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
                    text(record.getCategory()) | size(WIDTH, EQUAL, 15)
                })
            );
            
            count++;
        }
    }
}

Element Home::Render() {
    // Update data before rendering
    calculateTotals();
    updateSummary();
    updateRecentTransactions();
    
    return vbox({
        // Title
        text("Welcome to CelenganKu") | bold | center,
        text("Your Personal Finance Manager") | dim | center,
        separator(),
        
        // Financial summary
        vbox(summary_section) | flex,
        separator(),
        
        // Recent transactions
        vbox(record_rows) | flex
    }) | border | flex;
}
