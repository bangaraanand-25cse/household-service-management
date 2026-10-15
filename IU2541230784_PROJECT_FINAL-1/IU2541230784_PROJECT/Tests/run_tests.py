#!/usr/bin/env python3
"""Runs the REAL household executable in a pseudo-terminal (so typed input is echoed like a
real console), records the true output, checks it against the expected text, and renders each
transcript to Tests/Screenshots/Txx_PASS.png.  Usage:  python3 Tests/run_tests.py"""
import os, pty, select, subprocess, shutil, json, tempfile, re, sys, time
from PIL import Image, ImageDraw, ImageFont

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
EXE = os.path.join(ROOT, "household")
SHOTS = os.path.join(HERE, "Screenshots"); os.makedirs(SHOTS, exist_ok=True)
ANSI = re.compile(r"\x1b\[[0-9;]*[A-Za-z]")

def session(inputs, cwd):
    """Drive the program through a pty: send each line only after the program has printed its prompt."""
    pid, fd = pty.fork()
    if pid == 0:
        os.chdir(cwd); os.execv(EXE, [EXE])
    buf = b""
    def drain(t):
        nonlocal buf
        while True:
            r, _, _ = select.select([fd], [], [], t)
            if not r: return True
            try: d = os.read(fd, 4096)
            except OSError: return False
            if not d: return False
            buf += d
    alive = drain(0.25)
    for line in inputs:
        if not alive: break
        os.write(fd, (line + "\n").encode()); alive = drain(0.06)
    drain(0.4)
    os.waitpid(pid, 0)
    return ANSI.sub("", buf.decode(errors="replace").replace("\r\n", "\n").replace("\r", ""))

def last_lines(text, key):
    """Return the real output line (from the transcript) that contains the key."""
    for l in text.splitlines():
        if key in l: return l.strip()
    return None

SETUP = ["1","1","Ravi Kumar","9876543210","12 MG Road","1","1","Anita Shah","9123456780","5 Park Street",
         "1","2","AC Repair","500","1","2","Deep Cleaning","1200",
         "1","3","Suresh Patil","Plumbing","1","3","Mohan Das","Electrical"]
BOOK = ["6","1","1","1","1","20-10-2026"]
# (id, operation, input description, expected text, inputs)
T = [
 ("T01","Add Customer","Name=Ravi Kumar, Phone=9876543210","Customer added with ID 1",["1","1","Ravi Kumar","9876543210","12 MG Road","9"]),
 ("T02","Add Customer (invalid phone)","Phone=12345","Error: Phone must be exactly 10 digits",["1","1","Ravi Kumar","12345","12 MG Road","9"]),
 ("T03","Add Service","AC Repair, Rs. 500","Service added with ID 1",["1","2","AC Repair","500","9"]),
 ("T04","Add Service (negative charge)","Charge=-50","Error: Charge must be between 1 and 1000000",["1","2","AC Repair","-50","9"]),
 ("T05","Add Worker","Suresh Patil, Plumbing","Worker added with ID 1",["1","3","Suresh Patil","Plumbing","9"]),
 ("T06","Display Customers","Display > Customers","2    Anita Shah            9123456780",SETUP+["2","1","9"]),
 ("T07","Search Customer by name","Name contains 'anit' (case-insensitive)","2    Anita Shah            9123456780",SETUP+["3","2","anit","9"]),
 ("T08","Update Service charge","Service 1 -> Rs. 650","Service updated.",SETUP+["4","2","1","650","2","2","9"]),
 ("T09","Delete Customer","Delete customer ID 2","Customer deleted.",SETUP+["5","1","2","9"]),
 ("T10","Book Service","Cust 1, Service 1, Worker 1, 20-10-2026","Service booked. Booking ID 1",SETUP+BOOK+["9"]),
 ("T11","Book (worker already busy)","Worker 1 again on 20-10-2026","Error: Worker already booked on that date",SETUP+BOOK+["6","1","2","2","1","20-10-2026","9"]),
 ("T12","Book (invalid date)","Date=31-02-2026","Error: Date must be a valid DD-MM-YYYY",SETUP+["6","1","1","1","1","31-02-2026","9"]),
 ("T13","Complete Service","Complete booking 1","Service completed. Bill: Rs. 500.00",SETUP+BOOK+["6","3","1","9"]),
 ("T14","Cancel Booking twice","Cancel booking 1 two times","Error: Only a 'Booked' booking can be cancelled",SETUP+BOOK+["6","2","1","6","2","1","9"]),
 ("T16","Delete blocked by active booking","Delete customer 1 who has a Booked booking","Cannot delete: customer has an active booking.",SETUP+BOOK+["5","1","1","9"]),
 ("T17","Polymorphic display (Person*)","Display > Everyone","Worker: 1",SETUP+["2","5","9"]),
 ("T18","Report with per-worker totals","Book + complete, then Report","Suresh Patil          1",SETUP+BOOK+["6","3","1","7","9"]),
]

def font(sz, bold=False):
    for p in ["/usr/share/fonts/truetype/dejavu/DejaVuSansMono%s.ttf" % ("-Bold" if bold else ""),
              "/usr/share/fonts/truetype/liberation/LiberationMono-%s.ttf" % ("Bold" if bold else "Regular")]:
        if os.path.exists(p): return ImageFont.truetype(p, sz)
    return ImageFont.load_default()

def render(tid, title, status, transcript, inputs, expected, path):
    """Terminal-style PNG of the real transcript; typed input shown in cyan, the expected result line highlighted."""
    lines = transcript.splitlines()
    # show from the first interaction of the TEST (skip long set-up menus): keep last 34 lines
    omitted = max(0, len(lines) - 34); lines = lines[omitted:]
    f, fb = font(17), font(17, True)
    lh, pad, w = 24, 26, 1180
    h = pad*2 + 46 + lh*(len(lines)+ (1 if omitted else 0)) + 40
    im = Image.new("RGB", (w, h), (18, 18, 20)); d = ImageDraw.Draw(im)
    d.rectangle([0, 0, w, 40], fill=(48, 48, 54))
    for i, c in enumerate([(255,95,86),(255,189,46),(39,201,63)]): d.ellipse([16+i*26, 12, 30+i*26, 26], fill=c)
    d.text((110, 9), f"{tid} - {title}   |   household  (IU2541230784 - Aanand Bangar)", font=f, fill=(210,210,215))
    badge = "PASS" if status == "PASS" else "FAIL"
    d.rounded_rectangle([w-110, 6, w-14, 34], 6, fill=(30,150,60) if status=="PASS" else (190,40,40))
    d.text((w-98, 10), badge, font=fb, fill=(255,255,255))
    y = 40 + pad
    if omitted: d.text((pad, y), f"... ({omitted} earlier lines omitted) ...", font=f, fill=(120,120,130)); y += lh
    echoed = set(inputs)
    for ln in lines:
        hit = expected.split("\n")[0] in ln
        if hit: d.rectangle([pad-6, y-2, w-pad+6, y+lh-4], fill=(30, 70, 40))
        # split prompt (grey/white) from typed echo (cyan): typed text = trailing part after last ': '
        m = re.match(r"^(.*?: )(\S.*)$", ln)
        if m and m.group(2).strip() in echoed and not hit and len(ln) < 70 and ("Enter choice" in ln or "Choice" in ln or ": " in ln):
            d.text((pad, y), m.group(1), font=f, fill=(235,235,235))
            d.text((pad + d.textlength(m.group(1), font=f), y), m.group(2), font=fb, fill=(80,220,235))
        else:
            d.text((pad, y), ln, font=f, fill=(120,255,150) if hit else (235,235,235))
        y += lh
    d.text((pad, h-34), f"Expected: \"{expected}\"   ->   {status}", font=f, fill=(255,214,102))
    im.save(path)

def main():
    if not os.path.exists(EXE): sys.exit("Build first: g++ -std=c++17 -Wall -Wextra -o household src/*.cpp")
    results = []
    for tid, op, inp, exp, inputs in T:
        d = tempfile.mkdtemp(); out = session(inputs, d)
        actual = last_lines(out, exp.split("\n")[0]); ok = actual is not None
        res = dict(id=tid, op=op, input=inp, expected=exp, actual=actual or "(expected text NOT found)", status="PASS" if ok else "FAIL", out=out)
        results.append(res); shutil.rmtree(d)
        render(tid, op, res["status"], out, inputs, exp, os.path.join(SHOTS, f"{tid}_{res['status']}.png"))
    # T15 persistence: run 1 saves on exit, run 2 (fresh process, same Data folder) reloads
    d = tempfile.mkdtemp(); session(SETUP + BOOK + ["6","3","1","9"], d)
    in2 = ["7","2","4","9"]; out2 = session(in2, d)
    dat = open(os.path.join(d, "Data", "bookings.dat")).read().strip()
    exp = "Data loaded: 2 customers, 2 services, 2 workers, 1 bookings."
    ok = exp in out2 and "Completed 1" in out2 and "Total revenue (completed): Rs. 500.00" in out2
    r = dict(id="T15", op="Persistence: save on exit, reload on restart", input="Run 1: add data, book, complete, Exit. Run 2: restart, Report",
             expected=exp + " + Completed 1, revenue Rs. 500.00", actual=(last_lines(out2, "Data loaded") + " + " + last_lines(out2, "Bookings :") + " + " + last_lines(out2, "Total revenue")) if ok else "(mismatch)",
             status="PASS" if ok else "FAIL", out=out2 + "\n[bookings.dat after run 1] " + dat)
    results.append(r); shutil.rmtree(d)
    render("T15", r["op"], r["status"], out2, in2, exp, os.path.join(SHOTS, f"T15_{r['status']}.png"))
    results.sort(key=lambda r: r["id"])
    json.dump(results, open(os.path.join(HERE, "results.json"), "w"), indent=1)
    # keep a plain-text transcript of one full real session too
    for r in results: print(r["id"], r["status"], "-", r["op"], "|", r["actual"])
    print("TOTAL: %d/%d passed" % (sum(r["status"] == "PASS" for r in results), len(results)))
    return 0 if all(r["status"] == "PASS" for r in results) else 1
if __name__ == "__main__": sys.exit(main())
