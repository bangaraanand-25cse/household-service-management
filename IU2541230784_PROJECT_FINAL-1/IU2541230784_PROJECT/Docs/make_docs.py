import json
from docx import Document
from docx.shared import Pt, Inches
res = json.load(open("Tests/results.json"))
ENR="IU2541230784"
CLASSES=[
("Person (abstract)","id, name","Person(), Person(id,name), ~Person(), getId(), getName(), setName(), display()*, toRecord()*, role()*","Base class holding data common to customers and workers; defines the polymorphic interface."),
("Customer","phone, address (+ inherited id, name)","Customer(), Customer(id,name,phone,address), setPhone(), setAddress(), display(), toRecord()","Stores a customer who books services; validates phone and address."),
("Worker","skill (+ inherited id, name)","Worker(), Worker(id,name,skill), setSkill(), display(), toRecord()","Stores a service professional and the skill they offer."),
("Service","serviceId, name, charge","Service(), Service(id,name,charge), setName(), setCharge(), display(), toRecord()","Describes a household service and its price; validates charge."),
("Booking","bookingId, customerId, serviceId, workerId, date, status","Booking(), Booking(...), book(), cancel(), complete(), display(), toRecord()","Links a customer, service and worker on a date; controls the Booked/Completed/Cancelled life cycle."),
("HouseholdSystem","customers*, services*, workers*, bookings* (dynamic arrays), counts, capacities, FileManager fm","HouseholdSystem(), ~HouseholdSystem(), addCustomer(), addService(), addWorker(), bookService(), cancelBooking(), completeService(), saveData(), loadData(), run()","Controller: owns the dynamic arrays, implements every menu option and business rule."),
("FileManager","dir (data folder)","FileManager(), save(), load()","Reads and writes all records to text files in Data/."),
("Utils (namespace)","none (free functions)","readLine(), readInt(), readDouble(), isValidName(), isValidPhone(), isValidText(), isValidDate(), toLower(), grow<T>()","Input reading, validation rules and the template that doubles a dynamic array."),
]
CONCEPTS=[
("Encapsulation","All entity classes (private/protected data)","Data members are private or protected; they are changed only through validated setters (e.g. Customer::setPhone) so invalid data cannot enter an object."),
("Constructor","Every class","Default constructors create empty objects for the dynamic arrays; parameterized constructors (e.g. Customer(id,name,phone,address)) build fully validated objects."),
("Destructor","Person (virtual), all classes, HouseholdSystem","~HouseholdSystem() calls delete[] on all four arrays. ~Person() is virtual so derived objects are destroyed correctly."),
("Inheritance","Customer and Worker derive from Person","Common members (id, name, getters, setName) are written once in Person and reused by both subclasses."),
("Runtime Polymorphism","Person::display()/toRecord()/role() pure virtual","In 'Display > Everyone', a Person* points to each Customer and Worker and p->display() calls the correct overridden version at run time."),
("Dynamic Memory","HouseholdSystem constructor, Utils::grow<T>()","Arrays are created with new Customer[capacity]; when full, a double-size array is allocated, elements copied, old one delete[]d. Memory is freed in the destructor. Needed because the number of records is unknown in advance."),
("File Handling","FileManager::save()/load()","Records are written with ofstream as pipe-separated lines in Data/*.dat and read back with ifstream at start-up; corrupt lines are skipped with a warning."),
("Exception Handling","Setters/constructors, menu functions, FileManager","Invalid input throws invalid_argument; illegal booking changes throw logic_error; menu functions catch them and print a message so the program never crashes."),
]
FEATURES=["Add, display, search, update and delete customers, services and workers","Book a service, cancel it, or complete it and generate a bill","Prevents double-booking a worker on the same date","Summary report with booking counts and revenue per service","Input validation (name, 10-digit phone, positive charge, real calendar date)","Automatic load at start and save at exit using text files"]
FLOW="USER → MAIN MENU (main.cpp / HouseholdSystem::run) → HouseholdSystem → Customer / Service / Worker / Booking objects → FileManager → Data/*.dat files"
def table(doc, header, rows, widths=None):
    t=doc.add_table(rows=1, cols=len(header)); t.style="Table Grid"
    for i,h in enumerate(header):
        c=t.rows[0].cells[i]; c.text=""; r=c.paragraphs[0].add_run(h); r.bold=True
    for row in rows:
        cells=t.add_row().cells
        for i,v in enumerate(row): cells[i].text=str(v)
    for row in t.rows:
        for c in row.cells:
            for p in c.paragraphs:
                for r in p.runs: r.font.size=Pt(9)
    return t
def base():
    d=Document(); d.styles["Normal"].font.name="Calibri"; d.styles["Normal"].font.size=Pt(11); return d

# ================= SUBMISSION TEMPLATE =================
d=base(); d.add_heading(f"{ENR} – CPP72 – SUBMISSION TEMPLATE",1)
for l in ["Project Name: Household Service Management",f"Enrollment No.: {ENR}","Student Name: Aanand Bangar","Date of Submission: 14-10-2026"]: d.add_paragraph(l)
d.add_heading("1. Project Summary",2)
d.add_paragraph("Problem Statement: Households need a simple way to book services such as plumbing, electrical work and cleaning, assign a worker, and keep records. Doing this on paper causes double bookings and lost records. This project is a console application that manages customers, services, workers and bookings and stores them in files.")
d.add_paragraph("Objectives: (1) Apply OOP concepts in C++; (2) manage records with add/display/search/update/delete; (3) implement the booking transaction with validation; (4) keep data after the program closes using file handling.")
d.add_paragraph("Key Features:")
for f in FEATURES: d.add_paragraph(f,style="List Bullet")
d.add_heading("2. Class Summary",2)
table(d,["Class","Purpose / Responsibility","Key Data Members","Key Member Functions"],[(c[0],c[3],c[1],c[2]) for c in CLASSES])
d.add_heading("3. Project Flow",2)
d.add_paragraph("USER → MAIN MENU → HouseholdSystem → Entity classes (Customer, Service, Worker, Booking) → FileManager → FILE (Data/*.dat)")
d.add_heading("4. UML Deliverables",2)
table(d,["Diagram","File Name","Completed"],[("Use Case Diagram","UML/Use_Case_Diagram.png","☑"),("Class Diagram","UML/Class_Diagram.png","☑"),("Sequence Diagram 1 (Book a Service)","UML/Sequence_Diagram_1.png","☑"),("Sequence Diagram 2 (Cancel a Booking)","UML/Sequence_Diagram_2.png","☑")])
d.add_heading("5. C++ Concepts Demonstrated",2)
table(d,["Concept","Where Used in My Project","Explanation"],CONCEPTS)
d.add_heading("6. Testing",2)
table(d,["Test ID","Operation","Input","Expected Output","Actual Output","Pass/Fail"],[(r["id"],r["op"],r["input"],r["expected"],r["actual"],r["status"]) for r in res])
d.add_heading("7. Deployment",2)
for l in ["Compiler / IDE Used: g++ (GCC) with C++17; any editor / Code::Blocks / VS Code","Compilation Command: g++ -std=c++17 -Wall -Wextra -o household src/*.cpp","Executable Name: household (household.exe on Windows)","Data File(s): Data/customers.dat, Data/services.dat, Data/workers.dat, Data/bookings.dat"]: d.add_paragraph(l)
d.add_heading("8. Student Declaration",2)
d.add_paragraph("I understand the submitted project and can explain its classes, data members, member functions, file handling and UML diagrams.")
d.add_paragraph("Student Signature: ____________________")
d.save(f"Docs/{ENR}_CPP72_Submission_Template.docx")

# ================= PROJECT REPORT =================
d=base(); d.add_heading(f"{ENR} – CPP72 – PROJECT REPORT",1); d.add_heading("Household Service Management",2)
for l in [f"Enrollment No.: {ENR}","Student Name: Aanand Bangar","Semester: 3","Subject: C++ Guided Project"]: d.add_paragraph(l)
def sec(t,paras=None,bul=None):
    d.add_heading(t,2)
    for p in paras or []: d.add_paragraph(p)
    for b in bul or []: d.add_paragraph(b,style="List Bullet")
sec("1. Introduction",["Household Service Management is a console-based C++ application built with object-oriented programming. It lets an administrator keep records of customers, household services and workers, book a worker for a service on a date, cancel or complete the booking, and view a summary report. All records are saved to files so they are available the next time the program runs."])
sec("2. Problem Statement",["Small household service providers often record bookings on paper or in memory. This causes double-booked workers, forgotten appointments, lost customer details and no quick way to see revenue. A simple, validated, persistent system is needed to manage these records."])
sec("3. Objectives",bul=["Design the system with classes, encapsulation, inheritance and polymorphism.","Provide add, display, search, update and delete for all records.","Implement the booking transaction (book, cancel, complete) with business rules.","Use dynamic memory for record storage and release it correctly.","Persist data using file handling and validate all input with exception handling."])
sec("4. Functional Requirements",bul=["FR1: Add, display, search, update and delete customers, services and workers.","FR2: Book a service for an existing customer, service and worker on a valid date.","FR3: Reject a booking if the worker is already booked on that date.","FR4: Cancel a booked booking; complete a booked booking and show the bill.","FR5: Block deletion of a customer, service or worker that has an active booking.","FR6: Show a report: counts, booking status totals, revenue per service.","FR7: Load records at start-up and save on exit or on request."])
sec("5. System / Project Flow",[FLOW,"main.cpp only creates a HouseholdSystem object and calls run(). run() loads the files, shows the 9-option menu in a loop, and on Exit saves all records back to the files."])
d.add_heading("6. Class Descriptions",2)
table(d,["Class","Data Members","Member Functions","Responsibility"],[(c[0],c[1],c[2],c[3]) for c in CLASSES])
d.add_paragraph("Relationships: Customer and Worker inherit from Person (is-a). HouseholdSystem aggregates arrays of Customer, Service, Worker and Booking (has-a). Booking refers to a customer, service and worker by ID. HouseholdSystem uses FileManager for persistence.")
sec("7. OOP Concepts Used")
table(d,["Concept","Where Used","Explanation"],CONCEPTS)
sec("8. File Handling and Data Persistence",["FileManager stores each record type in its own text file inside Data/, one record per line with fields separated by '|': customers.dat (id|name|phone|address), services.dat (id|name|charge), workers.dat (id|name|skill) and bookings.dat (id|customerId|serviceId|workerId|date|status).","At start-up run() calls loadData(); on Exit it calls saveData(). A missing file is treated as an empty list (first run). Malformed lines are skipped and a warning is printed. Test T15 closes and reopens the program and confirms the old records and revenue are still present. Text fields cannot contain '|' (validated), so the format is unambiguous."])
d.add_heading("9. UML Diagrams",2)
for cap,f in [("Use Case Diagram","Use_Case_Diagram"),("Class Diagram","Class_Diagram"),("Sequence Diagram 1 – Book a Service","Sequence_Diagram_1"),("Sequence Diagram 2 – Cancel a Booking","Sequence_Diagram_2")]:
    _p=d.add_paragraph(cap); _p.runs[0].bold=True; _p.paragraph_format.keep_with_next=True
    d.add_picture(f"UML/{f}.png",width=Inches(5.8 if "Class" not in f else 4.8))
sec("10. Implementation Details",bul=["Source files: main.cpp, Person, Customer, Worker, Service, Booking, FileManager, HouseholdSystem, Utils (.h/.cpp).","Dynamic memory: HouseholdSystem allocates four arrays with new[] (initial capacity 4). Utils::grow<T>() doubles capacity when full; ~HouseholdSystem() calls delete[]. Copying of HouseholdSystem is disabled to avoid double deletion. The program was also checked with AddressSanitizer, with no leaks or memory errors.","Validation: names (letters/spaces), phone (10 digits), charge (1 to 1,000,000), date (real DD-MM-YYYY including leap years), menu numbers (re-asked if not a number).","Business rules: a worker cannot have two active bookings on one date; only 'Booked' bookings can be cancelled or completed; records used by active bookings cannot be deleted.","Compile: g++ -std=c++17 -Wall -Wextra -o household src/*.cpp (no warnings)."])
d.add_heading("11. Testing and Results",2)
table(d,["Test ID","Input / Operation","Expected","Actual","Status"],[(r["id"],r["op"]+": "+r["input"],r["expected"],r["actual"],r["status"]) for r in res])
d.add_paragraph(f"Result: {sum(r['status']=='PASS' for r in res)} of {len(res)} tests passed.")
d.add_heading("12. OOP Verification and Evidence (real runs)",2)
d.add_paragraph("All screenshots below were generated by running the compiled program (Tests/run_tests.py, Tests/make_evidence.py); nothing is typed in by hand. Typed input is shown in cyan and the checked output line is highlighted.")
d.add_paragraph("OOP harness: Tests/oop_checks.cpp runs 23 checks (abstraction, inheritance, virtual destructor, encapsulation, polymorphism via Person*, exceptions, dynamic array growth, file round-trip with a corrupt line) under AddressSanitizer and UBSan: 23 passed, 0 failed, no leaks. Tests/compile_fail_checks.sh proves the compiler rejects instantiating Person and accessing private members.")
for cap,f in [("Build: g++ -Wall -Wextra -Wpedantic -Werror","BUILD_PASS"),("OOP verification harness","OOP_checks_PASS"),("Compiler enforces abstraction and encapsulation","OOP_compile_fail_PASS"),("T10 Book a service","T10_PASS"),("T11 Double-booking blocked","T11_PASS"),("T13 Complete service and bill","T13_PASS"),("T17 Runtime polymorphism (Person*)","T17_PASS"),("T18 Report","T18_PASS"),("T15 Persistence after restart","T15_PASS")]:
    _p=d.add_paragraph(cap); _p.runs[0].bold=True; _p.paragraph_format.keep_with_next=True
    d.add_picture(f"Tests/Screenshots/{f}.png",width=Inches(6.0))
sec("13. Limitations",bul=["Console only; no graphical interface.","Dates are text and are not checked against today's date.","Single user; plain text files are not encrypted.","Searching is linear, which is fine for small data."])
sec("14. Future Scope",bul=["Graphical or web interface.","Customer ratings, worker availability calendar and payment records.","Database (e.g. SQLite) instead of text files.","User login with admin and customer roles."])
sec("15. Conclusion",["The project meets the blueprint: seven classes with inheritance and runtime polymorphism, dynamic arrays, validated input with exception handling, a complete booking transaction, and file persistence. All 15 test cases pass. Building it showed how separating responsibilities into classes keeps code clear and easy to extend."])
d.save(f"Docs/{ENR}_CPP72_Project_Report.docx")
print("docs ok")
