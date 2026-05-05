#ifndef PATIENTRECORD_H
#define PATIENTRECORD_H

#include <string>


//  PatientRecord
//  Holds all demographic / clinical info for one patient.

struct PatientRecord {
    int         patientID;
    std::string firstName;
    std::string lastName;
    int         age;
    std::string gender;         // "Male", "Female", "Other"
    std::string bloodType;      // e.g. "A+", "O-"
    std::string diagnosis;
    std::string medication;
    std::string phoneNumber;
    std::string email;
    std::string admissionDate;  // YYYY-MM-DD

    PatientRecord()
        : patientID(0), age(0) {}

    PatientRecord(int id,
                  const std::string& fn,
                  const std::string& ln,
                  int ag,
                  const std::string& gen,
                  const std::string& bt,
                  const std::string& diag,
                  const std::string& med,
                  const std::string& phone,
                  const std::string& em,
                  const std::string& date)
        : patientID(id), firstName(fn), lastName(ln), age(ag),
          gender(gen), bloodType(bt), diagnosis(diag), medication(med),
          phoneNumber(phone), email(em), admissionDate(date) {}
};

#endif // PATIENTRECORD_H