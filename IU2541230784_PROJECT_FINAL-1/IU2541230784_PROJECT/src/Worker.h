#ifndef WORKER_H
#define WORKER_H
#include "Person.h"

class Worker : public Person {
private:
    string skill;     // e.g. Plumbing, Electrical, Cleaning
public:
    Worker();
    Worker(int id, const string& name, const string& skill);
    ~Worker();
    string getSkill() const { return skill; }
    void setSkill(const string& s);
    void display() const override;
    string toRecord() const override;
    string role() const override { return "Worker"; }
};
#endif
