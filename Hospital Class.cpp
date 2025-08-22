#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

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
        medicalHistory.push("Admitted to room type " + to_string(type)); // can add "on " + current date
        // may use addMedicalRecord method instead
        cout << "Patient is admitted successfully !" << endl;
    }

    void dischargePatient()
    {
        // I will check this if-condition
        if (isAdmitted)
        {
            isAdmitted = false;
            medicalHistory.push("Discharged from hospital"); // can add "on " + current date
            // may use addMedicalRecord method instead
            cout << "Patient is discharged successfully !" << endl;
        }
        else
        {
            cout << "Patient is not admitted !" << endl;
        }
    }

    void addMedicalRecord(string record)
    {
        medicalHistory.push(record);
    };

    void requestTest(string testName)
    {
        testQueue.push(testName);
        medicalHistory.push("Requested test: " + testName);
    };
    string performTest()
    {
        if (testQueue.empty())
        {
            return "No tests are pending";
        }

        string testName = testQueue.front();
        testQueue.pop();
        medicalHistory.push("Performed test: " + testName);

        return testName;
    };

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
            // Displaying history in reverse order (most recent first/LIFO)
            cout << "====== Patient's Medical History ======" << endl;
            cout << "Patient Name: " << name << endl;
            cout << "Patient ID: " << id << endl;
            cout << "---------------------------------------" << endl;
            while (!tempHistory.empty())
            {
                cout << tempHistory.top() << endl;
                tempHistory.pop();
            }
        }
    }

    int getId()
    {
        return id;
    };

    string getName()
    {
        return name;
    }

    bool getAdmissionStatus()
    {

        return isAdmitted;
    };
};

// ========== DOCTOR CLASS ========== //
class Doctor
{
private:
    int id;
    string name;
    Department department;
    queue<int> appointmentQueue;

public:
    Doctor(int did, string n, Department d);

    void addAppointment(int patientId);
    int seePatient();

    int getId();
    string getName();
    string getDepartment();
};

// ========== HOSPITAL CLASS ========== //
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
    }

    int registerPatient(string name, int age, string contact)
    {
        patientCounter++;
        Patient newPatient(patientCounter, name, age, contact);
        patients.push_back(newPatient);
        return patientCounter;
    }

    int addDoctor(string name, Department dept)
    {
        doctorCounter++;
        Doctor newDoctor(doctorCounter, name, dept);
        doctors.push_back(newDoctor);
        return doctorCounter;
    }

    void admitPatient(int patientId, RoomType type)
    {
        bool found = false;
        for (auto& patient : patients)
        {
            if (patient.getId() == patientId)
            {
                found = true;
                if (patient.getAdmissionStatus())
                {
                    cout << "patient" << patient.getName();
                    cout << "Patient is already admitted !" << endl;
                }
                else
                {
                    patient.admitPatient(type);
                    cout << "patient :" << patient.getName();
                    cout << "Patient is admitted successfully to room type " << type << endl;
                }
                break;
            }
        }
        if (!found)
        {
            cout << "Patient with ID: " << patientId << "is not found" << endl;
        }
    }

    void addEmergency(int patientId)
    {
        emergencyQueue.push(patientId);
    }

    int handleEmergency()
    {
        if (emergencyQueue.empty())
        {
            return -1;
        }

        int patientId = emergencyQueue.front();
        emergencyQueue.pop();
        return patientId;
    }

    void bookAppointment(int doctorId, int patientId)
    {
        bool foundDoc = false;
        bool foundPat = false;

        for (Doctor& d : doctors)
        {
            if (doctorId == d.getId())
            {
                d.addAppointment(patientId);
                foundDoc = true;
            }
        }

        if (foundDoc)
        {
            for (Patient& p : patients)
            {
                if (patientId == p.getId())
                {
                    p.addMedicalRecord("Appointment booked with Doctor ID " + to_string(doctorId));
                    foundPat = true;
                }
            }
            if (foundPat)
            {
                cout << "Booking confirmed." << endl;
            }
            else
            {
                cout << "Booking cancelled , Patient ID not found ." << endl;
            }
        }

        else
        {
            cout << "Booking cancelled , Wrong doctor ID." << endl;
        }
    }

    void displayPatientInfo(int patientId)
    {
        bool found = false;

        for (Patient& p : patients)
        {
            if (p.getId() == patientId)
            {
                cout << "Patient ID      : " << p.getId() << endl;
                cout << "Patient Name    : " << p.getName() << endl;
               

                if (p.getAdmissionStatus())
                    cout << "Admission Status: Admitted" << endl;
                else
                    cout << "Admission Status: Not Admitted" << endl;

                found = true;
                break;
            }
        }

        if (!found)
        {
            cout << "Patient with ID " << patientId << " not found." << endl;
        }
    }

    void displayDoctorInfo(int doctorId)
    {
        bool foundDoc = false;
        for (Doctor& d : doctors)
        {
            if (doctorId == d.getId())
            {
                cout << "Doctor ID       : " << d.getId() << endl;
                cout << "Doctor Name     : " << d.getName() << endl;
                cout << "Doctor Department: " << d.getDepartment() << endl;
                foundDoc = true;
                break; 
            }
        }
        if (!foundDoc)
        {
            cout << "Wrong doctor ID." << endl;
        }
    }
};

// ========== MAIN PROGRAM ========== //
int main()
{

    /// Testing Patient Class
    Patient p1(1, "John Doe", 30, "555-1234");
    cout << "Patient created: " << p1.getName() << endl;
    p1.admitPatient(PRIVATE_ROOM); // if you want to see the patient is not admitted comment this line and run again
    p1.dischargePatient();
    p1.displayHistory();
    cout << "Patient name is: " << p1.getName() << endl;
    // end of my testing part

    Hospital hospital;

    // Test Case 1: Registering patients
    int p1 = hospital.registerPatient("John Doe", 35, "555-1234");
    int p2 = hospital.registerPatient("Jane Smith", 28, "555-5678");
    int p3 = hospital.registerPatient("Mike Johnson", 45, "555-9012");

    // Test Case 2: Adding doctors
    int d1 = hospital.addDoctor("Dr. Smith", CARDIOLOGY);
    int d2 = hospital.addDoctor("Dr. Brown", NEUROLOGY);
    int d3 = hospital.addDoctor("Dr. Lee", PEDIATRICS);

    // Test Case 3: Admitting patients
    hospital.admitPatient(p1, PRIVATE_ROOM);
    hospital.admitPatient(p2, ICU);
    // Try admitting already admitted patient
    hospital.admitPatient(p1, SEMI_PRIVATE);

    // Test Case 4: Booking appointments
    hospital.bookAppointment(d1, p1);
    hospital.bookAppointment(d1, p2);
    hospital.bookAppointment(d2, p3);
    // Try booking with invalid doctor/patient
    hospital.bookAppointment(999, p1); // Invalid doctor
    hospital.bookAppointment(d1, 999); // Invalid patient

    // Test Case 5: Handling medical tests
    // These would normally be called on Patient objects
    // In a real implementation, we'd need a way to access patients

    // Test Case 6: Emergency cases
    hospital.addEmergency(p3);
    hospital.addEmergency(p1);
    int emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency(); // No more emergencies

    // Test Case 7: Discharging patients
    // Would normally call dischargePatient() on Patient objects

    // Test Case 8: Displaying information
    hospital.displayPatientInfo(p1);
    hospital.displayPatientInfo(p2);
    hospital.displayPatientInfo(999); // Invalid patient

    hospital.displayDoctorInfo(d1);
    hospital.displayDoctorInfo(d2);
    hospital.displayDoctorInfo(999); // Invalid doctor

    // Test Case 9: Doctor seeing patients
    // These would normally be called on Doctor objects
    // In a real implementation, we'd need a way to access doctors

    // Test Case 10: Edge cases
    Hospital emptyHospital;
    emptyHospital.displayPatientInfo(1); // No patients
    emptyHospital.displayDoctorInfo(1);  // No doctors
    emptyHospital.handleEmergency();     // No emergencies

    return 0;
}
