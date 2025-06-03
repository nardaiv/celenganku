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
    int balance;

public:
    ExpenseTracker();

    void addRecord(Record newRecord);
    void removeRecord(int id);

    //TODO : add search and filter methods
    
    std::shared_ptr<std::vector<Record>> getCollection();
    virtual ~ExpenseTracker();
};

#endif