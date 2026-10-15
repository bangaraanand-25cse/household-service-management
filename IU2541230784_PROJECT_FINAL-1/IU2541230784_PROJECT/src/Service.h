#ifndef SERVICE_H
#define SERVICE_H
#include <string>
using namespace std;

class Service {
private:
    int serviceId;
    string name;
    double charge;
public:
    Service();
    Service(int id, const string& name, double charge);
    ~Service();
    int getId() const { return serviceId; }
    string getName() const { return name; }
    double getCharge() const { return charge; }
    void setName(const string& n);
    void setCharge(double c);
    void display() const;
    string toRecord() const;
};
#endif
