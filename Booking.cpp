#include "Booking.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

Booking::Booking() : bookingId(0), customerId(0), serviceId(0), workerId(0), date(""), status(BOOKED) {}
Booking::Booking(int b, int c, int s, int w, const string& d)
    : bookingId(b), customerId(c), serviceId(s), workerId(w), status(BOOKED) {
    if (!Utils::isValidDate(d)) throw invalid_argument("Date must be a valid DD-MM-YYYY");
    date = d;
}
Booking::~Booking() {}
string Booking::statusText() const {
    switch (status) {
        case BOOKED: return "Booked";
        case COMPLETED: return "Completed";
        default: return "Cancelled";
    }
}
void Booking::book() { status = BOOKED; }
void Booking::cancel() {
    if (status != BOOKED) throw logic_error("Only a 'Booked' booking can be cancelled");
    status = CANCELLED;
}
void Booking::complete() {
    if (status != BOOKED) throw logic_error("Only a 'Booked' booking can be completed");
    status = COMPLETED;
}
void Booking::display() const {
    cout << left << setw(5) << bookingId << setw(7) << customerId << setw(7) << serviceId
         << setw(8) << workerId << setw(13) << date << statusText() << "\n";
}
string Booking::toRecord() const {
    return to_string(bookingId) + "|" + to_string(customerId) + "|" + to_string(serviceId) + "|" +
           to_string(workerId) + "|" + date + "|" + to_string(static_cast<int>(status));
}
