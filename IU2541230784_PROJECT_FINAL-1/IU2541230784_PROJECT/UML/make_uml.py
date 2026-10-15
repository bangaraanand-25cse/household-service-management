import subprocess, matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

# ---------- Class diagram (graphviz) ----------
cls = r'''digraph G { rankdir=BT; node [shape=record, fontname="Helvetica", fontsize=10]; edge [fontname="Helvetica", fontsize=9];
Person [label="{«abstract»\nPerson|# id : int\l# name : string\l|+ Person()\l+ Person(id, name)\l+ ~Person() «virtual»\l+ getId() : int\l+ getName() : string\l+ setName(n) : void\l+ display() : void «pure virtual»\l+ toRecord() : string «pure virtual»\l+ role() : string «pure virtual»\l}"];
Customer [label="{Customer|- phone : string\l- address : string\l|+ Customer()\l+ Customer(id,name,phone,address)\l+ setPhone(p) : void\l+ setAddress(a) : void\l+ display() : void\l+ toRecord() : string\l}"];
Worker [label="{Worker|- skill : string\l|+ Worker()\l+ Worker(id,name,skill)\l+ setSkill(s) : void\l+ display() : void\l+ toRecord() : string\l}"];
Service [label="{Service|- serviceId : int\l- name : string\l- charge : double\l|+ Service()\l+ Service(id,name,charge)\l+ setName(n) : void\l+ setCharge(c) : void\l+ display() : void\l+ toRecord() : string\l}"];
Booking [label="{Booking|- bookingId : int\l- customerId : int\l- serviceId : int\l- workerId : int\l- date : string\l- status : Status\l|+ Booking()\l+ Booking(b,c,s,w,date)\l+ book() : void\l+ cancel() : void\l+ complete() : void\l+ display() : void\l+ toRecord() : string\l}"];
FileManager [label="{FileManager|- dir : string\l|+ FileManager(dir)\l+ save(...) : void\l+ load(...) : void\l}"];
HS [label="{HouseholdSystem|- customers : Customer*\l- services : Service*\l- workers : Worker*\l- bookings : Booking*\l- custCount, servCount, workCount, bookCount : int\l- fm : FileManager\l|+ HouseholdSystem()\l+ ~HouseholdSystem()\l+ addCustomer() : void\l+ addService() : void\l+ addWorker() : void\l+ bookService() : void\l+ cancelBooking() : void\l+ completeService() : void\l+ saveData() : void\l+ loadData() : void\l+ run() : void\l}"];
Utils [label="{«namespace»\nUtils|+ readLine / readInt / readDouble\l+ isValidName / isValidPhone\l+ isValidText / isValidDate\l+ toLower(s) : string\l+ grow<T>(arr, count, cap)\l}", style=dashed];
HS -> Utils [style=dashed, arrowhead=vee, label="uses"]; Person -> Utils [style=dashed, arrowhead=vee, label="validates with"];
Customer -> Person [arrowhead=empty]; Worker -> Person [arrowhead=empty];
HS -> Customer [dir=back, arrowtail=diamond, label="1 owns  0..*"]; HS -> Service [dir=back, arrowtail=diamond, label="0..*"];
HS -> Worker [dir=back, arrowtail=diamond, label="0..*"]; HS -> Booking [dir=back, arrowtail=diamond, label="0..*"];
HS -> FileManager [arrowhead=vee, label="uses"];
Booking -> Customer [style=dashed, arrowhead=vee, label="refers by customerId"];
Booking -> Service [style=dashed, arrowhead=vee, label="serviceId"];
Booking -> Worker [style=dashed, arrowhead=vee, label="workerId"];
}'''
open("class.dot","w").write(cls)
subprocess.run(["dot","-Tpng","-Gdpi=130","class.dot","-o","Class_Diagram.png"],check=True)

# ---------- Use case (graphviz) ----------
uc = r'''digraph U { rankdir=LR; fontname="Helvetica"; node [fontname="Helvetica", fontsize=10];
Admin [shape=box, label="«actor»\nAdmin / Manager", style=filled, fillcolor="#e8f0fe"];
Cust [shape=box, label="«actor»\nCustomer\n(via Admin desk)", style=filled, fillcolor="#e8f0fe"];
Disk [shape=box, label="«actor»\nData Files", style=filled, fillcolor="#fef3e0"];
subgraph cluster0 { label="Household Service Management System"; style=rounded;
node [shape=ellipse, style=filled, fillcolor="#f4f4f4"];
u1[label="Add Customer / Service / Worker"]; u2[label="Display Records"]; u3[label="Search Record"];
u4[label="Update Record"]; u5[label="Delete Record"]; u6[label="Book Service"]; u7[label="Cancel Booking"];
u8[label="Complete Service & Bill"]; u9[label="Generate Report"]; u10[label="Save / Load Data"]; }
Admin -> {u1 u2 u3 u4 u5 u6 u7 u8 u9 u10} [arrowhead=none];
Cust -> {u6 u7 u8} [arrowhead=none];
u10 -> Disk [arrowhead=none];
}'''
open("usecase.dot","w").write(uc)
subprocess.run(["dot","-Tpng","-Gdpi=130","usecase.dot","-o","Use_Case_Diagram.png"],check=True)

# ---------- Sequence diagrams (matplotlib) ----------
def seq(title, parts, msgs, fn, notes=None):
    n=len(parts); step=0.62; top=0.0
    H=len(msgs)*step+1.5
    fig,ax=plt.subplots(figsize=(1.95*n+0.6,H*0.95)); ax.set_xlim(-0.1,n+0.1); ax.set_ylim(-len(msgs)*step-0.7,1.0); ax.axis("off")
    ax.set_title(title,fontsize=12,fontweight="bold",pad=6)
    for i,p in enumerate(parts):
        x=i+0.5
        ax.text(x,0.7,p,ha="center",va="center",fontsize=9,fontweight="bold",bbox=dict(boxstyle="round,pad=0.35",fc="#e8f0fe",ec="#333"))
        ax.plot([x,x],[0.5,-len(msgs)*step-0.45],ls="--",color="#888",lw=0.8,zorder=0)
    # activation bars (callee lifelines while handling a call)
    for k,(a,b,t,ret) in enumerate(msgs):
        y=-0.25-k*step; xa,xb=a+0.5,b+0.5
        if not ret and a!=b:
            ax.add_patch(plt.Rectangle((xb-0.04,y-0.28),0.08,0.5,fc="#dfe9fb",ec="#555",lw=0.6,zorder=1))
        lab=f"{k+1}. {t}"
        if a==b:
            ax.annotate("",xy=(xa+0.04,y-0.22),xytext=(xa+0.04,y),arrowprops=dict(arrowstyle="->",connectionstyle="arc3,rad=-1.6",lw=1))
            ax.text(xa+0.16,y-0.1,lab,fontsize=7.8,va="center",bbox=dict(fc="white",ec="none",pad=0.5))
        else:
            ax.annotate("",xy=(xb,y),xytext=(xa,y),arrowprops=dict(arrowstyle="->" if not ret else "->",ls="--" if ret else "-",lw=1),zorder=2)
            ax.text((xa+xb)/2,y+0.09,lab,ha="center",fontsize=7.8,bbox=dict(fc="white",ec="none",pad=0.5))
    if notes:
        ax.text(0.0,-len(msgs)*step-0.62,notes,fontsize=7.5,style="italic",color="#444")
    plt.tight_layout(); plt.savefig(fn,dpi=140,bbox_inches="tight"); plt.close()

P=["Admin","main()","HouseholdSystem","Booking","FileManager"]
seq("Sequence Diagram 1: Book a Service (main transaction)",P,[
 (0,1,"choose 6 > Book a service",0),(1,2,"run() > transactionMenu()",0),
 (0,2,"enter customer, service, worker IDs, date",0),(2,2,"findCustomer / findService / findWorker",0),
 (2,3,"new Booking(id, cust, serv, work, date)",0),(3,2,"validated (else invalid_argument)",1),
 (2,2,"workerBusy(workerId, date)?",0),(2,3,"book()",0),(2,2,"store in bookings[] (grow if full)",0),
 (2,0,"\"Service booked. Booking ID n\"",1),(0,2,"choose 9 > Exit",0),(2,4,"save(customers, services, workers, bookings)",0),(4,2,"Data/*.dat written",1)],
 "Sequence_Diagram_1.png")
seq("Sequence Diagram 2: Cancel a Booking",["Admin","HouseholdSystem","Booking"],[
 (0,1,"choose 6 > Cancel a booking",0),(0,1,"enter Booking ID",0),(1,1,"findBooking(id)",0),
 (1,0,"\"Booking not found\" (if index < 0)",1),(1,2,"cancel()",0),
 (2,2,"status == BOOKED ? set CANCELLED : throw logic_error",0),(2,1,"return / exception",1),
 (1,0,"\"Booking cancelled\" or \"Error: ...\"",1)],
 "Sequence_Diagram_2.png")
