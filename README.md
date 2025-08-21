# 🏥 Hospital Management System (HMS)

A C++-based hospital management prototype developed during the FuzeTek Summer Internship.  The system manages patients, doctors, appointments, medical records, and emergencies using Object-Oriented Programming (OOP) and fundamental data structures.  
It is built according to the Software Requirements Specification (SRS) and follows the full software lifecycle from design to testing.

---

# 🚀 Features

**Patient Management**

- Register, admit, and discharge patients.
- Store and display medical history.
- Request and perform medical tests.

**Doctor Management**

- Add doctors with department specialization.
- Manage doctor queues and appointments.
- Track doctor details and assigned patients.

**Appointments & Scheduling**

- Book appointments between patients and doctors.
- Handle appointment queues in order.

**Emergency Handling**

- Add emergency cases to a priority queue.
- Process and handle urgent admissions.

**Hospital Administration**

- View patient information and medical records.
- Display doctor details and schedules.

---

# 📂 System Architecture

The HMS follows an Object-Oriented Design with the following classes:

| Class Name | Responsibility |
|------------|----------------|
| Patient    | Manages patient lifecycle, history, and tests. |
| Doctor     | Handles doctor details, department, and appointments. |
| Hospital   | Central controller for patients, doctors, and emergencies. |
| Enums      | Defines categories like RoomType and Department. |

---

# 📌 Functional Requirements Summary

| ID     | Feature             | Description |
|--------|-------------------|-------------|
| FR1.1  | Register Patient    | Add a new patient with basic details. |
| FR1.2  | Admit Patient       | Assign room type and mark as admitted. |
| FR1.3  | Discharge Patient   | Update status and log discharge. |
| FR1.4  | Medical Records     | Add and view patient history. |
| FR1.5  | Request/Perform Test | Queue and complete diagnostic tests. |
| FR2.1  | Add Doctor          | Register doctor with department. |
| FR2.2  | Book Appointment    | Assign patient to doctor’s queue. |
| FR2.3  | Doctor Sees Patient | Pop next patient from queue. |
| FR3.1  | Add Emergency       | Add patient to emergency queue. |
| FR3.2  | Handle Emergency    | Process emergency admissions. |
| FR4.1  | Display Patient Info | View patient details and history. |
| FR4.2  | Display Doctor Info  | Show doctor profile and appointments. |

---

# 🎯 Internship Context

This project is designed to give interns hands-on experience in:

- Agile software development with Jira.
- Modular OOP design and data structures in C++.
- Verification and validation of SRS requirements.
- Team collaboration using GitHub and version control best practices.

---
