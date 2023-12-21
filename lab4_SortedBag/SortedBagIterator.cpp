#include "SortedBagIterator.h"
#include "SortedBag.h"
#include <exception>

using namespace std;


//BC: Theta(N^2), where N is the number of elements
//WC: Theta(N^2)
//Total: Theta(N^2)
SortedBagIterator::SortedBagIterator(const SortedBag& b) : bag(b) {

    this->copy_elems = new TComp[b.nr_elements];
    int k = 0;

    // copy elements from the SortedBag to the iterator's copy_elems array
    for (int i = 0; i < this->bag.m; i++) {
        struct Node* current_node = this->bag.hashtable[i];
        while (current_node != nullptr) {
            for(int j=0; j<current_node->freq; j++) {
                this->copy_elems[k++] = current_node->elem;
            }
            current_node = current_node->next;
        }
    }


    // sort the copied elements in ascending order using the SortedBag's relation
    for(int i=0; i< this->bag.nr_elements-1; i++) {
        for (int j = i + 1; j < this->bag.nr_elements; j++) {
            if (!this->bag.r(this->copy_elems[i], this->copy_elems[j])) {
                int aux = this->copy_elems[i];
                this->copy_elems[i] = this->copy_elems[j];
                this->copy_elems[j] = aux;
            }
        }
    }

    // initialize the current_position, current_element, and copy_current_position
    this->copy_current_position = 0;
    int first_elem = this->copy_elems[0];
    this->current_position = this->bag.h(first_elem);
    this->current_element = this->bag.hashtable[this->current_position];
}


//BC: Theta(1)
//WC: O(M), where M is the maximum number of distinct elements associated to a key
//Total: O(N)
TComp SortedBagIterator::getCurrent() {
    if (!this->valid()) {
        throw std::exception();
    }

    // update the current_position and current_element based on the copy_current_position
    this->current_position = this->bag.h(copy_elems[copy_current_position]);
    struct Node* current_node = this->bag.hashtable[this->current_position];

    while (current_node != nullptr && copy_elems[copy_current_position] != current_node->elem) {
        current_node = current_node->next;
    }
    this->current_element = current_node;

    return this->current_element->elem;
}


//BC: Theta(1)
//WC: Theta(1)
//Total: Theta(1)
bool SortedBagIterator::valid() const {
    return (this->copy_current_position != this->bag.nr_elements);
}


//BC: Theta(1)
//WC: O(M), where M is the maximum number of distinct elements associated to a key
//Total: Theta(1)
void SortedBagIterator::next() {

    // moves the iterator to the next element in the iteration

    if (!this->valid()) {
        throw std::exception();
    }

    this->copy_current_position++;

    // update the current_position and current_element if the next element is different
    if (this->copy_elems[this->copy_current_position] != copy_elems[this->copy_current_position-1]) {

        this->current_position = this->bag.h(copy_elems[copy_current_position]);
        struct Node* current_node = this->bag.hashtable[this->current_position];

        while (current_node != nullptr && copy_elems[copy_current_position] != current_node->elem) {
            current_node = current_node->next;
        }
        this->current_element = current_node;
    }
}


//BC: Theta(1)
//WC: Theta(1)
//Total: Theta(1)
void SortedBagIterator::first() {

    // moves the iterator to the first element in the iteration

    this->copy_current_position = 0;
    int first_elem = this->copy_elems[0];
    this->current_position = this->bag.h(first_elem);
    this->current_element = this->bag.hashtable[this->current_position];
}