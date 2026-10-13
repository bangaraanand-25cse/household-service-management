#include "Customer.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

Customer::Customer() : Person(), phone(""), address("") {}
Customer::Customer(int id, const string& name, const string& phone, const string& address)
    : Person(id, name) { setPhone(phone); setAddress(address); }
Customer::~Customer() {}
void Customer::setPhone(const string& p) {
    if (!Utils::isValidPhone(p)) throw invalid_argument("Phone must be exactly 10 digits");
    phone = p;
}
void Customer::setAddress(const string& a) {
    if (!Utils::isValidText(a)) throw invalid_argument("Address cannot be empty or contain '|'");
    address = a;
}
void Customer::display() const {
    cout << left << setw(5) << id << setw(22) << name << setw(13) << phone << address << "\n";
}
string Customer::toRecord() const {
    return to_string(id) + "|" + name + "|" + phone + "|" + address;
}
