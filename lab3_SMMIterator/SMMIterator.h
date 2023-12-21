#pragma once

#include "SortedMultiMap.h"


class SMMIterator{
	friend class SortedMultiMap;
private:
	//DO NOT CHANGE THIS PART
	const SortedMultiMap& map;
	explicit SMMIterator(const SortedMultiMap& map);

    // the current position of the list of nodes
	int pos;

public:
	void first();
	void next();
	bool valid() const;
   	TElem getCurrent() const;

    void jumpBackward(int k);
};

