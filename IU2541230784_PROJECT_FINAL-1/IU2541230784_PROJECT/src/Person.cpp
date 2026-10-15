#include "Person.h"
#include "Utils.h"
#include <stdexcept>

Person::Person() : id(0), name("") {}
Person::Person(int id, const string& name) : id(id) { setName(name); }
Person::~Person() {}
void Person::setName(const string& n) {
    if (!Utils::isValidName(n)) throw invalid_argument("Name must be 2-40 letters/spaces");
    name = n;
}
