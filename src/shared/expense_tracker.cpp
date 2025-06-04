#include "expense_tracker.hpp"

// constructor
ExpenseTracker::ExpenseTracker() : Collection(std::make_shared<std::vector<Record>>()) {
    std::cout << "ExpenseTracker initialized." << std::endl;
}

void ExpenseTracker::addRecord(Record newRecord){
    Collection->push_back(newRecord);
    // this->save();
    if (newRecord.getType() == EARNING){
        total_income += newRecord.getAmount();
        balance += newRecord.getAmount();
    } else {
        total_expenses += newRecord.getAmount();
        balance -= newRecord.getAmount();
    }
}

void ExpenseTracker::removeRecord(int id){
    for (int i = 0; i < Collection->size(); i++){
        if ((*Collection)[i].getId() == id){
            Collection->erase(Collection->begin() + i);
            break;
        }
    }
    // this->save();
}

// void ExpenseTracker::save(){
//     std::ofstream outfile("data.bin", std::ios::binary);
        
//     size_t size = this->Collection.size();
//     outfile.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    
//     // Write the vector data
//     outfile.write(reinterpret_cast<const char*>(this->Collection.data()), size * sizeof(Record));
//     outfile.close();
// }

ExpenseTracker::~ExpenseTracker() = default;

std::shared_ptr<std::vector<Record>> ExpenseTracker::getCollection(){
    return Collection;
}

void ExpenseTracker::setCollection(std::shared_ptr<std::vector<Record>> collection){
    this->Collection = collection;
}

int ExpenseTracker::getBalance(){
    return balance;
}

int ExpenseTracker::getTotalIncome(){
    return total_income;
}

int ExpenseTracker::getTotalExpenses(){
    return total_expenses;
}