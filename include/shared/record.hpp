#ifndef RECORD_HPP
#define RECORD_HPP

#include <string>
#include <ctime>

class Record
{
private:
    int id;
    std::string name;
    std::string category;
    int amount;
    time_t date;
    
public:
    Record(std::string name, int amount);

    // getter
    void getId();
    void getName();
    void getCategory();
    void getAmount();
    void getDate();

    // setter
    void setName(std::string name);
    void setCategory(std::string category);
    void setAmount(int amount);
    void setDate(time_t date);    

    ~Record();
};

#endif