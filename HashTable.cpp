#include "HashTable.h"
#include <iostream>
#include <stdexcept>

// ============================================================
//  Constructor
// ============================================================
HashTable::HashTable(int cap)
    : size(0), capacity(cap)
{
    if (capacity <= 0) capacity = 101;
    table.resize(capacity, nullptr);
}

// ============================================================
//  Destructor  –  free every node in every bucket
// ============================================================
HashTable::~HashTable() {
    for (int i = 0; i < capacity; ++i) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        table[i] = nullptr;
    }
}

// ============================================================
//  hashFunction  –  division method, always returns [0, capacity)
// ============================================================
int HashTable::hashFunction(int key) const {
    // Handle negative IDs gracefully
    return ((key % capacity) + capacity) % capacity;
}

// ============================================================
//  insert  –  add a new record; duplicate IDs are rejected
// ============================================================
void HashTable::insert(int patientID, const PatientRecord& record) {
    int index = hashFunction(patientID);

    // Check for duplicate patient ID
    Node* current = table[index];
    while (current != nullptr) {
        if (current->patientID == patientID) {
            std::cerr << "[HashTable] Insert failed: Patient ID "
                      << patientID << " already exists.\n";
            return;
        }
        current = current->next;
    }

    // Insert at head of chain (O(1))
    Node* newNode = new Node(patientID, record);
    newNode->next  = table[index];
    table[index]   = newNode;
    ++size;
}

// ============================================================
//  search  –  returns pointer to record, or nullptr if not found
// ============================================================
PatientRecord* HashTable::search(int patientID) {
    int index = hashFunction(patientID);
    Node* current = table[index];

    while (current != nullptr) {
        if (current->patientID == patientID) {
            return &(current->record);
        }
        current = current->next;
    }
    return nullptr;  // not found
}

// ============================================================
//  update  –  replace record data; returns false if ID missing
// ============================================================
bool HashTable::update(int patientID, const PatientRecord& newRecord) {
    PatientRecord* existing = search(patientID);
    if (existing == nullptr) {
        return false;
    }
    *existing = newRecord;
    existing->patientID = patientID;  // ensure key integrity
    return true;
}

// ============================================================
//  deleteRecord  –  unlink and free the node; returns false if missing
// ============================================================
bool HashTable::deleteRecord(int patientID) {
    int index = hashFunction(patientID);
    Node* current = table[index];
    Node* prev    = nullptr;

    while (current != nullptr) {
        if (current->patientID == patientID) {
            // Unlink
            if (prev == nullptr) {
                table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            --size;
            return true;
        }
        prev    = current;
        current = current->next;
    }
    return false;  // not found
}

// ============================================================
//  getAllRecords  –  collect all records across all buckets
// ============================================================
std::vector<PatientRecord> HashTable::getAllRecords() const {
    std::vector<PatientRecord> records;
    records.reserve(size);

    for (int i = 0; i < capacity; ++i) {
        Node* current = table[i];
        while (current != nullptr) {
            records.push_back(current->record);
            current = current->next;
        }
    }
    return records;
}

// ============================================================
//  loadFactor
// ============================================================
double HashTable::loadFactor() const {
    return static_cast<double>(size) / capacity;
}