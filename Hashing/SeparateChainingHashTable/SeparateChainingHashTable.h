/**
 * @file SeparateChainingHashTable.h
 * @author Kofi Boateng
 * @brief
 * @version 0.1
 * @date 2022-10-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include <iostream>
#include <stdexcept>
#include <ostream>
#include <string>
#include <list>
#include <vector>
using std::begin;
using std::cout;
using std::end;
using std::endl;
using std::list;
using std::pair;

// This is an implementation of a Separate-Chaining Hash Table. A Separate chaining hash table uses a separate data structure,
// Linked List, Tree, Array, etc., to help handle collisions during hash computing. This implementation will use a linked list
// as auxilarry helper.

template <typename K, typename V>
class HashTable
{
private:
    static const int buckets = 10;
    list<pair<K, V>> table[buckets];

public:
    // returns whether the table is empty or not.
    bool isEmpty() const;
    // generates hash with K input.
    int hashFunction(K key);
    // inserts element into the table.
    void insert(K key, V value);
    // inserts element into the table.
    void add(K key, V value);
    // removes an element from the table if it exsit.
    void remove(K key);
    // removes an element from the table if it exsit.
    void erase(K key);
    // Searches the table for the key and returns true available.
    bool containsKey(K key) const;
    // Return the value associated with the given key.
    V get(K key) const;
    // Returns an immutable array of keys.
    std::vector<K> getKeys() const;
    // Returns an immutable array of values.
    std::vector<V> getValues() const;

    // Output a string representation of the table.
    // This requires that the data type T supports stream output itself.
    // This is used by the operator<< overload defined in this file.
    std::ostream &print(std::ostream &os) const; // Outputs a string.
    // Creates a new table on the heap
    HashTable() {}
};

// ============================================================================================================================================================
// Implementation Section
// ============================================================================================================================================================

template <typename K, typename V>
bool HashTable<K, V>::isEmpty() const
{
    int sum{};
    // Iterates through the array and checks the size of the buckets
    for (int i{}; i < buckets; i++)
    {
        sum += table[i].size();
    }
    // Will trigger if the sum adds up to 0
    if (!sum)
    {
        return true;
    }

    return false;
}

template <typename K, typename V>
int HashTable<K, V>::hashFunction(K key)
{
    // If our type is a digit, we would rather go ahead and not cast it to a string
    // and return its modulo for the hash value. For all other instances, we will
    // stringify keys
    return key % buckets;
}

template <typename K, typename V>
void HashTable<K, V>::insert(K key, V value)
{

    int hashValue = hashFunction(key);
    cout << "Key: " << key << " Hash: " << hashValue << endl;
    auto &cell = table[hashValue];
    // Iter at the beginning of the list;
    auto Iter = begin(cell);
    // We will use induction here and assume that our generated key does not exist.
    bool keyExist = false;

    for (; Iter != end(cell); Iter++)
    {
        // Checks for duplication and updates the value if necessary.
        if (Iter->first == key)
        {
            keyExist = true;
            cout << "[WARNING] : Duplicate value has been found. Overriding current value." << endl;
            Iter->second = value;
            break;
        }
    }

    if (!keyExist)
    {
        cell.emplace_back(key, value);
    }

    return;
}
template <typename K, typename V>
void HashTable<K, V>::add(K key, V value)
{

    int hashValue = hashFunction(key);
    auto &cell = table[hashValue];
    // Iter at the beginning of the list;
    auto Iter = begin(cell);
    // We will use induction here and assume that our generated key does not exist.
    bool keyExist = false;

    for (; Iter != end(cell); Iter++)
    {
        // Checks for duplication and updates the value if necessary.
        if (Iter->first == key)
        {
            keyExist = true;
            cout << "[WARNING] : Duplicate value has been found. Overriding current value." << endl;
            Iter->second = value;
            break;
        }
    }

    if (!keyExist)
    {
        cell.emplace_back(key, value);
    }

    return;
}

template <typename K, typename V>
void HashTable<K, V>::remove(K key)
{
    int hashValue = hashFunction(key);

    // Cell for which the value is hashed to.
    auto &cell = table[hashValue];
    // Iter at the beginning of the list;
    auto Iter = begin(cell);
    // We will use induction here and assume that our generated key does not exist.
    bool keyExist = false;

    for (; Iter != end(cell); Iter++)
    {
        if (Iter->first == key)
        {
            keyExist = true;
            Iter = cell.erase(Iter);
            break;
        }
    }

    if (!keyExist)
    {
        cout << "[WARNING]: Attempting to remove a value that does not exist." << endl;
    }

    return;
}

template <typename K, typename V>
void HashTable<K, V>::erase(K key)
{
    int hashValue = hashFunction(key);

    // Cell for which the value is hashed to.
    auto &cell = table[hashValue];
    // Iter at the beginning of the list;
    auto Iter = begin(cell);
    // We will use induction here and assume that our generated key does not exist.
    bool keyExist = false;

    for (; Iter != end(cell); Iter++)
    {
        if (Iter->first == key)
        {
            keyExist = true;
            Iter = cell.erase(Iter);
            break;
        }
    }

    if (!keyExist)
    {
        cout << "[WARNING]: Attempting to remove a value that does not exist." << endl;
    }

    return;
}

template <typename K, typename V>
bool HashTable<K, V>::containsKey(K key) const
{
    int hashValue = hashFunction(key);

    // If the bucket position is null, we will return false;
    if (!table[hashValue])
    {
        return false;
    }

    // If a value exist we must walk the list to check for the
    // the exact key.

    auto &cell = table[hashValue];
    auto Iter = begin(cell);

    // Walks the list until we find a key that matches
    while (Iter != end(cell))
    {
        if (Iter->first == key)
        {
            return true;
        }
    }

    return false;
}

// Get method is not stable for implementation yet.

// template <typename K, typename V>
// V HashTable<K, V>::get(K key) const
// {
//     int hashValue = hashFunction(key);

//     auto &cell = table[hashValue];

//     auto Iter = begin(cell);

//     bool keyFound = false;

//     K value = nullptr;

//     while (Iter != end(cell))
//     {
//         if (Iter->first == key)
//         {
//             keyFound = true;
//             value = Iter->second;
//         }
//     }

//     if (!keyFound)
//     {
//         throw new std::runtime_error("[WARNING]: Trying to retrieve a value that does not exist.");
//     }

//     return value;
// }

template <typename K, typename V>
std::vector<K> HashTable<K, V>::getKeys() const
{
    // Iterate through the table and insert the keys into this vector.
    std::vector<K> keysVector;
    for (int i{}; i < buckets; i++)
    {
        if (table[i].size() == 0)
            continue;

        auto &cell = table[i];
        auto Iter = begin(cell);

        while (Iter != end(cell))
        {
            keysVector.push_back(Iter->first);
            Iter++;
        }
    }

    return keysVector;
}
template <typename K, typename V>
std::vector<V> HashTable<K, V>::getValues() const
{
    // Iterate through the table and insert the keys into this vector.
    std::vector<V> keysVector;
    for (int i{}; i < buckets; i++)
    {
        if (table[i].size() == 0)
            continue;

        auto &cell = table[i];
        auto Iter = begin(cell);

        while (Iter != end(cell))
        {
            keysVector.push_back(Iter->second);
            Iter++;
        }
    }

    return keysVector;
}

template <typename K, typename V>
std::ostream &HashTable<K, V>::print(std::ostream &os) const
{
    os << "[ \n";

    for (int i{}; i < buckets; i++)
    {
        if (table[i].size() == 0)
            continue;

        auto Iter = table[i].begin();
        // If the list has more than one memeber
        if (table[i].size() > 1)
        {
            while (Iter != table[i].end())
            {
                cout << "  Key: " << Iter->first << ", Value: " << Iter->second << "-->";
                Iter++;
            }
        }
        else
        {
            cout << "  Key: " << Iter->first << ", Value: " << Iter->second << endl;
        }
    }

    os << "]" << endl;

    return os;
}