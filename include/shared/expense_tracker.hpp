#ifndef EXPENSE_TRACKER_HPP
#define EXPENSE_TRACKER_HPP

#include "record.hpp"
#include <vector>
#include <fstream>
#include <iostream>

class ExpenseTracker
{
private:
    std::vector<Record> Collection; 
    int balance;

public:
    ExpenseTracker();

    void addRecord(Record newRecord);
    void removeRecord(int id);

    //TODO : add search and filter methods
    

    virtual ~ExpenseTracker();

};

#endif