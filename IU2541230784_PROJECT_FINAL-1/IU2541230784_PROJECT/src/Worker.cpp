#include "Worker.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

Worker::Worker() : Person(), skill("") {}
Worker::Worker(int id, const string& name, const string& skill) : Person(id, name) { setSkill(skill); }
Worker::~Worker() {}
void Worker::setSkill(const string& s) {
    if (!Utils::isValidText(s)) throw invalid_argument("Skill cannot be empty or contain '|'");
    skill = s;
}
void Worker::display() const {
    cout << left << setw(5) << id << setw(22) << name << skill << "\n";
}
string Worker::toRecord() const { return to_string(id) + "|" + name + "|" + skill; }
