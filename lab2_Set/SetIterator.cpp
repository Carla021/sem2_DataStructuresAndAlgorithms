#include "SetIterator.h"
#include "Set.h"
#include <exception>

using namespace std;


SetIterator::SetIterator(const Set& m) : set(m), current(m.head)
{

}

//O(1)
void SetIterator::first() {
    current = set.head;
}

//O(1)
void SetIterator::next() {
    if(!valid())
        throw exception();
    else
        current = current->next;
}

//O(1)
TElem SetIterator::getCurrent()
{
    if(!valid())
        throw exception();
    else
	    return current->info;
}

//O(1)
bool SetIterator::valid() const {
	return current != nullptr;
}



