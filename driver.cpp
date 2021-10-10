#include <iostream>
#include "hash.h"
#include "heap.h"
#include "car.h"
#include <ctime>

// Prototype for 16 bit cyclic hash function; implementation is after
// main().
unsigned cyclic_hash16(string key);
int randPriority();

int main() {

    // Create a vector of cars to insert in the hash table
    vector<Car> cars;
    cars.push_back(Car("x101", 5, "super car"));
    cars.push_back(Car("x101", 12, "mega car"));
    cars.push_back(Car("x101", 4, "shack of cars"));
    cars.push_back(Car("challenger", 10, "mega car"));
    cars.push_back(Car("challenger", 5, "car world"));
    cars.push_back(Car("stratos", 7, "car world"));
    cars.push_back(Car("stratos", 15, "super car"));
    cars.push_back(Car("stratos", 8, "shack of cars"));
    cars.push_back(Car("challenger", 3, "car joint"));
    cars.push_back(Car("gt500", 3, "shack of cars"));
    cars.push_back(Car("miura", 28, "mega car"));
    cars.push_back(Car("gt340", 11, "super car"));

    // Create the hash table of length 10 using cyclic_hash function
    HashTable<Car> ht(10, cyclic_hash16);

    vector<Car>::iterator itCar;
    for (itCar=cars.begin();itCar!=cars.end();itCar++)
        ht.insert(*itCar);

    cout << "\nDump of ht:\n";
    ht.dump();

    // Extract some orders

    Car d;
    vector<string> orders;

    orders.push_back("challenger");
    orders.push_back("miura");
    orders.push_back("gt500");
    orders.push_back("gt340");
    orders.push_back("gt500");

    vector<string>::iterator itString;

    for ( itString=orders.begin();itString!=orders.end();itString++ ) {
        cout << "\nGet next '" << *itString << "' order...\n";
        if ( ht.getNext(*itString, d) ) {
            cout << "  " << d << endl;
        } else {
            cout << "  No " << *itString << " orders!\n";
        }
    }

    cout << endl;

    cout << "\nDump of ht:\n";
    ht.dump();


    // Simple test of copy constructor; also need to test assignment
    // operator!
    cout << "\nCreate copy of hash table, remove two 'stratos' orders, dump both tables...\n";

    HashTable<Car> ht2(10, cyclic_hash16);
    ht2 = ht;

    HashTable<Car> ht3(ht2);

    ht2.getNext("stratos", d);
    ht2.getNext("stratos", d);

    cout << "\nDump of ht:\n";
    ht.dump();

    cout << "\nDump of ht2:\n";
    ht2.dump();

    cout << "\nDump of ht3 (copy of ht2 before removal, should match ht1):\n";
    ht3.dump();

    HashTable<Car> ht4(0, cyclic_hash16);
    cout << "\nCopying an empty Hash table using copy and assignment...\n";

    HashTable<Car> ht5(ht4);
    HashTable<Car> ht6(0, cyclic_hash16);

    cout << "Dumping ht5, a copy of empty ht4 using copy constructor:\n";
    ht5.dump();

    cout <<"Dumping ht6, a copy of empty ht4 using assignment operator:\n";
    ht6.dump();

    Heap<Car> _testHeap;
    cout << "\nTesting remove/read top on an empty heap (exception testing):\n";
    try
    {
        cout << "Testing readTop(): ";
        _testHeap.readTop();
    }
    catch(range_error &e)
    {
        cout << e.what() << endl;
    }

    try
    {
        cout << "Testing removeTop(): ";
        _testHeap.removeTop();
    }
    catch(range_error &e)
    {
        cout << e.what() << endl;
    }

    cout << "\nTesting runtimes:\n";

    Heap<Car> carsTest;
    clock_t time_req;
    time_req = clock();
    for(int i = 0; i < 50000; i++)
    {
        carsTest.insert(Car("challenger", randPriority(), "car joint"));
    }
    time_req = clock() - time_req;
    cout << "Inserting 50000 cars took: " << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;

    time_req = clock();
    for(int i = 0; i < 50000; i++)
    {
        carsTest.removeTop();
    }
    time_req = clock() - time_req;
    cout << "Removing 50000 cars took: " << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;


    time_req = clock();
    for(int i = 0; i < 100000; i++)
    {
        carsTest.insert(Car("challenger", randPriority(), "car joint"));
    }
    time_req = clock() - time_req;
    cout << "\nInserting 100000 cars took: " << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;

    time_req = clock();
    for(int i = 0; i < 100000; i++)
    {
        carsTest.removeTop();
    }
    time_req = clock() - time_req;
    cout << "Removing 100000 cars took: " << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;


    time_req = clock();
    for(int i = 0; i < 200000; i++)
    {
        carsTest.insert(Car("challenger", randPriority(), "car joint"));
    }
    time_req = clock() - time_req;
    cout << "\nInserting 200000 cars took: " << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;

    time_req = clock();
    for(int i = 0; i < 200000; i++)
    {
        carsTest.removeTop();
    }
    time_req = clock() - time_req;
    cout << "Removing 200000 cars took: " << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;

    return 0;

}

unsigned cyclic_hash16(string key) {
    unsigned usize = 16;
    unsigned s = 5; // shift by 5
    unsigned h = 0;
    for (unsigned int i = 0; i < key.length(); i++) {
        h = ((h << s) | (h >> (usize - s)));
        h += key[i];
        h = h & 0xffff;
    }
    return h;
}

int randPriority()
{
    srand (time(NULL));
    int pri = rand() % 100;
    return pri;
}
