#ifndef BOOKING_H
#define BOOKING_H
#include <string>
using namespace std;

class Booking {
public:
    enum Status { BOOKED = 0, COMPLETED = 1, CANCELLED = 2 };
private:
    int bookingId, customerId, serviceId, workerId;
    string date;       // DD-MM-YYYY
    Status status;
public:
    Booking();
    Booking(int bookingId, int customerId, int serviceId, int workerId, const string& date);
    ~Booking();
    int getId() const { return bookingId; }
    int getCustomerId() const { return customerId; }
    int getServiceId() const { return serviceId; }
    int getWorkerId() const { return workerId; }
    string getDate() const { return date; }
    Status getStatus() const { return status; }
    void setStatus(Status s) { status = s; }
    string statusText() const;
    void book();        // marks as BOOKED  (throws if already finished)
    void cancel();      // BOOKED -> CANCELLED (throws otherwise)
    void complete();    // BOOKED -> COMPLETED (throws otherwise)
    void display() const;
    string toRecord() const;
};
#endif
