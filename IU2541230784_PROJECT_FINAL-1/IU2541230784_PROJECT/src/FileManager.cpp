#include "FileManager.h"
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <iostream>

static vector<string> split(const string& line) {
    vector<string> parts;
    string item;
    stringstream ss(line);
    while (getline(ss, item, '|')) parts.push_back(item);
    return parts;
}

FileManager::FileManager(const string& d) : dir(d) {}
FileManager::~FileManager() {}

template <class T>
static void writeAll(const string& path, const T* arr, int n) {
    ofstream out(path);
    if (!out) throw runtime_error("Cannot write file: " + path);
    for (int i = 0; i < n; i++) out << arr[i].toRecord() << "\n";
}

void FileManager::save(const Customer* c, int nc, const Service* s, int ns,
                       const Worker* w, int nw, const Booking* b, int nb) const {
    filesystem::create_directories(dir);
    writeAll(dir + "/customers.dat", c, nc);
    writeAll(dir + "/services.dat", s, ns);
    writeAll(dir + "/workers.dat", w, nw);
    writeAll(dir + "/bookings.dat", b, nb);
}

void FileManager::load(Customer*& c, int& nc, int& cc, Service*& s, int& ns, int& sc,
                       Worker*& w, int& nw, int& wc, Booking*& b, int& nb, int& bc) const {
    nc = ns = nw = nb = 0;
    string line;
    int skipped = 0;
    { ifstream in(dir + "/customers.dat");
      while (getline(in, line)) {
        auto p = split(line);
        try {
            if (p.size() != 4) throw invalid_argument("bad field count");
            if (nc == cc) Utils::grow(c, nc, cc);
            c[nc++] = Customer(stoi(p[0]), p[1], p[2], p[3]);
        } catch (const exception&) { skipped++; }
      } }
    { ifstream in(dir + "/services.dat");
      while (getline(in, line)) {
        auto p = split(line);
        try {
            if (p.size() != 3) throw invalid_argument("bad field count");
            if (ns == sc) Utils::grow(s, ns, sc);
            s[ns++] = Service(stoi(p[0]), p[1], stod(p[2]));
        } catch (const exception&) { skipped++; }
      } }
    { ifstream in(dir + "/workers.dat");
      while (getline(in, line)) {
        auto p = split(line);
        try {
            if (p.size() != 3) throw invalid_argument("bad field count");
            if (nw == wc) Utils::grow(w, nw, wc);
            w[nw++] = Worker(stoi(p[0]), p[1], p[2]);
        } catch (const exception&) { skipped++; }
      } }
    { ifstream in(dir + "/bookings.dat");
      while (getline(in, line)) {
        auto p = split(line);
        try {
            if (p.size() != 6) throw invalid_argument("bad field count");
            if (nb == bc) Utils::grow(b, nb, bc);
            Booking bk(stoi(p[0]), stoi(p[1]), stoi(p[2]), stoi(p[3]), p[4]);
            int st = stoi(p[5]);
            if (st < 0 || st > 2) throw invalid_argument("bad status");
            bk.setStatus(static_cast<Booking::Status>(st));
            b[nb++] = bk;
        } catch (const exception&) { skipped++; }
      } }
    if (skipped) cout << "Warning: " << skipped << " corrupt record(s) skipped while loading.\n";
}
