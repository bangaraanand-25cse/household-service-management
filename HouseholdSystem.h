#ifndef HOUSEHOLDSYSTEM_H
#define HOUSEHOLDSYSTEM_H
#include "Customer.h"
#include "Worker.h"
#include "Service.h"
#include "Booking.h"
#include "FileManager.h"

// Controller class: owns the dynamic arrays and implements all menu operations.
class HouseholdSystem {
private:
    Customer* customers; int custCount, custCap;
    Service*  services;  int servCount, servCap;
    Worker*   workers;   int workCount, workCap;
    Booking*  bookings;  int bookCount, bookCap;
    FileManager fm;

    int findCustomer(int id) const;
    int findService(int id) const;
    int findWorker(int id) const;
    int findBooking(int id) const;
    int nextId(int kind) const;                 // 0=cust 1=serv 2=work 3=book
    bool workerBusy(int workerId, const string& date) const;
    bool inActiveBooking(char kind, int id) const;
    void clearAll();

    // menu helpers
    void addMenu();      void displayMenu();   void searchMenu();
    void updateMenu();   void deleteMenu();    void transactionMenu();
    void report() const; void saveLoadMenu();
public:
    HouseholdSystem();
    ~HouseholdSystem();
    HouseholdSystem(const HouseholdSystem&) = delete;            // owns raw memory
    HouseholdSystem& operator=(const HouseholdSystem&) = delete;

    void addCustomer();  void addService();  void addWorker();
    void bookService();  void cancelBooking(); void completeService();
    void saveData();     void loadData();
    void run();                                  // main menu loop
};
#endif
