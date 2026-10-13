# Household Service Management (IU2541230784 - CPP72)
Author: Aanand Bangar | Enrollment No.: IU2541230784

Console-based C++17 OOP project: manage customers, services and workers, and book / cancel / complete household services. Records persist in text files in `Data/`.

## Build
    g++ -std=c++17 -Wall -Wextra -Wpedantic -o household src/*.cpp      (Linux/Mac/MinGW)
Windows: double-click `BUILD_WINDOWS.bat` (needs MinGW g++). The build has 0 warnings.

## Run
Run `./household` (or `household.exe`) from this folder so it finds `Data/`. Records load at start and are saved on Exit (9) or via menu 8.

## Menu
1 Add, 2 Display (incl. polymorphic "Everyone"), 3 Search, 4 Update, 5 Delete, 6 Transaction (Book / Cancel / Complete), 7 Report, 8 Save/Load, 9 Exit.

## Classes
Person (abstract) -> Customer, Worker; Service; Booking; FileManager; HouseholdSystem (controller); Utils (validation helpers + dynamic array growth).

## Verification (all real runs)
- `python3 Tests/run_tests.py` : 18 functional tests through a pseudo-terminal; writes `Tests/results.json` and `Tests/Screenshots/*.png`.
- `python3 Tests/make_evidence.py` : -Werror build, 23 OOP checks under AddressSanitizer/UBSan, compile-fail proofs; writes evidence PNGs.
- `Tests/oop_checks.cpp`, `Tests/compile_fail_checks.sh` : the OOP harness sources.

## Folders
`src/` code | `Data/` records | `Tests/` tests + screenshots | `UML/` diagrams (+ generator) | `Docs/` report and submission template (+ generator)
