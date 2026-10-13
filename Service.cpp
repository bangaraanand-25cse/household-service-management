#include "Service.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

Service::Service() : serviceId(0), name(""), charge(0) {}
Service::Service(int id, const string& name, double charge) : serviceId(id) { setName(name); setCharge(charge); }
Service::~Service() {}
void Service::setName(const string& n) {
    if (!Utils::isValidText(n)) throw invalid_argument("Service name cannot be empty or contain '|'");
    name = n;
}
void Service::setCharge(double c) {
    if (c <= 0 || c > 1000000) throw invalid_argument("Charge must be between 1 and 1000000");
    charge = c;
}
void Service::display() const {
    cout << left << setw(5) << serviceId << setw(24) << name << "Rs. " << fixed << setprecision(2) << charge << "\n";
}
string Service::toRecord() const {
    return to_string(serviceId) + "|" + name + "|" + to_string(charge);
}
