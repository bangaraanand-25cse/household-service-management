#ifndef CUSTOMER_H
#define CUSTOMER_H
#include "Person.h"

class Customer : public Person {
private:
    string phone;
    string address;
public:
    Customer();
    Customer(int id, const string& name, const string& phone, const string& address);
    ~Customer();
    string getPhone() const { return phone; }
    string getAddress() const { return address; }
    void setPhone(const string& p);
    void setAddress(const string& a);
    void display() const override;
    string toRecord() const override;
    string role() const override { return "Customer"; }
};
#endif
