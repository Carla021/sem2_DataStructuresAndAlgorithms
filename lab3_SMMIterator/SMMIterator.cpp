#include "SMMIterator.h"
#include "SortedMultiMap.h"
#include <exception>

using namespace std;

// O(1) // the iterators constructor
SMMIterator::SMMIterator(const SortedMultiMap& d) : map(d){
	pos = map.elems.head;
}

// O(1)
void SMMIterator::first(){
	pos = map.elems.head;
}

// O(1)
void SMMIterator::next(){
	if(!this->valid())
        throw std::exception();
    pos = map.elems.list[pos].next;
}

// O(1)
bool SMMIterator::valid() const{
	return pos != -1;
}

// O(1)
TElem SMMIterator::getCurrent() const{
	if(!this->valid())
        throw std::exception();
    return map.elems.list[pos].info;
}

// O(k) k - the number of jumps to be executed backwards
void SMMIterator::jumpBackward(int k) {
    if(k <= 0) {
        // Invalid jump amount
        throw exception();
    }
    if(!valid()) {
        // Iterator is invalid
        throw exception();
    }
    int posBackup = pos;
    for(int i = 0; i < k; i++)
    {
        if(pos == map.elems.tail) {
            //Not enough elements left in the SortedMultiMap
            pos = -1;
            throw exception();
        }
        pos = map.elems.list[pos].next;
    }
    if(pos == -1) {
        pos = posBackup;
        // not enough elements left in the sortedMultiMap
        throw exception();
    }
}
