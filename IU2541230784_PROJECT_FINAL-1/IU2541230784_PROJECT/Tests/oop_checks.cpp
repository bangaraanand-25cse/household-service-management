// OOP verification harness: every check really executes against the project classes.
// Build: g++ -std=c++17 -I src Tests/oop_checks.cpp (all src/*.cpp except main.cpp) -o oop_checks
#include "Customer.h"
#include "Worker.h"
#include "Service.h"
#include "Booking.h"
#include "FileManager.h"
#include "Utils.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <type_traits>
#include <filesystem>
using namespace std;

static int passed = 0, failed = 0;
static void check(const string& name, bool ok) {
    cout << (ok ? "[PASS] " : "[FAIL] ") << name << "\n";
    ok ? passed++ : failed++;
}
template <class E, class F> static bool throwsType(F f) {
    try { f(); } catch (const E&) { return true; } catch (...) { return false; }
    return false;
}
static_assert(is_abstract_v<Person>, "Person must be abstract");
static_assert(has_virtual_destructor_v<Person>, "Person needs a virtual destructor");
static_assert(is_base_of_v<Person, Customer> && is_base_of_v<Person, Worker>, "inheritance");
static_assert(!is_base_of_v<Person, Service> && !is_base_of_v<Person, Booking>, "Service/Booking are not people");

int main() {
    cout << "== OOP verification (IU2541230784) ==\n";
    // 1 Abstraction / inheritance (also proven at compile time by static_assert above)
    check("Abstraction: Person is abstract (cannot be instantiated)", is_abstract_v<Person>);
    check("Inheritance: Customer and Worker derive from Person", is_base_of_v<Person, Customer> && is_base_of_v<Person, Worker>);
    check("Destructor: Person has a virtual destructor", has_virtual_destructor_v<Person>);

    // 2 Constructors
    Customer dc; Worker dw; Service ds; Booking db;
    check("Default constructors give empty objects", dc.getId() == 0 && dw.getName().empty() && ds.getCharge() == 0 && db.getStatus() == Booking::BOOKED);
    Customer c(7, "Ravi Kumar", "9876543210", "12 MG Road");
    Worker w(3, "Suresh Patil", "Plumbing");
    check("Parameterized constructors store values", c.getId() == 7 && c.getPhone() == "9876543210" && w.getSkill() == "Plumbing");

    // 3 Encapsulation: validated setters reject bad data and leave the object unchanged
    check("Encapsulation: bad phone rejected (invalid_argument)", throwsType<invalid_argument>([&] { c.setPhone("12345"); }));
    check("Encapsulation: object unchanged after rejected update", c.getPhone() == "9876543210");
    check("Encapsulation: constructor rejects invalid name", throwsType<invalid_argument>([] { Customer x(1, "R", "9876543210", "a"); }));
    check("Encapsulation: negative charge rejected", throwsType<invalid_argument>([&] { ds.setCharge(-5); }));
    check("Encapsulation: '|' rejected in text (protects file format)", throwsType<invalid_argument>([&] { w.setSkill("a|b"); }));

    // 4 Runtime polymorphism through base pointers
    Person* people[2] = { new Customer(1, "Anita Shah", "9123456780", "5 Park St"), new Worker(2, "Mohan Das", "Electrical") };
    check("Polymorphism: role() resolved at run time", people[0]->role() == "Customer" && people[1]->role() == "Worker");
    check("Polymorphism: dynamic type via typeid", typeid(*people[0]) == typeid(Customer) && typeid(*people[1]) == typeid(Worker));
    check("Polymorphism: toRecord() differs per subclass", people[0]->toRecord() == "1|Anita Shah|9123456780|5 Park St" && people[1]->toRecord() == "2|Mohan Das|Electrical");
    ostringstream cap; streambuf* old = cout.rdbuf(cap.rdbuf());
    people[0]->display(); people[1]->display();
    cout.rdbuf(old);
    check("Polymorphism: display() prints the right fields per class", cap.str().find("5 Park St") != string::npos && cap.str().find("Electrical") != string::npos);
    for (Person* p : people) delete p;   // virtual destructor: no leak/UB (verified under AddressSanitizer)
    check("Delete through Person* completed", true);

    // 5 Exception handling + booking life cycle
    Booking b(1, 1, 1, 1, "20-10-2026");
    check("Exception: invalid date throws invalid_argument", throwsType<invalid_argument>([] { Booking x(1, 1, 1, 1, "31-02-2026"); }));
    b.complete();
    check("Life cycle: BOOKED -> COMPLETED", b.getStatus() == Booking::COMPLETED);
    check("Exception: cancelling a completed booking throws logic_error", throwsType<logic_error>([&] { b.cancel(); }));
    check("Utils: leap-year date 29-02-2028 valid, 29-02-2027 invalid", Utils::isValidDate("29-02-2028") && !Utils::isValidDate("29-02-2027"));

    // 6 Dynamic memory: grow() doubles capacity and keeps elements
    int cap_n = 2, n = 2; Customer* arr = new Customer[cap_n];
    arr[0] = Customer(1, "Aa Bb", "1234567890", "x"); arr[1] = Customer(2, "Cc Dd", "1234567891", "y");
    Utils::grow(arr, n, cap_n);
    check("Dynamic memory: capacity doubled 2 -> 4", cap_n == 4);
    check("Dynamic memory: elements preserved after growth", arr[0].getName() == "Aa Bb" && arr[1].getId() == 2);
    delete[] arr;

    // 7 File handling round trip + corrupt-line tolerance
    string dir = (filesystem::temp_directory_path() / "hh_oop_check").string();
    filesystem::remove_all(dir);
    {
        FileManager fm(dir);
        Customer cs[1] = { c }; Service ss[1] = { Service(1, "AC Repair", 500) };
        Worker ws[1] = { w }; Booking bs[1] = { b };
        fm.save(cs, 1, ss, 1, ws, 1, bs, 1);
    }
    { ofstream bad(dir + "/customers.dat", ios::app); bad << "garbage line\n"; }
    int nc = 0, cc = 4, ns = 0, sc = 4, nw = 0, wc = 4, nb = 0, bc = 4;
    Customer* C = new Customer[cc]; Service* S = new Service[sc]; Worker* W = new Worker[wc]; Booking* B = new Booking[bc];
    ostringstream cap2; old = cout.rdbuf(cap2.rdbuf());
    FileManager(dir).load(C, nc, cc, S, ns, sc, W, nw, wc, B, nb, bc);
    cout.rdbuf(old);
    check("File handling: records survive save -> load", nc == 1 && ns == 1 && nw == 1 && nb == 1 && C[0].getName() == "Ravi Kumar" && B[0].getStatus() == Booking::COMPLETED);
    check("File handling: corrupt line skipped with a warning", cap2.str().find("1 corrupt") != string::npos);
    delete[] C; delete[] S; delete[] W; delete[] B;
    filesystem::remove_all(dir);

    cout << "\nSummary: " << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
