#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// ============================================================
//  escapeCSV  –  wraps field in double-quotes, escapes internals
// ============================================================
std::string FileManager::escapeCSV(const std::string& field) {
    std::string result = "\"";
    for (char c : field) {
        if (c == '"') result += "\"\"";  // double up quotes
        else          result += c;
    }
    result += "\"";
    return result;
}

// ============================================================
//  parseCSVLine  –  handles quoted fields with embedded commas
// ============================================================
std::vector<std::string> FileManager::parseCSVLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];

        if (c == '"') {
            if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                // Escaped quote inside quoted field
                field += '"';
                ++i;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (c == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    fields.push_back(field);  // last field
    return fields;
}

// ============================================================
//  saveToCSV
// ============================================================
bool FileManager::saveToCSV(const HashTable& ht, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[FileManager] Could not open file for writing: "
                  << filename << "\n";
        return false;
    }

    // Header row
    file << "patientID,firstName,lastName,age,gender,bloodType,"
            "diagnosis,medication,phoneNumber,email,admissionDate\n";

    std::vector<PatientRecord> records = ht.getAllRecords();
    for (const PatientRecord& r : records) {
        file << r.patientID           << ","
             << escapeCSV(r.firstName)  << ","
             << escapeCSV(r.lastName)   << ","
             << r.age                  << ","
             << escapeCSV(r.gender)     << ","
             << escapeCSV(r.bloodType)  << ","
             << escapeCSV(r.diagnosis)  << ","
             << escapeCSV(r.medication) << ","
             << escapeCSV(r.phoneNumber)<< ","
             << escapeCSV(r.email)      << ","
             << escapeCSV(r.admissionDate) << "\n";
    }

    file.close();
    std::cout << "[FileManager] Saved " << records.size()
              << " record(s) to " << filename << "\n";
    return true;
}

// ============================================================
//  loadFromCSV
// ============================================================
int FileManager::loadFromCSV(HashTable& ht, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // Not an error on first run — file just doesn't exist yet
        return 0;
    }

    std::string line;
    // Skip header
    if (!std::getline(file, line)) {
        return 0;
    }

    int loaded = 0;
    int lineNum = 1;

    while (std::getline(file, line)) {
        ++lineNum;
        if (line.empty()) continue;

        std::vector<std::string> fields = parseCSVLine(line);
        if (fields.size() != 11) {
            std::cerr << "[FileManager] Skipping malformed line "
                      << lineNum << " (expected 11 fields, got "
                      << fields.size() << ")\n";
            continue;
        }

        try {
            int id  = std::stoi(fields[0]);
            int age = std::stoi(fields[3]);

            PatientRecord r(id,
                            fields[1],  // firstName
                            fields[2],  // lastName
                            age,
                            fields[4],  // gender
                            fields[5],  // bloodType
                            fields[6],  // diagnosis
                            fields[7],  // medication
                            fields[8],  // phoneNumber
                            fields[9],  // email
                            fields[10]  // admissionDate
                            );
            ht.insert(id, r);
            ++loaded;
        } catch (const std::exception& e) {
            std::cerr << "[FileManager] Parse error on line "
                      << lineNum << ": " << e.what() << "\n";
        }
    }

    file.close();
    std::cout << "[FileManager] Loaded " << loaded
              << " record(s) from " << filename << "\n";
    return loaded;
}