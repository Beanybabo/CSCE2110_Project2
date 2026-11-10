#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
#include "Facility.h"

// initial number of buckets. chains can grow so there is no facility limit
const int DEFAULT_HASH_CAPACITY = 13;

class HashTable {
public:
    HashTable(int capacity = DEFAULT_HASH_CAPACITY);

    bool insert(const Facility& facility);

    const Facility* find(const std::string& id) const;

    int getCapacity() const;
    int getEntryCount() const;
    int getCollisionCount() const;

    int hashFunction(const std::string& key) const;

private:
    std::vector<std::vector<Facility> > buckets;
    int capacity;
    int entryCount;
    int collisionCount;
};

#endif