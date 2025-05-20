#ifndef EXPENSE_TRACKER_HPP
#define EXPENSE_TRACKER_HPP

#include "record.hpp"

#include <vector>

class ExpenseTracker
{
private:
    std::vector<Record> Collection; 
    int balance;
    
    void save();

public:
    ExpenseTracker();

    void addRecord();
    void removeRecord();

    

    ~ExpenseTracker();

};

#endif