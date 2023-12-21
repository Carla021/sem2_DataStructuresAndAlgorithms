#include "SMMIterator.h"
#include "SortedMultiMap.h"
#include <iostream>
#include <utility>
#include <vector>
#include <exception>
#include <utility>
using namespace std;

// O(1)
SortedMultiMap::SortedMultiMap(Relation r) {
	this->comp = r;
    this->length = 0;
}

/*
 * best case: Theta(1) - when the element added has a true relation with the smallest or largest element already in the map
 * worst case: Theta(n) where n is the array length - when the relations above mentioned are false
 * total complexity: O(n), n - array length
 */
void SortedMultiMap::add(TKey c, TValue v) {
	DLLA& elements = this->elems;

    // resize if the list is full and there is no empty space
    if(elements.size == elements.capacity && elements.first_empty == -1)
    {
        elements.capacity *= 2;
        auto* aux = new DLLANode[elements.capacity];
        for(int index = 0; index < elements.size; ++index)
            aux[index] = elements[index];
        delete[] elements.list;
        elements.list = aux;
    }

    // if the list is full and there is empty space
    if(elements.first_empty == -1)
    {
        elements.first_empty = elements.size;
        elements[elements.first_empty] = DLLANode(NULL_TELEM, -1, -1);
        ++elements.size;
    }

    // if the list is empty
    if(this->length == 0)
    {
        int copy_empty = elements.first_empty;
        elements.first_empty = elements[elements.first_empty].next;
        elements[copy_empty] = DLLANode(make_pair(c, v), -1, -1);
        elements.tail = elements.head = copy_empty;
        ++this->length;
        return;
    }

    int copy_empty = elements.first_empty;
    elements.first_empty = elements[elements.first_empty].next;

    // if the new node is the new head of the list
    if(comp(c, elements[elements.head].info.first))
    {
        elements[copy_empty] = DLLANode(make_pair(c, v), -1, elements.head);
        elements[elements.head].prev = copy_empty;
        elements.head = copy_empty;
        ++this->length;
        return;
    }

    // if the new node is the new tail of the list
    if(comp(elements[elements.tail].info.first, c))
    {
        elements[copy_empty] = DLLANode(make_pair(c, v), elements.tail, -1);
        elements[elements.tail].next = copy_empty;
        elements.tail = copy_empty;
        ++this->length;
        return;
    }

    int after = elements.head;

    // find the node that will be before the new added node
    for(int node = elements.head; node != -1; node = elements[node].next)
    {
        if(comp(elements[node].info.first, c) && elements[node].info.first != c)
            after = node;
        else
            break;
    }
    elements[copy_empty] = DLLANode(make_pair(c, v), after, elements[after].next);
    elements[elements[after].next].prev = copy_empty;
    elements[after].next = copy_empty;
    ++this->length;
}

/*
 * best case: O(1): when the element searched is the first element in the list
 * worst case: O(n) n - the length of the array: when all elements of the array must be parsed to find the element
 * total: O(n): the entire array is parsed linearly
 */
vector<TValue> SortedMultiMap::search(TKey c) const {
	vector<TValue> values;

    // we try to find a node with a key that has a False relation with c
    for(int node = this->elems.head; node != -1; node = this->elems.list[node].next)
        if(this->elems.list[node].info.first == c)
            values.push_back(this->elems.list[node].info.second);
        else
            if(!comp(this->elems.list[node].info.first, c))
                break;

    return values;
}

/*
 * best case: Theta(1) - when the searched element for removing is the head or the tail of the list
 * worst case: Theta(n), n - array length: when the element is not in the list
 * total: O(n): because it goes through the entire array
 */
bool SortedMultiMap::remove(TKey c, TValue v) {
	DLLA& elements = this->elems;
    if(this->length == 0)
        return false;

    if(this->length == 1)
    {
        if(elements[elements.head].info != make_pair(c, v))
            return false;
        elements[elements.head].next = elements.first_empty;
        elements.first_empty = elements.head;
        elements.head = elements.tail = -1;
        --this->length;
        return true;
    }

    // if the pair is the head of the list
    if(elements[elements.head].info == make_pair(c, v))
    {
        int copy_new_head = elements[elements.head].next;
        elements[elements.head].next = elements.first_empty;
        elements.first_empty = elements.head;
        elements.head = copy_new_head;
        elements[elements.head].prev = -1;
        --this->length;
        return true;
    }

    // if the pair is the tail of the list
    if(elements[elements.tail].info == make_pair(c, v))
    {
        int copy_new_tail = elements[elements.tail].prev;
        elements[elements.tail].next = elements.first_empty;
        elements.first_empty = elements.tail;
        elements.tail = copy_new_tail;
        elements[elements.tail].next = -1;
        --this->length;
        return true;
    }

    int pos = -1;
    // iterate through the list until we find the node with the pair and the relation between the key of the node and the key of the pair is True
    for(int node = elements.head; node != -1; node = elements[node].next)
    {
        if(comp(c, elements[node].info.first) && c != elements[node].info.first)
            break;
        if(make_pair(c, v) == elements[node].info)
        {
            pos = node;
            break;
        }
    }
    if(pos == -1)
        return false;
    elements[elements[pos].prev].next = elements[pos].next;
    elements[elements[pos].next].prev = elements[pos].prev;
    elements[pos].next = elements.first_empty;
    elements.first_empty = pos;
    --this->length;
    return true;

}

// O(1)
int SortedMultiMap::size() const {
	return this->length;
}

// O(1)
bool SortedMultiMap::isEmpty() const {
    return  this->length == 0;
}

// O(1)
SMMIterator SortedMultiMap::iterator() const {
	return SMMIterator{*this};
}

// O(1)
SortedMultiMap::~SortedMultiMap() {
	delete[] elems.list;
}

// O(1)
SortedMultiMap::DLLANode &SortedMultiMap::DLLA::operator[](int pos) {
    return this->list[pos];
}

//O(1)
SortedMultiMap::DLLANode::DLLANode(TElem info, int prev, int next) : info(std::move(info)), prev{prev}, next{next} {};

//O(1)
SortedMultiMap::DLLANode::DLLANode() : info{ NULL_TELEM }, prev{ 0 }, next{ 0 } {}



