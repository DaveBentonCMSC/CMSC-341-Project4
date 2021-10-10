// Dave Benton
// CMSC 341 - Fall 2020 - Project 4
// heap.h
// Templated, vector-based heap implementation

// To work with Heap, the template class T must provide the following:
//  (a) Default constructor
//  (b) priority function (unsigned priority())
//  (c) overloaded insertion operator
// To work with Heap *and* HashTable, it also needs:
//  (d) key function (string key())

#ifndef _HEAP_H
#define _HEAP_H

#include <iostream>
#include <vector>
#include <exception>
#include <utility>   // swap

using std::vector;
using std::endl;
using std::cout;
using std::swap;
using std::range_error;

// To work with Heap and HashTable, class T must provide the
// following:
//  (a) Default constructor
//  (b) priority function (unsigned priority())
//  (c) key function (string key())
//  (d) overloaded insertion operator

class Grader;

template <class T>
class Heap {

    friend Grader;

public:
    // Constructor
    Heap();

    // Inline functions: size, empty, used
    unsigned size() const { return _heap.size() - 1 ; }
    bool empty() const { return _heap.size() <= 1 ; }
    bool used() const { return _used ; }

    // Main heap operations: insert, read, remove
    void insert(const T& object);
    T readTop() const;
    void removeTop();

    // Dump the entire heap
    void dump() const;

    // Root node always has index 1
    static const unsigned ROOT = 1;

private:
    vector<T> _heap;   // Vector-based heap
    bool _used;        // Has the heap ever been used?  Needed for
    // linear probing.

    // *********************************************
    // Private helper function declarations go here!
    // *********************************************
    void heaper(int top, int bottom);
    void heaperDown(int top);

};

// ***************************************
// Templated function definitions go here!
// ***************************************


template <class T>
Heap<T>::Heap() // default constructor
{
    _used = false; // sets the heap to "unused"
    T object; // empty object to go at index 0
    _heap.push_back(object); // inserts empty object into vector (heap)
}


template <class T>
void Heap<T>::insert(const T& object)
{
    _used = true; // changes heap to used when inserting an object
    _heap.push_back(object); // inserts object
    int index = size(); // used for the insert helper
    heaper(1, index); // calls helper to maintain
}


template <class T>
T Heap<T>::readTop() const // returns the highest priority object
{
    if (empty()) // if the heap is empty
    {
        throw range_error("Heap is empty, out of range"); // throws a range error
    }
    else // if not empty
    {
        return _heap[1]; // returns highest priority object
    }
}


template <class T>
void Heap<T>::removeTop() // removes the highest priority object
{
    if (empty()) // if heap is empty
    {
        throw range_error("Heap is empty, out of range"); // throws a range error
    }
    else
    {
        int index = size(); // used for the helper to maintain the heap
        //swap(_heap[1], _heap[index]);
        _heap[1] = _heap[index];
        _heap.pop_back();
        heaperDown(1); // calls the helper to maintain the heap
    }
}


template <class T>
void Heap<T>::dump() const // prints out the heap in index order
{
    for (unsigned int i = 1; i <= size(); i++) // traverses the heap
    {
        cout << _heap[i] << endl; // calls car overloaded << operator to print each car
    }
}

template <class T>
void Heap<T>::heaper(int top, int bottom) // helper to maintain heap when inserting
{
    if (_heap[bottom].priority() > _heap[top].priority()) // if the newly insert object
                                                          // has a higher priority than the "root"
    {
        int parent = (bottom) / 2; // gets index of newly inserted object's parent
        if (_heap[parent].priority() < _heap[bottom].priority()) // if the parent has a lower priority
        {
            swap(_heap[bottom], _heap[parent]); // swaps the objects
            heaper(top, parent); //recursive call to repeat until correct
        }
    }
}


template <class T>
void Heap<T>::heaperDown(int top) // helper to maintain heap when removing
{

    int left = top * 2 + 1;
    int right = top * 2 + 2;
    int max = top;

    if (left < size() && _heap[left].priority() > _heap[top].priority())
    {
        max = left;
    }

    if (right < size() && _heap[right].priority() > _heap[max].priority())
    {
        max = right;
    }

    if (max != top)
    {
        swap(_heap[top], _heap[max]);
        heaperDown(max);
    }
}

#endif