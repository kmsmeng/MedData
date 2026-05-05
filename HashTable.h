#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "PatientRecord.h"
#include <vector>
#include <string>

// ============================================================
//  Node  –  singly-linked list node used for chaining
// ============================================================
struct Node {
    int           patientID;
    PatientRecord record;
    Node*         next;

    Node(int id, const PatientRecord& rec)
        : patientID(id), record(rec), next(nullptr) {}
};

// ============================================================
//  HashTable  –  chained hash table keyed on patientID
// ============================================================
class HashTable {
private:
    std::vector<Node*> table;   // array of bucket heads
    int size;                   // number of records stored
    int capacity;               // number of buckets

    // Hash function: division method
    int hashFunction(int key) const;

public:
    // Constructor / Destructor
    explicit HashTable(int cap = 101);
    ~HashTable();

    // Core DSA operations
    void insert(int patientID, const PatientRecord& record);
    PatientRecord* search(int patientID);
    bool update(int patientID, const PatientRecord& newRecord);
    bool deleteRecord(int patientID);

    // Utility
    int  getSize()     const { return size; }
    int  getCapacity() const { return capacity; }
    bool isEmpty()     const { return size == 0; }

    // Retrieve all records (for display / CSV export)
    std::vector<PatientRecord> getAllRecords() const;

    // Load factor for diagnostics
    double loadFactor() const;
};

#endif // HASHTABLE_H