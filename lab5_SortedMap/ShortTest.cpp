#include <assert.h>

#include "SortedMap.h"
#include "SMIterator.h"
#include "ShortTest.h"
#include <exception>
#include <iostream>

using namespace std;

bool relatie1(TKey cheie1, TKey cheie2) {
	if (cheie1 <= cheie2) {
		return true;
	}
	else {
		return false;
	}
}

void testAll(){
	SortedMap sm(relatie1);
	assert(sm.size() == 0);
	assert(sm.isEmpty());
    sm.add(1,2);
    assert(sm.size() == 1);
    assert(!sm.isEmpty());
    assert(sm.search(1)!=NULL_TVALUE);
    TValue v =sm.add(1,3);
    assert(v == 2);
    assert(sm.search(1) == 3);
    SMIterator it = sm.iterator();
    it.first();
    while (it.valid()){
    	TElem e = it.getCurrent();
    	assert(e.second != NULL_TVALUE);
    	it.next();
    }
    assert(sm.remove(1) == 3);
    assert(sm.isEmpty());
}

void testAddIfNotPresent()
{
    cout << "Test add if not present" << endl;

    SortedMap sm1([](TKey k1, TKey k2) {return  k1 < k2; });
    sm1.add(1, 1);
    sm1.add(2, 2);
    sm1.add(3, 3);

    SortedMap sm2([](TKey k1, TKey k2) {return k1 < k2;});
    sm2.add(4, 4);
    sm2.add(5, 5);

    int addedPairs = sm1.addIfNotPresent(sm2);
    assert(addedPairs == 2);
}
