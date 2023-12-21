#pragma once
//DO NOT INCLUDE SORTEDBAGITERATOR

//DO NOT CHANGE THIS PART
typedef int TComp;
typedef TComp TElem;
typedef bool(*Relation)(TComp, TComp);
#define NULL_TCOMP -11111;

typedef TComp (*TFunction)(TComp);
typedef bool(*Condition)(TComp);

// Define the structure for a node in the hashtable
struct Node {
    TElem elem;  // the element stored in the node
    int freq;  // the frequency of the element
    struct Node* next;  // pointer to the next node in the chain
};

// Forward declaration of SortedBagIterator
class SortedBagIterator;

// Define the SortedBag class
class SortedBag {
    friend class SortedBagIterator;

private:
    struct Node** hashtable;  // pointer to the hashtable
    int m;  // size of the hashtable
    TComp h (TComp) const;  // hash function
    int nr_elements;  // number of elements in the sorted bag
    Relation r;  // relation used for element comparison

public:
    //constructor
    explicit SortedBag(Relation r);

    //adds an element to the sorted bag
    void add(TComp e);

    //removes one occurence of an element from a sorted bag
    //returns true if an eleent was removed, false otherwise (if e was not part of the sorted bag)
    bool remove(TComp e);

    //checks if an element appearch is the sorted bag
    bool search(TComp e) const;

    //returns the number of occurrences for an element in the sorted bag
    int nrOccurrences(TComp e) const;

    //returns the number of elements from the sorted bag
    int size() const;

    //returns an iterator for this sorted bag
    SortedBagIterator iterator() const;

    //checks if the sorted bag is empty
    bool isEmpty() const;

    void resize();

    void filter(Condition cond);

    //destructor
    ~SortedBag();
};