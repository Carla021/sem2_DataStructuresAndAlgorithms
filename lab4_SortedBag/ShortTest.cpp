#include "ShortTest.h"
#include "SortedBag.h"
#include "SortedBagIterator.h"
#include <assert.h>
#include <iostream>

using namespace std;

bool relation1(TComp e1, TComp e2) {
	return e1 <= e2;
}

void testAll() {
	SortedBag sb(relation1);
	sb.add(5);
	sb.add(6);
	sb.add(0);
	sb.add(5);
	sb.add(10);
	sb.add(8);

	assert(sb.size() == 6);
	assert(sb.nrOccurrences(5) == 2);

	assert(sb.remove(5) == true);
	assert(sb.size() == 5);

	assert(sb.search(6) == true);
	assert(sb.isEmpty() == false);

	SortedBagIterator it = sb.iterator();
	assert(it.valid() == true);
	while (it.valid()) {
		it.getCurrent();
		it.next();
	}
	assert(it.valid() == false);
	it.first();
	assert(it.valid() == true);

}

bool is_prime(int num) {
    if(num < 2) {
        return false;
    }
    for(int i = 2; i * i <= num; i++) {
        if(num % i == 0)
            return false;
    }
    return true;
}

void test_filter()
{
    cout << "Test filter function" << endl;

    SortedBag bag(relation1);

    bag.add(1);
    bag.add(2);
    bag.add(3);
    bag.add(4);
    bag.add(5);

    bag.filter(is_prime);
    SortedBagIterator iterator = bag.iterator();
    assert(iterator.getCurrent() == 2);
    iterator.next();
    assert(iterator.getCurrent() == 3);
    iterator.next();
    assert(iterator.getCurrent() == 5);

}
