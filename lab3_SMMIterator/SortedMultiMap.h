#pragma once
//DO NOT INCLUDE SMMITERATOR

//DO NOT CHANGE THIS PART
#include <vector>
#include <utility>
typedef int TKey;
typedef int TValue;
typedef std::pair<TKey, TValue> TElem;
#define NULL_TVALUE -111111
#define NULL_TELEM pair<TKey, TValue>(-111111, -111111)
using namespace std;
class SMMIterator;
typedef bool(*Relation)(TKey, TKey);


class SortedMultiMap {
	friend class SMMIterator; // gives access to SMMIterator class to the private and protected members of SortedMultiMap
    private:
		struct DLLANode
        {
            TElem info; // pair of key and value
            int prev; // index of the previous node
            int next; // index of the next node

            DLLANode();
            DLLANode(TElem info, int prev, int next);
        };

        struct DLLA // dynamic array of nodes of type DLLANode
        {
            DLLANode* list; // pointer to the first node of the list
            int head, tail; // index of the head and tail of the list
            int capacity, size; // capacity and size of the list
            int first_empty; // index of the first empty node

            DLLA()
            {
                this->capacity = 1;
                this->size = 1;
                this->list = new DLLANode[this->capacity];
                this->list[0] = DLLANode(NULL_TELEM, -1, -1); // add at index 0 a new empty node
                this->first_empty = 0; // initialize the first empty node with the index of the first node
                this->head = -1;
                this->tail = -1;
            }

            DLLANode& operator[](int pos);
        };

public:
    DLLA elems;
    Relation  comp;
    int length;

public:

    // constructor
    explicit SortedMultiMap(Relation r);

	//adds a new key value pair to the sorted multi map
    void add(TKey c, TValue v);

	//returns the values belonging to a given key
    vector<TValue> search(TKey c) const;

	//removes a key value pair from the sorted multimap
	//returns true if the pair was removed (it was part of the multimap), false if nothing is removed
    bool remove(TKey c, TValue v);

    //returns the number of key-value pairs from the sorted multimap
    int size() const;

    //verifies if the sorted multi map is empty
    bool isEmpty() const;

    // returns an iterator for the sorted multimap. The iterator will returns the pairs as required by the relation (given to the constructor)	
    SMMIterator iterator() const;

    // destructor
    ~SortedMultiMap();
};
