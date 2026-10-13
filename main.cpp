#include "HouseholdSystem.h"

int main() {
    HouseholdSystem system;   // creates dynamic arrays
    system.run();             // menu loop; all logic lives in the system class
    return 0;                 // destructor releases memory
}
