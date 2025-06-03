#ifndef RECORD_HPP
#define RECORD_HPP

#include "record_type.hpp"
#include <string>
#include <ctime>
#include <cstdlib>

class Record
{
private:
    int id;
    RecordType type;
    std::string name;
    std::string category;
    int amount;
    time_t date;
    
public:
    Record(std::string name, RecordType type, int id, std::string category, int amount, time_t date);

    // getter
    int getId() const;
    std::string getName() const;
    std::string getCategory() const;
    int getAmount() const;
    time_t getDate() const;
    RecordType getType() const;

    // setter
    void setName(std::string name);
    void setCategory(std::string category);
    void setAmount(int amount);
    void setDate(time_t date);    

    virtual ~Record();
};

#endif