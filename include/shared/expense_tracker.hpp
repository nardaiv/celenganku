#ifndef EXPENSE_TRACKER_HPP
#define EXPENSE_TRACKER_HPP

#include "record.hpp"
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>

class ExpenseTracker
{
private:
    std::shared_ptr<std::vector<Record>> Collection;
    // summary
    int balance;
    int total_income;
    int total_expenses;


public:
    ExpenseTracker();

    void addRecord(Record newRecord);
    void removeRecord(int id);

    //TODO : add search and filter methods
    void setCollection(std::shared_ptr<std::vector<Record>> collection);

    
    int getBalance();
    int getTotalIncome();
    int getTotalExpenses();
    std::shared_ptr<std::vector<Record>> getCollection();
    virtual ~ExpenseTracker();
};

#endif