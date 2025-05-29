#include "record.hpp"

Record::Record(std::string name, RecordType type, std::string category, int amount, time_t date) 
    : name(name), type(type), category(category), amount(amount), date(date){

    // // set date to the current time
    // this->date = std::time(0);

    // // set id to random 
    // std::srand(time(0));
    // this->id = std::rand() % 1000000;
}

// getter
int Record::getId() const{
    return id;
}

std::string Record::getName() const{
    return name;
}

std::string Record::getCategory() const{
    return category;
}

int Record::getAmount() const{
    return amount;
}

time_t Record::getDate() const{
    return date;
}

// setter
void Record::setName(std::string name){
    this->name = name;
}

void Record::setCategory(std::string category){
    this->category = category;
}

void Record::setAmount(int amount){
    this->amount = amount;  
}

void Record::setDate(time_t date){
    this->date = date;
}    

Record::~Record() = default;