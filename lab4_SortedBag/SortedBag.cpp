#include <cstddef>
#include "SortedBag.h"
#include "SortedBagIterator.h"


//BC: Theta(C)
//WC: Theta(C), where C is the capacity of the hashtable
//Total: Theta(C)
SortedBag::SortedBag(Relation r) {
    this->r = r;
    this->m = 127;
    this->nr_elements = 0;

    // create a hashtable of size m and initialize each entry to nullptr
    this->hashtable = new struct Node*[this->m];
    for (int i = 0; i < this->m; i++) {
        this->hashtable[i] = nullptr;
    }
}


//BC: Theta(1)
//WC: Theta(1)
//Total: Theta(1)

// hash function
TComp SortedBag::h(TComp k) const {
    // hash function used to determine the position of an element in the hashtable.
    // it calculates the modulo of the key k with the hashtable size m, and ensures the result is non-negative

    int modulo = k % this->m;
    return (modulo < 0) ? (modulo + this->m) : modulo;
}


//BC: Theta(M)
//WC: Theta(M), where M is the maximum number of distinct elements associated to a key
//Total: Theta(M)
void SortedBag::resize() {
    // function to resize the hashtable when the load factor exceeds 1
    // it doubles the capacity m and creates a new hashtable
    // then, it copies the elements and frequencies from the old hashtable to the new one, rehashing them to new positions

    this->m *= 2;

    // copy elements and frequencies in temporary arrays

    auto* copy_elems = new TComp[this->nr_elements];
    int* copy_freq = new int[this->nr_elements];
    int k = 0;

    // copy elements and frequencies from the old hashtable to the temporary arrays
    for (int i = 0; i < this->m / 2; i++) {
        struct Node *current_node = this->hashtable[i];
        while (current_node != nullptr) {
            copy_elems[k] = current_node->elem;
            copy_freq[k++] = current_node->freq;
            current_node = current_node->next;
        }
    }

    // delete old hashtable and reset the number of elements
    for (int i = 0; i < this->m / 2; i++) {
        struct Node* current_node = this->hashtable[i];
        while (current_node != nullptr) {
            struct Node* next_node = current_node->next;
            delete current_node;
            current_node = next_node;
        }
    }
    delete[] this->hashtable;

    // reset hashtable and add elements from the copied array
    this->nr_elements = 0;
    this->hashtable = new struct Node* [this->m];
    for (int i = 0; i < this->m; i++) {
        this->hashtable[i] = nullptr;
    }

    for (int i = 0; i < k; i++) {
        for(int j = 0; j < copy_freq[i]; j++) {
            this->add(copy_elems[i]);
        }
    }

    // delete the temporary arrays
    delete[] copy_elems;
}


//BC: O(M),
//WC: O(M), where M is the maximum number of distinct elements associated to a key
//Total: O(M)

// add an element to the sorted bag
void SortedBag::add(TComp e) {

    // it first checks the load factor and resize the hashtable if necessary
    auto load_factor = this->nr_elements / this->m;
    if (load_factor >= 1) {
        this->resize();
    }

    // it calculates the position of the element in the hashtable using the hash function
    TComp position = this->h(e);

    struct Node* current_node = this->hashtable[position];

    // it searches for the element in the corresponding chain and increments its frequency if found.
    while (current_node != nullptr && e != current_node->elem) {
        current_node = current_node->next;
    }

    if(current_node != nullptr) {
        current_node->freq++;
    } else {
        // if the element is not found, it creates a new node and inserts it in the correct position in the chain,
        //maintaining the sorted order based on the given relation.

        current_node = this->hashtable[position];
        struct Node* prev_node = nullptr;

        auto* new_node = new struct Node;
        new_node->elem = e;
        new_node->freq = 1;

        // find the correct position to insert the new node while maintaining the sorted order.
        while (current_node != nullptr && !r(e, current_node->elem)) {
            prev_node = current_node;
            current_node = current_node->next;
        }

        if (prev_node == nullptr) {
            // inserting at the beginning of the chain
            new_node->next = this->hashtable[position];
            this->hashtable[position] = new_node;
        } else {
            // inserting in the middle or at the end of the chain
            new_node->next = current_node;
            prev_node->next = new_node;
        }
    }

    this->nr_elements++;
}


//BC: Theta(1)
//WC: O(M), where M is the maximum number of distinct elements associated to a key
//Total: O(M)

// remove an element from the sorted bag
bool SortedBag::remove(TComp e) {

    // it calculates the position of an element in the hashtable using the hash function

    TComp position = this->h(e);
    struct Node* current_node = this->hashtable[position];
    struct Node* prev_node = nullptr;

    // it searches for the element in the corresponding chain and removes it if found
    // if the frequency of the element is greater than 1, it decrements the frequency instead of removing the node completely
    while (current_node != nullptr && e != current_node->elem) {
        prev_node = current_node;
        current_node = current_node->next;
    }

    if (current_node != nullptr) {
        if(current_node->freq == 1) {
            // element frequency is 1, remove the node from the chain
            if (prev_node == nullptr) {
                this->hashtable[position] = this->hashtable[position]->next;
            } else {
                // removing a node from the middle or end of the chain
                prev_node->next = current_node->next;
            }
        } else {
            // element frequency is greater than 1, decrement the frequency
            current_node->freq--;
        }
        this->nr_elements--;
        return true;
    }
    else {
        return false;
    }
}


//BC: Theta(1)
//WC: O(M), where M is the maximum number of distinct elements associated to a key
//Total: O(M)

// search for an element in the sorted bag
bool SortedBag::search(TComp elem) const {

    // the function calculates the position of the element in the hashtable using the hash function

    TComp position = this->h(elem);
    struct Node* current_node = this->hashtable[position];

    while (current_node != nullptr && elem != current_node->elem) {
        current_node = current_node->next;
    }

    return (current_node != nullptr);
}


//BC: Theta(1)
//WC: O(M), where M is the maximum number of distinct elements associated to a key
//Total: O(M)

// return the number of occurences of an element in the sorted bag
int SortedBag::nrOccurrences(TComp elem) const {
    TComp position = this->h(elem);
    struct Node* current_node = this->hashtable[position];

    while (current_node != nullptr && elem != current_node->elem) {
        current_node = current_node->next;
    }

    // returns the element's frequency if found, and 0 otherwise
    return (current_node != nullptr) ? current_node->freq : 0;
}


//BC: Theta(1)
//WC: Theta(1)
//Total: Theta(1)

// return the size (number of elements) of the sorted bag
int SortedBag::size() const {
    return this->nr_elements;
}


//BC: Theta(1)
//WC: Theta(1)
//Total: Theta(1)

// check if the sorted bag is empty
bool SortedBag::isEmpty() const {
    return (this->nr_elements == 0);
}

// return an iterator for the sorted bag
SortedBagIterator SortedBag::iterator() const {
    return SortedBagIterator(*this);
}


//BC: Theta(N), where N is the number of elements
//WC: Theta(N)
//Total: Theta(N)
SortedBag::~SortedBag() {

    // destructor that frees the memory allocated for the sorted bag
    //it iterates through each chain in the hashtable and deletes all the nodes

    for (int i = 0; i < this->m / 2; i++) {
        struct Node* current_node = this->hashtable[i];
        while (current_node != nullptr) {
            struct Node* next_node = current_node->next;
            delete current_node;
            current_node = next_node;
        }
    }
    delete[] this->hashtable;
}

/*
void SortedBag::filter(Condition cond) {

    SortedBagIterator iterator = this->iterator();
    for(int i = 0; i < nr_elements; i++) {
        TComp current = iterator.getCurrent();
        if(!cond(current)) {
            iterator.next();
            this->remove(current);
        }
        else {
            iterator.next();
        }
    }

}*/

// O(m), m - size of the hashtable
void SortedBag::filter(Condition cond) {
   // Traverse the hashtable and remove elements that don't satisfy the condition
    for (int i = 0; i < m; i++) {
        Node* current = hashtable[i];
        Node* prev = nullptr;

        while (current != nullptr) {
            if (!cond(current->elem)) {
                // Element does not satisfy the condition, remove it
                Node* toRemove = current;

                if (prev == nullptr) {
                    // Update the head of the linked list
                    hashtable[i] = current->next;
                    current = hashtable[i];
                } else {
                    // Update the previous node's next pointer
                    prev->next = current->next;
                    current = prev->next;
                }

                delete toRemove;
                nr_elements--;
            } else {
                // Element satisfies the condition, move to the next node
                prev = current;
                current = current->next;
            }
        }
    }
}

