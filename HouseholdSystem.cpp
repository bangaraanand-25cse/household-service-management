#include "HouseholdSystem.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <map>

static const int INITIAL_CAP = 4;   // small on purpose: shows the arrays growing

HouseholdSystem::HouseholdSystem()
    : customers(new Customer[INITIAL_CAP]), custCount(0), custCap(INITIAL_CAP),
      services(new Service[INITIAL_CAP]),   servCount(0), servCap(INITIAL_CAP),
      workers(new Worker[INITIAL_CAP]),     workCount(0), workCap(INITIAL_CAP),
      bookings(new Booking[INITIAL_CAP]),   bookCount(0), bookCap(INITIAL_CAP), fm("Data") {}

HouseholdSystem::~HouseholdSystem() {
    delete[] customers; delete[] services; delete[] workers; delete[] bookings;
}

// ---------- lookups ----------
int HouseholdSystem::findCustomer(int id) const { for (int i = 0; i < custCount; i++) if (customers[i].getId() == id) return i; return -1; }
int HouseholdSystem::findService(int id)  const { for (int i = 0; i < servCount; i++) if (services[i].getId() == id) return i; return -1; }
int HouseholdSystem::findWorker(int id)   const { for (int i = 0; i < workCount; i++) if (workers[i].getId() == id) return i; return -1; }
int HouseholdSystem::findBooking(int id)  const { for (int i = 0; i < bookCount; i++) if (bookings[i].getId() == id) return i; return -1; }

int HouseholdSystem::nextId(int kind) const {
    int m = 0;
    if (kind == 0) for (int i = 0; i < custCount; i++) m = max(m, customers[i].getId());
    if (kind == 1) for (int i = 0; i < servCount; i++) m = max(m, services[i].getId());
    if (kind == 2) for (int i = 0; i < workCount; i++) m = max(m, workers[i].getId());
    if (kind == 3) for (int i = 0; i < bookCount; i++) m = max(m, bookings[i].getId());
    return m + 1;
}
bool HouseholdSystem::workerBusy(int workerId, const string& date) const {
    for (int i = 0; i < bookCount; i++)
        if (bookings[i].getWorkerId() == workerId && bookings[i].getDate() == date &&
            bookings[i].getStatus() == Booking::BOOKED) return true;
    return false;
}
bool HouseholdSystem::inActiveBooking(char kind, int id) const {
    for (int i = 0; i < bookCount; i++) {
        if (bookings[i].getStatus() != Booking::BOOKED) continue;
        if (kind == 'c' && bookings[i].getCustomerId() == id) return true;
        if (kind == 's' && bookings[i].getServiceId() == id) return true;
        if (kind == 'w' && bookings[i].getWorkerId() == id) return true;
    }
    return false;
}
void HouseholdSystem::clearAll() { custCount = servCount = workCount = bookCount = 0; }

// ---------- add ----------
void HouseholdSystem::addCustomer() {
    try {
        string name = Utils::readLine("Name: ");
        string phone = Utils::readLine("Phone (10 digits): ");
        string addr = Utils::readLine("Address: ");
        Customer c(nextId(0), name, phone, addr);       // constructor validates
        if (custCount == custCap) Utils::grow(customers, custCount, custCap);
        customers[custCount++] = c;
        cout << "Customer added with ID " << c.getId() << ".\n";
    } catch (const invalid_argument& e) { cout << "Error: " << e.what() << "\n"; }
}
void HouseholdSystem::addService() {
    try {
        string name = Utils::readLine("Service name: ");
        double ch = Utils::readDouble("Charge (Rs.): ");
        Service s(nextId(1), name, ch);
        if (servCount == servCap) Utils::grow(services, servCount, servCap);
        services[servCount++] = s;
        cout << "Service added with ID " << s.getId() << ".\n";
    } catch (const invalid_argument& e) { cout << "Error: " << e.what() << "\n"; }
}
void HouseholdSystem::addWorker() {
    try {
        string name = Utils::readLine("Worker name: ");
        string skill = Utils::readLine("Skill: ");
        Worker w(nextId(2), name, skill);
        if (workCount == workCap) Utils::grow(workers, workCount, workCap);
        workers[workCount++] = w;
        cout << "Worker added with ID " << w.getId() << ".\n";
    } catch (const invalid_argument& e) { cout << "Error: " << e.what() << "\n"; }
}
void HouseholdSystem::addMenu() {
    cout << "\n-- Add Record --\n1. Customer\n2. Service\n3. Worker\n";
    int c = Utils::readInt("Choice: ");
    if (c == 1) addCustomer(); else if (c == 2) addService(); else if (c == 3) addWorker();
    else cout << "Invalid choice.\n";
}

// ---------- display ----------
void HouseholdSystem::displayMenu() {
    cout << "\n-- Display Records --\n1. Customers\n2. Services\n3. Workers\n4. Bookings\n5. Everyone (polymorphic)\n";
    int c = Utils::readInt("Choice: ");
    if (c == 1 || c == 5) {
        cout << "\nCUSTOMERS\n" << left << setw(5) << "ID" << setw(22) << "Name" << setw(13) << "Phone" << "Address\n";
        for (int i = 0; i < custCount; i++) customers[i].display();
        if (!custCount) cout << "(none)\n";
    }
    if (c == 5) {
        cout << "\nPEOPLE VIA Person* (runtime polymorphism)\n";
        for (int i = 0; i < custCount; i++) { Person* p = &customers[i]; cout << p->role() << ": "; p->display(); }
        for (int i = 0; i < workCount; i++) { Person* p = &workers[i];   cout << p->role() << ": "; p->display(); }
    }
    if (c == 2) {
        cout << "\nSERVICES\n" << left << setw(5) << "ID" << setw(24) << "Name" << "Charge\n";
        for (int i = 0; i < servCount; i++) services[i].display();
        if (!servCount) cout << "(none)\n";
    }
    if (c == 3) {
        cout << "\nWORKERS\n" << left << setw(5) << "ID" << setw(22) << "Name" << "Skill\n";
        for (int i = 0; i < workCount; i++) workers[i].display();
        if (!workCount) cout << "(none)\n";
    }
    if (c == 4) {
        cout << "\nBOOKINGS\n" << left << setw(5) << "ID" << setw(7) << "Cust" << setw(7) << "Serv"
             << setw(8) << "Worker" << setw(13) << "Date" << "Status\n";
        for (int i = 0; i < bookCount; i++) bookings[i].display();
        if (!bookCount) cout << "(none)\n";
    }
    if (c < 1 || c > 5) cout << "Invalid choice.\n";
}

// ---------- search ----------
void HouseholdSystem::searchMenu() {
    cout << "\n-- Search --\n1. Customer by ID\n2. Customer by name\n3. Service by ID\n4. Worker by ID\n5. Worker by skill\n6. Booking by ID\n";
    int c = Utils::readInt("Choice: ");
    if (c == 1) {
        int i = findCustomer(Utils::readInt("Customer ID: "));
        if (i < 0) cout << "Not found.\n"; else customers[i].display();
    } else if (c == 2) {
        string key = Utils::toLower(Utils::readLine("Name contains: ")); int n = 0;
        for (int i = 0; i < custCount; i++)
            if (Utils::toLower(customers[i].getName()).find(key) != string::npos) { customers[i].display(); n++; }
        if (!n) cout << "Not found.\n";
    } else if (c == 3) {
        int i = findService(Utils::readInt("Service ID: "));
        if (i < 0) cout << "Not found.\n"; else services[i].display();
    } else if (c == 4) {
        int i = findWorker(Utils::readInt("Worker ID: "));
        if (i < 0) cout << "Not found.\n"; else workers[i].display();
    } else if (c == 5) {
        string key = Utils::toLower(Utils::readLine("Skill contains: ")); int n = 0;
        for (int i = 0; i < workCount; i++)
            if (Utils::toLower(workers[i].getSkill()).find(key) != string::npos) { workers[i].display(); n++; }
        if (!n) cout << "Not found.\n";
    } else if (c == 6) {
        int i = findBooking(Utils::readInt("Booking ID: "));
        if (i < 0) cout << "Not found.\n"; else bookings[i].display();
    } else cout << "Invalid choice.\n";
}

// ---------- update ----------
void HouseholdSystem::updateMenu() {
    cout << "\n-- Update --\n1. Customer\n2. Service charge\n3. Worker skill\n";
    int c = Utils::readInt("Choice: ");
    try {
        if (c == 1) {
            int i = findCustomer(Utils::readInt("Customer ID: "));
            if (i < 0) { cout << "Not found.\n"; return; }
            customers[i].setPhone(Utils::readLine("New phone (10 digits): "));
            customers[i].setAddress(Utils::readLine("New address: "));
            cout << "Customer updated.\n";
        } else if (c == 2) {
            int i = findService(Utils::readInt("Service ID: "));
            if (i < 0) { cout << "Not found.\n"; return; }
            services[i].setCharge(Utils::readDouble("New charge: "));
            cout << "Service updated.\n";
        } else if (c == 3) {
            int i = findWorker(Utils::readInt("Worker ID: "));
            if (i < 0) { cout << "Not found.\n"; return; }
            workers[i].setSkill(Utils::readLine("New skill: "));
            cout << "Worker updated.\n";
        } else cout << "Invalid choice.\n";
    } catch (const invalid_argument& e) { cout << "Error: " << e.what() << " (record unchanged)\n"; }
}

// ---------- delete ----------
void HouseholdSystem::deleteMenu() {
    cout << "\n-- Delete --\n1. Customer\n2. Service\n3. Worker\n";
    int c = Utils::readInt("Choice: ");
    int id, i;
    if (c == 1) {
        id = Utils::readInt("Customer ID: "); i = findCustomer(id);
        if (i < 0) { cout << "Not found.\n"; return; }
        if (inActiveBooking('c', id)) { cout << "Cannot delete: customer has an active booking.\n"; return; }
        for (int k = i; k < custCount - 1; k++) customers[k] = customers[k + 1];
        custCount--; cout << "Customer deleted.\n";
    } else if (c == 2) {
        id = Utils::readInt("Service ID: "); i = findService(id);
        if (i < 0) { cout << "Not found.\n"; return; }
        if (inActiveBooking('s', id)) { cout << "Cannot delete: service has an active booking.\n"; return; }
        for (int k = i; k < servCount - 1; k++) services[k] = services[k + 1];
        servCount--; cout << "Service deleted.\n";
    } else if (c == 3) {
        id = Utils::readInt("Worker ID: "); i = findWorker(id);
        if (i < 0) { cout << "Not found.\n"; return; }
        if (inActiveBooking('w', id)) { cout << "Cannot delete: worker has an active booking.\n"; return; }
        for (int k = i; k < workCount - 1; k++) workers[k] = workers[k + 1];
        workCount--; cout << "Worker deleted.\n";
    } else cout << "Invalid choice.\n";
}

// ---------- main transaction ----------
void HouseholdSystem::bookService() {
    try {
        int cid = Utils::readInt("Customer ID: ");
        if (findCustomer(cid) < 0) throw invalid_argument("Customer not found");
        int sid = Utils::readInt("Service ID: ");
        if (findService(sid) < 0) throw invalid_argument("Service not found");
        int wid = Utils::readInt("Worker ID: ");
        if (findWorker(wid) < 0) throw invalid_argument("Worker not found");
        string date = Utils::readLine("Date (DD-MM-YYYY): ");
        Booking b(nextId(3), cid, sid, wid, date);          // validates date
        if (workerBusy(wid, date)) throw invalid_argument("Worker already booked on that date");
        b.book();
        if (bookCount == bookCap) Utils::grow(bookings, bookCount, bookCap);
        bookings[bookCount++] = b;
        cout << "Service booked. Booking ID " << b.getId() << ", charge Rs. "
             << fixed << setprecision(2) << services[findService(sid)].getCharge() << ".\n";
    } catch (const invalid_argument& e) { cout << "Error: " << e.what() << "\n"; }
}
void HouseholdSystem::cancelBooking() {
    try {
        int i = findBooking(Utils::readInt("Booking ID: "));
        if (i < 0) throw invalid_argument("Booking not found");
        bookings[i].cancel();
        cout << "Booking cancelled.\n";
    } catch (const exception& e) { cout << "Error: " << e.what() << "\n"; }
}
void HouseholdSystem::completeService() {
    try {
        int i = findBooking(Utils::readInt("Booking ID: "));
        if (i < 0) throw invalid_argument("Booking not found");
        bookings[i].complete();
        int s = findService(bookings[i].getServiceId());
        cout << "Service completed. Bill: Rs. " << fixed << setprecision(2)
             << (s >= 0 ? services[s].getCharge() : 0.0) << "\n";
    } catch (const exception& e) { cout << "Error: " << e.what() << "\n"; }
}
void HouseholdSystem::transactionMenu() {
    cout << "\n-- Transaction --\n1. Book a service\n2. Cancel a booking\n3. Complete a service\n";
    int c = Utils::readInt("Choice: ");
    if (c == 1) bookService(); else if (c == 2) cancelBooking(); else if (c == 3) completeService();
    else cout << "Invalid choice.\n";
}

// ---------- report ----------
void HouseholdSystem::report() const {
    int nb = 0, nc = 0, nx = 0; double revenue = 0;
    map<string, double> perService;
    map<string, int> perWorker;                     // completed jobs per worker
    for (int i = 0; i < bookCount; i++) {
        Booking::Status st = bookings[i].getStatus();
        if (st == Booking::BOOKED) nb++;
        else if (st == Booking::CANCELLED) nx++;
        else {
            nc++;
            int w = findWorker(bookings[i].getWorkerId());
            if (w >= 0) perWorker[workers[w].getName()]++;
            int s = findService(bookings[i].getServiceId());
            if (s >= 0) { revenue += services[s].getCharge(); perService[services[s].getName()] += services[s].getCharge(); }
        }
    }
    cout << "\n========== SUMMARY REPORT ==========\n"
         << "Customers: " << custCount << "   Services: " << servCount << "   Workers: " << workCount << "\n"
         << "Bookings : " << bookCount << " (Booked " << nb << ", Completed " << nc << ", Cancelled " << nx << ")\n"
         << "Total revenue (completed): Rs. " << fixed << setprecision(2) << revenue << "\n";
    for (const auto& kv : perService) cout << "  " << left << setw(22) << kv.first << "Rs. " << kv.second << "\n";
    if (!perWorker.empty()) {
        cout << "Completed jobs per worker:\n";
        for (const auto& kv : perWorker) cout << "  " << left << setw(22) << kv.first << kv.second << "\n";
    }
    cout << "====================================\n";
}

// ---------- save / load ----------
void HouseholdSystem::saveData() {
    try {
        fm.save(customers, custCount, services, servCount, workers, workCount, bookings, bookCount);
        cout << "Data saved.\n";
    } catch (const exception& e) { cout << "Save failed: " << e.what() << "\n"; }
}
void HouseholdSystem::loadData() {
    clearAll();
    try {
        fm.load(customers, custCount, custCap, services, servCount, servCap,
                workers, workCount, workCap, bookings, bookCount, bookCap);
        cout << "Data loaded: " << custCount << " customers, " << servCount << " services, "
             << workCount << " workers, " << bookCount << " bookings.\n";
    } catch (const exception& e) { cout << "Load failed: " << e.what() << "\n"; }
}
void HouseholdSystem::saveLoadMenu() {
    cout << "\n-- Save / Load --\n1. Save now\n2. Load from files (discards unsaved changes)\n";
    int c = Utils::readInt("Choice: ");
    if (c == 1) saveData(); else if (c == 2) loadData(); else cout << "Invalid choice.\n";
}

// ---------- main loop ----------
void HouseholdSystem::run() {
    loadData();                                  // START -> load existing records
    try {
        while (true) {
            cout << "\n===== HOUSEHOLD SERVICE MANAGEMENT =====\n"
                 << "1. Add Record\n2. Display Records\n3. Search Record\n4. Update Record\n"
                 << "5. Delete Record\n6. Main Transaction (Book/Cancel/Complete)\n7. Report\n"
                 << "8. Save / Load\n9. Exit\n";
            int c = Utils::readInt("Enter choice: ");
            switch (c) {
                case 1: addMenu(); break;
                case 2: displayMenu(); break;
                case 3: searchMenu(); break;
                case 4: updateMenu(); break;
                case 5: deleteMenu(); break;
                case 6: transactionMenu(); break;
                case 7: report(); break;
                case 8: saveLoadMenu(); break;
                case 9: saveData(); cout << "Goodbye!\n"; return;   // save updated records -> EXIT
                default: cout << "Invalid choice (1-9).\n";
            }
        }
    } catch (const runtime_error&) {             // input stream closed
        saveData();
        cout << "\nInput ended. Data saved. Goodbye!\n";
    }
}
