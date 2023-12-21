#pragma once
#include "SortedBag.h"

class SortedBag;

class SortedBagIterator
{
    friend class SortedBag;

private:
    const SortedBag& bag;
    explicit SortedBagIterator(const SortedBag& b);

    int current_position;
    int copy_current_position;
    struct Node* current_element;
    TComp* copy_elems;

public:
    TComp getCurrent();
    bool valid() const;
    void next();
    void first();
};