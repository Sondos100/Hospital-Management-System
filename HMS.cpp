#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

// All used data are AI-generated and for educational purpose only !
const string PATIENT_FILE = "patients.csv";
const string DOCTOR_FILE = "doctors.csv";

string getCurrentDateTime()
{
    time_t now = time(0);
    char buffer[80];
    tm *ltm = localtime(&now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}

// ========== ENUMERATIONS ========== //
enum Department
{
    CARDIOLOGY,
    NEUROLOGY,
    ORTHOPEDICS,
    PEDIATRICS,
    EMERGENCY,
    GENERAL
};

enum RoomType
{
    GENERAL_WARD,
    ICU,
    PRIVATE_ROOM,
    SEMI_PRIVATE
};

// ========== PATIENT CLASS ========== //
// Stores individual patient details and medical records.
class Patient
{
private:
    int id;
    string name;
    int age;
    string contact;
    stack<string> medicalHistory;
    queue<string> testQueue;
    bool isAdmitted;
    RoomType roomType;

public:
    Patient(int pid, string n, int a, string c)
    {
        // Supposed Entered Data Are Valid
        id = pid;
        name = n;
        age = a;
        contact = c;
        isAdmitted = false;
    }

    void admitPatient(RoomType type)
    {
        isAdmitted = true;
        roomType = type;
        addMedicalRecord("Admitted to " + roomString(type) + " on " + getCurrentDateTime());
    }

    void dischargePatient()
    {
        isAdmitted = false;
        addMedicalRecord("Discharged from hospital on " + getCurrentDateTime());
    }

    void addMedicalRecord(string record)
    {
        medicalHistory.push(record);
    }

    void requestTest(string testName)
    {
        testQueue.push(testName);
        addMedicalRecord("Requested test: " + testName + " on " + getCurrentDateTime());
    }

    string performTest()
    {
        if (testQueue.empty())
        {
            return "No tests are pending";
        }

        string testName = testQueue.front();
        testQueue.pop();
        addMedicalRecord("Performed test: " + testName + " on " + getCurrentDateTime());

        return testName;
    }

    void displayHistory()
    {
        stack<string> tempHistory = medicalHistory;

        if (tempHistory.empty())
        {
            cout << "No medical history available." << endl;
            return;
        }

        else
        {
            // Displaying history in reverse order (LIFO)
            cout << "\n------- Medical History -------\n";
            while (!tempHistory.empty())
            {
                cout << tempHistory.top() << endl;
                tempHistory.pop();
            }
            cout << "________________________________________\n\n";
        }
    }

    int getId()
    {
        return id;
    }

    string getName()
    {
        return name;
    }

    int getAge()
    {
        return age;
    }

    string getContact()
    {
        return contact;
    }

    bool getAdmissionStatus()
    {
        return isAdmitted;
    }

    string roomString(RoomType type)
    {
        switch (type)
        {
        case GENERAL_WARD:
            return "General Ward";
        case ICU:
            return "ICU";
        case PRIVATE_ROOM:
            return "Private Room";
        case SEMI_PRIVATE:
            return "Semi-Private Room";
        default:
            return "Unknown";
        }
    }

    string getRoomTypeAsString()
    {
        return (isAdmitted ? roomString(roomType) : "None");
    }
};

// ========== DOCTOR CLASS ========== //
// Stores doctor details and appointment handling.
class Doctor
{
private:
    int id;
    string name;
    Department department;
    queue<int> appointmentQueue;
    int appointmentCount;

public:
    Doctor(int did, string n, Department d)
    {
        id = did;
        name = n;
        department = d;
        appointmentCount = 0;
    }

    // New constructor for loading from file
    Doctor(int did, string n, Department d, int aCount)
    {
        id = did;
        name = n;
        department = d;
        appointmentCount = aCount; // to preserve the previous values
    }

    void addAppointment(int patientId)
    {
        appointmentQueue.push(patientId);
        appointmentCount++;
    }

    int seePatient()
    {
        if (appointmentQueue.empty())
        {
            return -1;
        }

        int nextPatient = appointmentQueue.front();
        appointmentQueue.pop();
        appointmentCount--;
        return nextPatient;
    }

    int getId()
    {
        return id;
    }

    string getName()
    {
        return name;
    }

    string getDepartment()
    {
        switch (department)
        {
        case CARDIOLOGY:
            return "Cardiology";
        case NEUROLOGY:
            return "Neurology";
        case ORTHOPEDICS:
            return "Orthopedics";
        case PEDIATRICS:
            return "Pediatrics";
        case EMERGENCY:
            return "Emergency";
        case GENERAL:
            return "General";
        default:
            return "Unknown";
        }
    }

    int getAppointmentCount() const
    {
        return appointmentCount;
    }
};

// ========== HOSPITAL CLASS ========== //
// Manages hospital-level operations: patients, doctors, emergencies, data storage.
class Hospital
{
private:
    vector<Patient> patients;
    vector<Doctor> doctors;
    queue<int> emergencyQueue;
    int patientCounter;
    int doctorCounter;

public:
    Hospital()
    {
        patientCounter = 0;
        doctorCounter = 0;
        loadPatients();
        loadDoctors();
    }

    // Save current patient list to CSV file.
    void savePatients()
    {
        ofstream file(PATIENT_FILE);
        if (!file.is_open())
        {
            cerr << "Error: Could not open " << PATIENT_FILE << " for writing.\n";
            return;
        }

        file << "ID,Name,Age,Contact,Admission Status,Room Type\n";
        for (auto &p : patients)
        {
            file << p.getId() << ","
                 << p.getName() << ","
                 << p.getAge() << ","
                 << p.getContact() << ","
                 << (p.getAdmissionStatus() ? "Admitted" : "Not Admitted") << ","
                 << p.getRoomTypeAsString() << "\n";
        }

        file.close();
    }

    void saveDoctors()
    {
        ofstream file(DOCTOR_FILE);
        if (!file.is_open())
        {
            cerr << "Error: Could not open " << DOCTOR_FILE << " for writing.\n";
            return;
        }

        file << "ID,Name,Department,Appointment\n";

        for (auto &d : doctors)
        {
            file << d.getId() << ","
                 << d.getName() << ","
                 << d.getDepartment() << ","
                 << d.getAppointmentCount() << "\n";
        }

        file.close();
    }

    // Load patient data from file into memory.
    // Expected CSV format: ID,Name,Age,Contact,AdmissionStatus
    void loadPatients()
    {
        ifstream file(PATIENT_FILE);
        if (!file.is_open())
        {
            cerr << "Error opening patient file.\n";
            return;
        }

        string line;
        getline(file, line);

        while (getline(file, line))
        {
            stringstream ss(line);
            string idStr, name, ageStr, contact, admittedStr, roomStr;
            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, ageStr, ',');
            getline(ss, contact, ',');
            getline(ss, admittedStr, ',');
            getline(ss, roomStr);

            int id = stoi(idStr);
            int age = stoi(ageStr);
            bool admitted = (admittedStr == "Admitted");

            Patient p(id, name, age, contact);

            if (admitted)
            {
                RoomType room = GENERAL_WARD;
                if (roomStr == "ICU")
                    room = ICU;
                else if (roomStr == "Private Room")
                    room = PRIVATE_ROOM;
                else if (roomStr == "Semi-Private Room")
                    room = SEMI_PRIVATE;
                else if (roomStr == "General Ward")
                    room = GENERAL_WARD;

                p.admitPatient(room);
            }

            patients.push_back(p);
            patientCounter = id;
        }

        file.close();
    }

    void loadDoctors()
    {
        ifstream file(DOCTOR_FILE);
        if (!file.is_open())
        {
            cerr << "Error opening doctors file.\n";
            return;
        }

        string line;
        getline(file, line); // Skip header

        while (getline(file, line))
        {
            if (line.empty())
                continue; // Skip empty lines

            stringstream ss(line);
            string idStr, name, deptStr, countStr;

            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, deptStr, ',');
            getline(ss, countStr);

            if (idStr.empty() || name.empty() || deptStr.empty() || countStr.empty())
            {
                cerr << "Skipping bad line: " << line << endl;
                continue;
            }

            int id = stoi(idStr);
            int count = stoi(countStr);

            Department dept = GENERAL;
            if (deptStr == "Cardiology")
                dept = CARDIOLOGY;
            else if (deptStr == "Neurology")
                dept = NEUROLOGY;
            else if (deptStr == "Orthopedics")
                dept = ORTHOPEDICS;
            else if (deptStr == "Pediatrics")
                dept = PEDIATRICS;
            else if (deptStr == "Emergency")
                dept = EMERGENCY;

            // Doctor doc(id, name, dept);      XXX when adding a new doc. all appointments are set to 0.
            Doctor doc(id, name, dept, count);

            doctors.push_back(doc);
            if (id > doctorCounter)
                doctorCounter = id;
        }

        file.close();
    }

    int registerPatient(string name, int age, string contact)
    {
        Patient newPatient(++patientCounter, name, age, contact);
        patients.push_back(newPatient);
        savePatients();
        return patientCounter;
    }

    int addDoctor(string name, Department dept)
    {
        Doctor newDoctor(++doctorCounter, name, dept);
        doctors.push_back(newDoctor);
        saveDoctors();
        return doctorCounter;
    }

    void admitPatient(int patientId, RoomType type)
    {
        for (auto &patient : patients)
        {
            if (patient.getId() == patientId)
            {
                if (patient.getAdmissionStatus())
                {
                    cout << "ERROR: Patient '" << patient.getName() << "' is already admitted." << endl;
                }
                else
                {
                    patient.admitPatient(type);
                    cout << "Patient '" << patient.getName()
                         << "' is admitted to " << patient.roomString(type) << "." << endl;
                }
                return;
            }
        }

        cout << "ERROR: Patient ID '" << patientId << "' not found." << endl;
    }

    void addEmergency(int patientId)
    {
        for (Patient &p : patients)
        {
            if (patientId == p.getId())
            {
                emergencyQueue.push(patientId);
                p.addMedicalRecord("Marked as Emergency Case on " + getCurrentDateTime());
                cout << "Patient '" << p.getName() << "' added to emergency queue." << endl;
                // savePatients();
                return;
            }
        }

        cout << "ERROR: Patient ID not found, please register first." << endl;
    }

    int handleEmergency()
    {
        if (emergencyQueue.empty())
        {
            cout << "No emergency cases in queue." << endl;
            return -1;
        }

        int patientId = emergencyQueue.front();
        emergencyQueue.pop();

        for (Patient &p : patients)
        {
            if (p.getId() == patientId)
            {
                p.addMedicalRecord("Emergency Case Handled on " + getCurrentDateTime());
                cout << "Emergency handled for patient '" << p.getName() << "'." << endl;
                break;
            }
        }
        // savePatients();
        return patientId;
    }

    void bookAppointment(int doctorId, int patientId)
    {
        for (Doctor &d : doctors)
        {
            if (doctorId == d.getId())
            {
                for (Patient &p : patients)
                {
                    if (patientId == p.getId())
                    {
                        d.addAppointment(patientId);
                        p.addMedicalRecord("Appointment booked with Doctor ID " + to_string(doctorId) + " on " + getCurrentDateTime());
                        cout << "Patient '" << p.getName() << "' booked appointment with " << d.getName() << "." << endl;
                        return;
                    }
                }
                cout << "ERROR: Patient ID '" << patientId << "' not found." << endl;
                return;
            }
        }

        cout << "ERROR: Doctor ID '" << doctorId << "' not found." << endl;
    }

    void displayPatientInfo(int patientId)
    {
        for (Patient &p : patients)
        {
            if (p.getId() == patientId)
            {
                cout << "\n========= Patient Information =========\n";
                cout << "ID : " << p.getId() << endl;
                cout << "Name : " << p.getName() << endl;
                cout << "Age : " << p.getAge() << endl;
                cout << "Contact : " << p.getContact() << endl;
                cout << "Admission Status : " << (p.getAdmissionStatus() ? "Admitted" : "Not Admitted") << endl;
                cout << "Room Type : " << p.getRoomTypeAsString() << endl;

                p.displayHistory();

                return;
            }
        }

        cout << "ERROR: Patient ID '" << patientId << "' not found." << endl;
    }

    void displayDoctorInfo(int doctorId)
    {
        for (Doctor &d : doctors)
        {
            if (doctorId == d.getId())
            {
                cout << "\n========= Doctor Information =========\n";
                cout << "ID : " << d.getId() << endl;
                cout << "Name : " << d.getName() << endl;
                cout << "Department : " << d.getDepartment() << endl;
                cout << "Pending Appointments : " << d.getAppointmentCount() << endl
                     << endl;
                // Could Display Appointments too.
                return;
            }
        }

        cout << "ERROR: Doctor ID '" << doctorId << "' not found." << endl;
    }

    void dischargePatient(int patientId)
    {
        for (Patient &patient : patients)
        {
            if (patient.getId() == patientId)
            {
                if (!patient.getAdmissionStatus())
                {
                    cout << "ERROR: Patient '" << patient.getName() << "' is not admitted.\n";
                    return;
                }

                patient.dischargePatient();
                cout << "Patient '" << patient.getName() << "' has been discharged.\n";
                return;
            }
        }

        cout << "ERROR: Patient ID '" << patientId << "' not found.\n";
    }

    void requestTest(int patientId, string testName)
    {
        for (auto &patient : patients)
        {
            if (patient.getId() == patientId)
            {
                patient.requestTest(testName);
                cout << "Test '" << testName << "' requested for patient '" << patient.getName() << "'.\n";
                return;
            }
        }

        cout << "ERROR: Patient ID '" << patientId << "' not found." << endl;
    }

    void performTest(int patientId)
    {
        for (auto &patient : patients)
        {
            if (patient.getId() == patientId)
            {
                string result = patient.performTest();
                cout << "Patient '" << patient.getName() << "' performed " << result << " test." << endl;
                return;
            }
        }

        cout << "ERROR: Patient ID '" << patientId << "' not found." << endl;
    }

    void seePatient(int doctorId)
    {
        for (Doctor &d : doctors)
        {
            if (d.getId() == doctorId)
            {
                int patientId = d.seePatient();
                if (patientId == -1)
                {
                    cout << "No patients in queue for " << d.getName() << ".\n";
                }
                else
                {
                    cout << d.getName() << " is now seeing patient with ID: " << patientId << ".\n";
                }
                return;
            }
        }

        cout << "ERROR: Doctor ID '" << doctorId << "' not found.\n";
    }
};

// Main App. loop: displays top-level menu and routes user input.
void run(Hospital &hospital)
{
    int mainChoice;

    do
    {
        cout << "\n========== HOSPITAL MANAGEMENT SYSTEM ==========\n\n";
        cout << "1. Patient Management\n";
        cout << "2. Doctor Management\n";
        cout << "3. Emergency Management\n";
        cout << "0. Exit\n";
        cout << "\n-> Enter your choice: ";
        cin >> mainChoice;

        switch (mainChoice)
        {
        case 1: // Patient Management
        {
            int patientChoice;
            do
            {
                cout << "\n\n--- Patient Management ---\n\n";
                cout << "1. Register Patient\n";
                cout << "2. Admit Patient\n";
                cout << "3. Discharge Patient\n";
                cout << "4. Request Medical Test\n";
                cout << "5. Perform Medical Test\n";
                cout << "6. View Patient Info\n";
                cout << "0. Back to Main Menu\n";
                cout << "\n-> Enter your choice: ";
                cin >> patientChoice;
                cout << endl;

                switch (patientChoice)
                {
                case 1:
                {
                    string name, contact;
                    int age;
                    cout << "Enter name: ";
                    cin.ignore();
                    getline(cin, name);
                    cout << "Enter age: ";
                    cin >> age;
                    cout << "Enter contact number: ";
                    cin.ignore();
                    getline(cin, contact);
                    int id = hospital.registerPatient(name, age, contact);
                    cout << "Patient registered with ID: " << id << endl;
                    break;
                }
                case 2:
                {
                    int id, room;
                    cout << "Enter patient ID: ";
                    cin >> id;
                    cout << "0. General\n1. ICU\n2. Private\n3. Semi-Private\nRoom type: ";
                    cin >> room;
                    if (room < 0 || room > 3)
                    {
                        cout << "ERROR: Invalid room type.\n";
                        break;
                    }
                    hospital.admitPatient(id, static_cast<RoomType>(room));
                    hospital.savePatients();
                    break;
                }
                case 3:
                {
                    int id;
                    cout << "Enter patient ID: ";
                    cin >> id;
                    hospital.dischargePatient(id);
                    hospital.savePatients();
                    break;
                }
                case 4:
                {
                    int id;
                    string test;
                    cout << "Enter patient ID: ";
                    cin >> id;
                    cin.ignore();
                    cout << "Enter test name: ";
                    getline(cin, test);
                    hospital.requestTest(id, test);
                    hospital.savePatients();
                    break;
                }
                case 5:
                {
                    int id;
                    cout << "Enter patient ID: ";
                    cin >> id;
                    hospital.performTest(id);
                    hospital.savePatients();
                    break;
                }
                case 6:
                {
                    int id;
                    cout << "Enter patient ID: ";
                    cin >> id;
                    hospital.displayPatientInfo(id);
                    break;
                }
                }
            } while (patientChoice != 0);
            break;
        }

        case 2: // Doctor Management
        {
            int doctorChoice;
            do
            {
                cout << "\n\n--- Doctor Management ---\n\n";
                cout << "1. Add Doctor\n";
                cout << "2. Book Appointment\n";
                cout << "3. View Doctor Info\n";
                cout << "4. See Next Patient\n";
                cout << "0. Back to Main Menu\n";
                cout << "\n-> Enter your choice: ";
                cin >> doctorChoice;
                cout << endl;

                switch (doctorChoice)
                {
                case 1:
                {
                    string name;
                    int dept;
                    cout << "Enter doctor's name (Dr. Name): ";
                    cin.ignore();
                    getline(cin, name);
                    cout << "0. Cardiology\n1. Neurology\n2. Orthopedics\n3. Pediatrics\n4. Emergency\n5. General\nDepartment: ";
                    cin >> dept;
                    if (dept < 0 || dept > 5)
                    {
                        cout << "ERROR: Invalid department.\n";
                        break;
                    }
                    int id = hospital.addDoctor(name, static_cast<Department>(dept));
                    cout << "Doctor added with ID: " << id << endl;
                    break;
                }
                case 2:
                {
                    int docId, patId;
                    cout << "Enter doctor ID: ";
                    cin >> docId;
                    cout << "Enter patient ID: ";
                    cin >> patId;
                    hospital.bookAppointment(docId, patId);
                    break;
                }
                case 3:
                {
                    int id;
                    cout << "Enter doctor ID: ";
                    cin >> id;
                    hospital.displayDoctorInfo(id);
                    break;
                }
                case 4:
                {
                    int id;
                    cout << "Enter doctor ID: ";
                    cin >> id;
                    hospital.seePatient(id);
                    hospital.saveDoctors(); // To Save Updated Count
                    break;
                }
                }
            } while (doctorChoice != 0);
            break;
        }

        case 3: // Emergency Management
        {
            int emergencyChoice;
            do
            {
                cout << "\n\n--- Emergency Management ---\n\n";
                cout << "1. Add Emergency Case\n";
                cout << "2. Handle Emergency Case\n";
                cout << "0. Back to Main Menu\n";
                cout << "\n-> Enter your choice: ";
                cin >> emergencyChoice;
                cout << endl;

                switch (emergencyChoice)
                {
                case 1:
                {
                    int id;
                    cout << "Enter patient ID: ";
                    cin >> id;
                    hospital.addEmergency(id);
                    break;
                }
                case 2:
                {
                    hospital.handleEmergency();
                    break;
                }
                }
            } while (emergencyChoice != 0);
            break;
        }

        case 0:
            cout << "Exiting system. Saving data...\n";
            hospital.savePatients();
            hospital.saveDoctors();
            break;

        default:
            cout << "ERROR: Invalid option. Try again.\n";
        }

    } while (mainChoice != 0);
}

// ========== MAIN PROGRAM ========== //
int main()
{
    Hospital hospital;
    run(hospital);
    return 0;
}