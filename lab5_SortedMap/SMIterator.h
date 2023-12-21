#pragma once
#include "SortedMap.h"
#include <stack>

using namespace std;

//DO NOT CHANGE THIS PART
class SMIterator{
	friend class SortedMap;
private:
	const SortedMap& map;
	SMIterator(const SortedMap& mapionar);

	//TODO - Representation
    int currentNode;  //represents the current node in the iterator
    stack<int> stack;  // stack used for traversing the tree

public:
	void first();
	void next();
	bool valid() const;
    TElem getCurrent() const;
};

