// ============================================================
//  MedData - Patient Record Lookup System
//  CSC 307 Data Structures and Algorithm Analysis
//  Spring 2026
//
//  main.cpp  -  Application entry point, menu system,
//               input validation, and all CRUD commands.
// ============================================================

#include "HashTable.h"
#include "FileManager.h"
#include "PatientRecord.h"

#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <vector>

// ------------------------------------------------------------
//  Constants
// ------------------------------------------------------------
const std::string DATA_FILE    = "patients.csv";
const int         TABLE_CAP    = 101;   // prime bucket count

// ------------------------------------------------------------
//  Forward declarations
// ------------------------------------------------------------
void printBanner();
void printMenu();
void printDivider(char c = '-', int width = 60);
void printRecord(const PatientRecord& r);
void printAllRecords(const HashTable& ht);

void cmdAddPatient(HashTable& ht);
void cmdSearchPatient(HashTable& ht);
void cmdUpdatePatient(HashTable& ht);
void cmdDeletePatient(HashTable& ht);
void cmdListAll(const HashTable& ht);
void cmdSave(const HashTable& ht);
void cmdStats(const HashTable& ht);

// -- Input helpers --
int         getIntInput(const std::string& prompt, int minVal, int maxVal);
std::string getStringInput(const std::string& prompt, int maxLen = 100);
std::string getStringInputOptional(const std::string& prompt);
bool        confirmAction(const std::string& message);

// -- Validators --
bool isValidDate(const std::string& date);       // YYYY-MM-DD
bool isValidBloodType(const std::string& bt);
bool isValidGender(const std::string& g);
bool isValidPhone(const std::string& phone);
bool isValidEmail(const std::string& email);
bool isValidAge(int age);

// ============================================================
//  main
// ============================================================
int main() {
    printBanner();

    HashTable ht(TABLE_CAP);

    // Auto-load existing data on startup
    std::cout << "\n[System] Loading patient data...\n";
    int loaded = FileManager::loadFromCSV(ht, DATA_FILE);
    if (loaded == 0) {
        std::cout << "[System] No existing data found. Starting fresh.\n";
    }

    printDivider();

    bool running = true;
    while (running) {
        printMenu();

        int choice = getIntInput("Enter choice", 0, 7);

        switch (choice) {
            case 1: cmdAddPatient(ht);    break;
            case 2: cmdSearchPatient(ht); break;
            case 3: cmdUpdatePatient(ht); break;
            case 4: cmdDeletePatient(ht); break;
            case 5: cmdListAll(ht);       break;
            case 6: cmdSave(ht);          break;
            case 7: cmdStats(ht);         break;
            case 0:
                std::cout << "\n[System] Saving data before exit...\n";
                FileManager::saveToCSV(ht, DATA_FILE);
                std::cout << "[System] Goodbye.\n\n";
                running = false;
                break;
            default:
                std::cout << "[Error] Invalid option. Please try again.\n";
        }
    }

    return 0;
}

// ============================================================
//  DISPLAY HELPERS
// ============================================================

void printBanner() {
    printDivider('=');
    std::cout << "         MedData - Patient Record Lookup System\n";
    std::cout << "         CSC 307 | Spring 2026\n";
    printDivider('=');
}

void printMenu() {
    std::cout << "\n";
    printDivider();
    std::cout << "  MAIN MENU\n";
    printDivider();
    std::cout << "  [1] Add Patient\n";
    std::cout << "  [2] Search Patient by ID\n";
    std::cout << "  [3] Update Patient Record\n";
    std::cout << "  [4] Delete Patient Record\n";
    std::cout << "  [5] List All Patients\n";
    std::cout << "  [6] Save to File\n";
    std::cout << "  [7] System Stats\n";
    std::cout << "  [0] Exit\n";
    printDivider();
}

void printDivider(char c, int width) {
    std::cout << std::string(width, c) << "\n";
}

void printRecord(const PatientRecord& r) {
    printDivider();
    std::cout << std::left;
    std::cout << "  " << std::setw(20) << "Patient ID:"
              << r.patientID << "\n";
    std::cout << "  " << std::setw(20) << "Name:"
              << r.firstName << " " << r.lastName << "\n";
    std::cout << "  " << std::setw(20) << "Age:"
              << r.age << "\n";
    std::cout << "  " << std::setw(20) << "Gender:"
              << r.gender << "\n";
    std::cout << "  " << std::setw(20) << "Blood Type:"
              << r.bloodType << "\n";
    std::cout << "  " << std::setw(20) << "Diagnosis:"
              << r.diagnosis << "\n";
    std::cout << "  " << std::setw(20) << "Medication:"
              << r.medication << "\n";
    std::cout << "  " << std::setw(20) << "Phone:"
              << r.phoneNumber << "\n";
    std::cout << "  " << std::setw(20) << "Email:"
              << r.email << "\n";
    std::cout << "  " << std::setw(20) << "Admission Date:"
              << r.admissionDate << "\n";
    printDivider();
}

void printAllRecords(const HashTable& ht) {
    std::vector<PatientRecord> records = ht.getAllRecords();

    if (records.empty()) {
        std::cout << "\n[Info] No patient records found.\n";
        return;
    }

    // Sort by patient ID for consistent display
    std::sort(records.begin(), records.end(),
              [](const PatientRecord& a, const PatientRecord& b) {
                  return a.patientID < b.patientID;
              });

    std::cout << "\n  Total records: " << records.size() << "\n";
    for (const PatientRecord& r : records) {
        printRecord(r);
    }
}

// ============================================================
//  CRUD COMMANDS
// ============================================================

// ------------------------------------------------------------
//  [1] Add Patient
// ------------------------------------------------------------
void cmdAddPatient(HashTable& ht) {
    std::cout << "\n--- ADD PATIENT ---\n";

    // Patient ID
    int id = getIntInput("Enter Patient ID (1 - 999999)", 1, 999999);

    // Check for duplicate before collecting all fields
    if (ht.search(id) != nullptr) {
        std::cout << "[Error] Patient ID " << id
                  << " already exists. Use Update to modify.\n";
        return;
    }

    // First / Last Name
    std::string firstName, lastName;
    while (true) {
        firstName = getStringInput("First Name", 50);
        if (!firstName.empty()) break;
        std::cout << "[Error] First name cannot be empty.\n";
    }
    while (true) {
        lastName = getStringInput("Last Name", 50);
        if (!lastName.empty()) break;
        std::cout << "[Error] Last name cannot be empty.\n";
    }

    // Age
    int age = getIntInput("Age (0 - 130)", 0, 130);

    // Gender
    std::string gender;
    while (true) {
        gender = getStringInput("Gender (Male / Female / Other)", 10);
        // Normalize capitalization
        if (!gender.empty()) {
            gender[0] = toupper(gender[0]);
            for (size_t i = 1; i < gender.size(); ++i)
                gender[i] = tolower(gender[i]);
        }
        if (isValidGender(gender)) break;
        std::cout << "[Error] Please enter Male, Female, or Other.\n";
    }

    // Blood Type
    std::string bloodType;
    while (true) {
        bloodType = getStringInput("Blood Type (e.g. A+, O-, AB+)", 5);
        // Uppercase
        std::transform(bloodType.begin(), bloodType.end(),
                       bloodType.begin(), ::toupper);
        if (isValidBloodType(bloodType)) break;
        std::cout << "[Error] Invalid blood type. "
                     "Valid: A+, A-, B+, B-, AB+, AB-, O+, O-\n";
    }

    // Diagnosis
    std::string diagnosis = getStringInput("Diagnosis", 200);
    if (diagnosis.empty()) diagnosis = "N/A";

    // Medication
    std::string medication = getStringInput("Medication", 200);
    if (medication.empty()) medication = "N/A";

    // Phone
    std::string phone;
    while (true) {
        phone = getStringInput("Phone Number (digits only, 10 digits)", 15);
        if (isValidPhone(phone)) break;
        std::cout << "[Error] Phone must be exactly 10 digits.\n";
    }

    // Email
    std::string email;
    while (true) {
        email = getStringInput("Email Address", 100);
        if (isValidEmail(email)) break;
        std::cout << "[Error] Invalid email. Must contain '@' and '.'.\n";
    }

    // Admission Date
    std::string admDate;
    while (true) {
        admDate = getStringInput("Admission Date (YYYY-MM-DD)", 10);
        if (isValidDate(admDate)) break;
        std::cout << "[Error] Invalid date format. Use YYYY-MM-DD "
                     "(e.g. 2026-01-15).\n";
    }

    // Build and insert
    PatientRecord rec(id, firstName, lastName, age, gender, bloodType,
                      diagnosis, medication, phone, email, admDate);
    ht.insert(id, rec);

    std::cout << "\n[Success] Patient " << id << " ("
              << firstName << " " << lastName << ") added.\n";
}

// ------------------------------------------------------------
//  [2] Search Patient
// ------------------------------------------------------------
void cmdSearchPatient(HashTable& ht) {
    std::cout << "\n--- SEARCH PATIENT ---\n";

    int id = getIntInput("Enter Patient ID to search", 1, 999999);

    PatientRecord* found = ht.search(id);
    if (found == nullptr) {
        std::cout << "[Info] No patient found with ID " << id << ".\n";
    } else {
        std::cout << "\n[Found] Record for Patient ID " << id << ":\n";
        printRecord(*found);
    }
}

// ------------------------------------------------------------
//  [3] Update Patient
// ------------------------------------------------------------
void cmdUpdatePatient(HashTable& ht) {
    std::cout << "\n--- UPDATE PATIENT ---\n";

    int id = getIntInput("Enter Patient ID to update", 1, 999999);

    PatientRecord* existing = ht.search(id);
    if (existing == nullptr) {
        std::cout << "[Error] No patient found with ID " << id << ".\n";
        return;
    }

    std::cout << "\n[Current Record]:\n";
    printRecord(*existing);

    std::cout << "\nEnter new values (press Enter to keep current value):\n\n";

    // Helper lambda: keep old value if user enters nothing
    auto keepOrReplace = [](const std::string& prompt,
                             const std::string& current,
                             int maxLen = 100) -> std::string {
        std::cout << prompt << " [" << current << "]: ";
        std::string input;
        std::getline(std::cin, input);
        if (input.empty()) return current;
        if ((int)input.size() > maxLen) input = input.substr(0, maxLen);
        return input;
    };

    std::string firstName = keepOrReplace("First Name", existing->firstName, 50);
    std::string lastName  = keepOrReplace("Last Name",  existing->lastName,  50);

    // Age
    std::cout << "Age [" << existing->age << "]: ";
    std::string ageStr;
    std::getline(std::cin, ageStr);
    int age = existing->age;
    if (!ageStr.empty()) {
        try {
            int tmp = std::stoi(ageStr);
            if (isValidAge(tmp)) age = tmp;
            else std::cout << "[Warning] Invalid age. Keeping current.\n";
        } catch (...) {
            std::cout << "[Warning] Invalid input. Keeping current age.\n";
        }
    }

    // Gender
    std::string gender;
    while (true) {
        std::string input = keepOrReplace("Gender (Male/Female/Other)",
                                          existing->gender, 10);
        if (!input.empty()) input[0] = toupper(input[0]);
        for (size_t i = 1; i < input.size(); ++i) input[i] = tolower(input[i]);
        if (isValidGender(input)) { gender = input; break; }
        std::cout << "[Error] Please enter Male, Female, or Other.\n";
    }

    // Blood Type
    std::string bloodType;
    while (true) {
        std::string input = keepOrReplace("Blood Type", existing->bloodType, 5);
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        if (isValidBloodType(input)) { bloodType = input; break; }
        std::cout << "[Error] Invalid blood type.\n";
    }

    std::string diagnosis = keepOrReplace("Diagnosis",  existing->diagnosis,  200);
    std::string medication= keepOrReplace("Medication", existing->medication, 200);

    // Phone
    std::string phone;
    while (true) {
        std::string input = keepOrReplace("Phone Number", existing->phoneNumber, 15);
        if (isValidPhone(input)) { phone = input; break; }
        std::cout << "[Error] Phone must be 10 digits.\n";
    }

    // Email
    std::string email;
    while (true) {
        std::string input = keepOrReplace("Email", existing->email, 100);
        if (isValidEmail(input)) { email = input; break; }
        std::cout << "[Error] Invalid email format.\n";
    }

    // Admission Date
    std::string admDate;
    while (true) {
        std::string input = keepOrReplace("Admission Date (YYYY-MM-DD)",
                                          existing->admissionDate, 10);
        if (isValidDate(input)) { admDate = input; break; }
        std::cout << "[Error] Invalid date. Use YYYY-MM-DD.\n";
    }

    if (!confirmAction("Confirm update? (y/n)")) {
        std::cout << "[Cancelled] No changes made.\n";
        return;
    }

    PatientRecord updated(id, firstName, lastName, age, gender, bloodType,
                          diagnosis, medication, phone, email, admDate);

    if (ht.update(id, updated)) {
        std::cout << "[Success] Patient " << id << " updated.\n";
    } else {
        std::cout << "[Error] Update failed.\n";
    }
}

// ------------------------------------------------------------
//  [4] Delete Patient
// ------------------------------------------------------------
void cmdDeletePatient(HashTable& ht) {
    std::cout << "\n--- DELETE PATIENT ---\n";

    int id = getIntInput("Enter Patient ID to delete", 1, 999999);

    PatientRecord* found = ht.search(id);
    if (found == nullptr) {
        std::cout << "[Error] No patient found with ID " << id << ".\n";
        return;
    }

    std::cout << "\n[Record to Delete]:\n";
    printRecord(*found);

    if (!confirmAction("Are you sure you want to delete this record? (y/n)")) {
        std::cout << "[Cancelled] Record not deleted.\n";
        return;
    }

    if (ht.deleteRecord(id)) {
        std::cout << "[Success] Patient " << id << " deleted.\n";
    } else {
        std::cout << "[Error] Deletion failed.\n";
    }
}

// ------------------------------------------------------------
//  [5] List All
// ------------------------------------------------------------
void cmdListAll(const HashTable& ht) {
    std::cout << "\n--- ALL PATIENT RECORDS ---\n";
    printAllRecords(ht);
}

// ------------------------------------------------------------
//  [6] Save
// ------------------------------------------------------------
void cmdSave(const HashTable& ht) {
    std::cout << "\n--- SAVE TO FILE ---\n";
    FileManager::saveToCSV(ht, DATA_FILE);
}

// ------------------------------------------------------------
//  [7] Stats
// ------------------------------------------------------------
void cmdStats(const HashTable& ht) {
    std::cout << "\n--- SYSTEM STATS ---\n";
    printDivider();
    std::cout << "  Records stored : " << ht.getSize()     << "\n";
    std::cout << "  Bucket count   : " << ht.getCapacity() << "\n";
    std::cout << "  Load factor    : " << std::fixed
              << std::setprecision(4)  << ht.loadFactor()  << "\n";
    std::cout << "  Data file      : " << DATA_FILE        << "\n";
    printDivider();
}

// ============================================================
//  INPUT HELPERS
// ============================================================

// Get a validated integer within [minVal, maxVal]
int getIntInput(const std::string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        std::cout << "  " << prompt << " (" << minVal
                  << "-" << maxVal << "): ";
        std::string line;
        std::getline(std::cin, line);

        // Strip whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty()) {
            std::cout << "[Error] Input cannot be empty.\n";
            continue;
        }

        bool allDigits = true;
        for (char c : line) {
            if (!isdigit(c)) { allDigits = false; break; }
        }
        if (!allDigits) {
            std::cout << "[Error] Please enter a whole number.\n";
            continue;
        }

        try {
            value = std::stoi(line);
        } catch (...) {
            std::cout << "[Error] Number out of range.\n";
            continue;
        }

        if (value < minVal || value > maxVal) {
            std::cout << "[Error] Please enter a value between "
                      << minVal << " and " << maxVal << ".\n";
            continue;
        }
        return value;
    }
}

// Get a non-empty trimmed string (with optional max length)
std::string getStringInput(const std::string& prompt, int maxLen) {
    while (true) {
        std::cout << "  " << prompt << ": ";
        std::string input;
        std::getline(std::cin, input);

        // Trim leading/trailing whitespace
        input.erase(0, input.find_first_not_of(" \t"));
        if (!input.empty())
            input.erase(input.find_last_not_of(" \t") + 1);

        if ((int)input.size() > maxLen) {
            std::cout << "[Warning] Input truncated to "
                      << maxLen << " characters.\n";
            input = input.substr(0, maxLen);
        }
        return input;  // caller decides if empty is ok
    }
}

// y/n confirmation
bool confirmAction(const std::string& message) {
    while (true) {
        std::cout << "  " << message << " ";
        std::string input;
        std::getline(std::cin, input);
        if (input.empty()) continue;
        char c = tolower(input[0]);
        if (c == 'y') return true;
        if (c == 'n') return false;
        std::cout << "[Error] Please enter 'y' or 'n'.\n";
    }
}

// ============================================================
//  VALIDATORS
// ============================================================

// Date must be YYYY-MM-DD with basic range checks
bool isValidDate(const std::string& date) {
    if (date.size() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;

    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }

    int year  = std::stoi(date.substr(0, 4));
    int month = std::stoi(date.substr(5, 2));
    int day   = std::stoi(date.substr(8, 2));

    if (year  < 1900 || year  > 2100) return false;
    if (month < 1    || month > 12  ) return false;
    if (day   < 1    || day   > 31  ) return false;

    // Days-per-month check
    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    // Leap year
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        daysInMonth[1] = 29;
    if (day > daysInMonth[month - 1]) return false;

    return true;
}

// Only the 8 standard ABO/Rh types
bool isValidBloodType(const std::string& bt) {
    static const std::string valid[] = {
        "A+","A-","B+","B-","AB+","AB-","O+","O-"
    };
    for (const std::string& v : valid) {
        if (bt == v) return true;
    }
    return false;
}

bool isValidGender(const std::string& g) {
    return g == "Male" || g == "Female" || g == "Other";
}

// Must be exactly 10 digits
bool isValidPhone(const std::string& phone) {
    if (phone.size() != 10) return false;
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Must contain '@' and at least one '.' after '@'
bool isValidEmail(const std::string& email) {
    size_t at = email.find('@');
    if (at == std::string::npos || at == 0) return false;
    size_t dot = email.find('.', at);
    if (dot == std::string::npos || dot == at + 1) return false;
    if (dot == email.size() - 1) return false;
    return true;
}

bool isValidAge(int age) {
    return age >= 0 && age <= 130;
}
