#include "Set.h"
#include "SetITerator.h"


//O(1)
Set::Set() {
	this->head = nullptr;
    this->length = 0;
}

//O(n) in worst case
bool Set::add(TElem elem) {
	if (this->search(elem))
        return false;
    Node* new_node = new Node{ elem, nullptr };
    if (this->head == nullptr) {
        this->head = new_node;
    }
    else {
        Node* current = this->head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = new_node;
    }

    length++;
    return true;
}

//O(n)
bool Set::remove(TElem elem) {
	Node* current = head;
    Node* prev = nullptr;

    while(current != nullptr && current->info != elem)
    {
        prev = current;
        current = current->next;
    }

    if(current == nullptr)
        return false;

    if(prev == nullptr)
        head = current->next;
    else
        prev->next = current->next;

    delete current;
    length--;

    return true;
}


//O(n) for worst case
bool Set::search(TElem elem) const {
	Node* current = head;

    while(current != nullptr)
    {
        if (current->info == elem)
            return true;
        current = current->next;
    }

    return false;
}

//O(1)
int Set::size() const {
	return length;
}

//O(1)
bool Set::isEmpty() const {
	return length == 0;
}

//O(n) for worst case
Set::~Set() {
	Node* current = head;

    while (current != nullptr)
    {
        Node* next = current->next;
        delete current;
        current = next;
    }

    head = nullptr;
    length = 0;
}

//O(1)
SetIterator Set::iterator() const {
	return SetIterator(*this);
}

//O(n)
void Set::intersection(const Set &s) {
    Node* current = head;

    while(current != nullptr)
    {
        if(!s.search(current->info))
        {
            Node* to_remove = current;
            current = current->next;
            remove(to_remove->info);
        } else {
            current = current->next;
        }
    }
}


