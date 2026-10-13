#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <string>
#include "Customer.h"
#include "Worker.h"
#include "Service.h"
#include "Booking.h"
using namespace std;

// Saves and loads each record type to a text file (one record per line, fields separated by '|').
class FileManager {
private:
    string dir;
public:
    FileManager(const string& dir = "Data");
    ~FileManager();
    void save(const Customer* c, int nc, const Service* s, int ns,
              const Worker* w, int nw, const Booking* b, int nb) const;
    // load: fills the arrays (growing them if needed). Missing files are fine (first run).
    void load(Customer*& c, int& nc, int& cc, Service*& s, int& ns, int& sc,
              Worker*& w, int& nw, int& wc, Booking*& b, int& nb, int& bc) const;
};
#endif
