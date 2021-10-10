// Dave Benton
// CMSC 341 - Fall 2020 - Project 4
// hash.h

// Templated, hash table implementation.  Each buckets is a heap.  A
// bucket contains objects with the same key values stored as a
// max-heap based on priority.  Collisions are resolved by linear
// probing.

// To work with Heap and HashTable, the template class T must provide
// the following:
//  (a) Default constructor
//  (b) priority function (unsigned priority())
//  (c) key function (string key())
//  (d) overloaded insertion operator

#ifndef _HASH_H
#define _HASH_H

#include <string>
#include "heap.h"

using std::string;

// Hash function typedef.  The hash function must return an 'unsigned
// int' value.  Reduction mod N is the responsiblity of the caller,
// not the hash function.
typedef unsigned (*hash_fn)(string);

class Grader;

template <class T>
class HashTable {

    friend Grader;

public:
    // Constructor.  Requires table size and hash function.
    HashTable(unsigned size, hash_fn hash);

    // Destructor, copy, and assignment
    ~HashTable();
    HashTable(const HashTable<T>& ht);
    const HashTable<T>& operator=(const HashTable<T>& ht);

    // In-line functions

    // Table size; set by constructor
    unsigned tableSize() const { return _N; }
    // Number of entries in the table
    unsigned numEntries() const { return _n; }
    // Load factor
    float lambda() const { return ((float) _n) / _N; }

    // Main hash table functions

    // insert returns 'true' if successful; 'false' otherwise
    bool insert(const T& object);
    // getNext retrieves **and removes** the highest priority order of
    // type indicated by key.  It returns 'true' if successful; 'false'
    // otherwise.
    bool getNext(string key, T& obj);

    // Dump the entire hash table
    void dump() const;

private:
    unsigned _N;       // hash table size
    unsigned _n;       // current number of entries
    hash_fn _hash;     // hash function
    Heap<T> *_table;   // array of heaps

    // ***********************************************
    // Private helper function declarations go here! *
    // ***********************************************

};

// *****************************************
// Templated function definitions go here! *
// *****************************************

template <class T>
HashTable<T>::HashTable(unsigned int size, hash_fn hash) // Hash table constructor
{
    _N = size; // initializes hash table size
    _n = 0; // initializes the number of items in the hash table
    _hash = hash; // initializes priority function
    _table = new Heap<T>[size]; // initializes the hash table as an array
}


template <class T>
HashTable<T>::~HashTable() // Destructor
{
    _N = 0; // resets the size of the hash table to 0
    _n = 0; // resets number of entries to 0
    delete[] _table; // deletes the hash table
    _table = nullptr;
}


template <class T>
HashTable<T>::HashTable(const HashTable<T>& ht) // Copy constructor
{
    _table = new Heap<T>[ht.tableSize()]; // re creates the array with the correct size
    for (unsigned int i = 0; i < ht.tableSize(); i++) // loops through ht
    {
        _table[i] = ht._table[i]; // copies each index for ht into _table
    }
    _N = ht._N; // sets _table's size using ht's
    _n = ht._n; // sets _table's number of entries using ht's
    _hash = ht._hash; // sets _table's priority function using ht's
}


// overloaded assignment operator
template <class T>
const HashTable<T>& HashTable<T>::operator=(const HashTable<T>& ht)
{
    if (this != &ht) // gaurd against self assignment
    {
        delete[] _table; // empties the hash table
        _table = new Heap<T>[ht.tableSize()]; // builds the hash table with the correct size
        for (unsigned int i = 0; i < ht.tableSize(); i++) // loops through ht
        {
            _table[i] = ht._table[i]; // copies each value from ht into _table
        }
        _N = ht._N; // sets _table's size using ht's
        _n = ht._n; // sets _table's number of entries using ht's
        _hash = ht._hash; // sets _table's priority function using ht's
    }
    return *this; // returns the copied hash table
}


template <class T>
bool HashTable<T>::insert(const T& object) // inserts objects into the hash table buckets
{
    int index = _hash(object.key()) % _N; // get's hash value
    int ogHash = index;// saves first hash value to end while loop
    //while loop runs until we find the spot where the bucket has not yet been used or if the key matches
    while ((_table[index].used() == true && _table[index].readTop().key() != object.key())) {
        index = (index + 1) % _N; // linear probing
        if (index == ogHash) //prevents an infinite loop, if we make it back where we started, end
        {
            return false; // returns false if we could not insert object
        }
    }
    if (_table[index].used() == false ||
        _table[index].readTop().key() == object.key()) // increases _n if inserting a new object
    {
        _n++;
        //_table[index].insert(object); // inserts object using the heap class's insert
        //return true; // returns true to signify we could insert
    }
    //return false;
    _table[index].insert(object); // inserts object using the heap class's insert
    return true; // returns true to signify we could insert
}

//returns and removes the highest priority object with given key
template <class T>
bool HashTable<T>::getNext(string key, T& obj)
{
    int index = _hash(key) % _N; // gets the hash value
    //int ogHash = index;
    for (unsigned int i = 1; i <= _N; i++) // used to linear probe
    {
        //_table[index].dump();
        if (_table[index].empty() != true) // if the bucket is not empty
        {
            if (_table[index].readTop().key() == key) // if the key at the top of the bucket matches key
            {
                obj = _table[index].readTop(); // object gets set to the highest priority object in bucket
                _table[index].removeTop(); // removes that object
                if (_table[index].empty() == true) // if bucket becomes empty
                {
                    _n--; // decrements number of entries
                }
                return true; // returns true if could remove
            }
        }
        index = (index + 1) % _N; // linear probes
    }
    return false; // returns false if could not remove
}


template <class T>
void HashTable<T>::dump() const // dumps the hash table in index order
{
    for (unsigned int i = 0; i < _N; i++) // loops through the hash table
    {
        cout << "["<< i << "]:" << endl; // displays current hash table index
        _table[i].dump(); // dumps the bucket
    }
}

#endif