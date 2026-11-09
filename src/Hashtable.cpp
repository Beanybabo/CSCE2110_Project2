#include "../Include/Hashtable.h"
#include "../Include/Stringutils.h"
 
// creates an empty table with 'capacity' buckets
HashTable::HashTable(int capacity) {
    if (capacity < 1) {
        capacity = 1;
    }
    this->capacity = capacity;
    buckets.resize(capacity);
    entryCount = 0;
    collisionCount = 0;
}
 
// Hash function: hash = (hash * 31 + character) for every character of the uppercase key, taken modulo the capacity after each step
int HashTable::hashFunction(const std::string& key) const {
    std::string upperKey = toUpper(key);   // F004 and f004 must match
    long long hash = 0;
    for (size_t i = 0; i < upperKey.size(); i++) {
        hash = (hash * 31 + static_cast<unsigned char>(upperKey[i])) % capacity;
    }
    return static_cast<int>(hash);
}
 
// inserts a facility. Collision = the target bucket already has an entry.
// duplicates are rejected and do not change the collision count.
bool HashTable::insert(const Facility& facility) {
    Facility item = facility;
    item.id = toUpper(trim(item.id));
 
    std::vector<Facility>& chain = buckets[hashFunction(item.id)];
 
    for (size_t i = 0; i < chain.size(); i++) {
        if (chain[i].id == item.id) {
            return false;
        }
    }
 
    if (!chain.empty()) {
        collisionCount++;
    }
    chain.push_back(item);
    entryCount++;
    return true;
}
 
// finds a facility by ID. Only the matching bucket is searched.
const Facility* HashTable::find(const std::string& id) const {
    std::string key = toUpper(trim(id));
    const std::vector<Facility>& chain = buckets[hashFunction(key)];
    for (size_t i = 0; i < chain.size(); i++) {
        if (chain[i].id == key) {
            return &chain[i];
        }
    }
    return nullptr;
}
 
int HashTable::getCapacity() const { return capacity; }
int HashTable::getEntryCount() const { return entryCount; }
int HashTable::getCollisionCount() const { return collisionCount; }