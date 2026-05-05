#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "HashTable.h"
#include <string>

// ============================================================
//  FileManager  –  handles CSV persistence for the HashTable
// ============================================================
class FileManager {
public:
    // Save all records to a CSV file; returns true on success
    static bool saveToCSV(const HashTable& ht, const std::string& filename);

    // Load records from a CSV file into the hash table; returns # loaded
    static int loadFromCSV(HashTable& ht, const std::string& filename);

private:
    // Escape a string field for CSV (wrap in quotes, escape internal quotes)
    static std::string escapeCSV(const std::string& field);

    // Parse a CSV line into individual fields
    static std::vector<std::string> parseCSVLine(const std::string& line);
};

#endif // FILEMANAGER_H