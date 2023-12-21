#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>

using namespace std;

//O(log n)
SMIterator::SMIterator(const SortedMap& m) : map(m){
    // initializes the iterator with the given SortedMap instance
    // Initializes the stack with the leftmost path of the tree

    int node = map.bst.root;
    while(node != NULL_TVALUE) {
        this->stack.push(node);  // push the current node onto the stack
        node = map.bst.nodes[node].left;  // move to the left child of the current node
    }
    if(!this->stack.empty()) {
        this->currentNode = stack.top();  // sets the currentNode to the top of the stack (leftmost node)
    }
    else {
        this->currentNode = NULL_TVALUE;  // if the stack is empty, set currently to NULL_TVALUE
    }
}

//O(log n)
void SMIterator::first(){
    // resets the iterator by clearing the stack
    // initializes the stack with the leftmost path of the tree
    // sets the currentNode to the top of the stack (leftmost node)

    while(!this->stack.empty())
        this->stack.pop();  // clear the stack
    int node = map.bst.root;
    while(node != NULL_TVALUE)
    {
        this->stack.push(node);  // push the current node onto the stack
        node = map.bst.nodes[node].left;  // move to the left child of the current node
    }
    if(!this->stack.empty()) {
        this->currentNode = this->stack.top();  // set the currentNode to the top of the stack (leftmost node)
    }
    else {
        this->currentNode = NULL_TVALUE;  // if the stack is empty, set currentNode to NULL_TVALUE
    }
}

//O(log n)
void SMIterator::next(){
    // moves the iterator to the next node in the sorted order

    if(!valid())
        throw std::exception();  // throws an exception if the iterator is not valid
    int node = this->stack.top();  // get the top node from the stack
    this->stack.pop();  // pop the top node from the stack
    if(map.bst.nodes[node].right != NULL_TVALUE)
    {
        node = map.bst.nodes[node].right;  // move to the right child of the popped node
        while(node != NULL_TVALUE) {
            this->stack.push(node);  // push the current node onto the stack
            node = map.bst.nodes[node].left;  // move to the left child of the current node
        }
    }
    if(!this->stack.empty()) {
        this->currentNode = this->stack.top();  // set the currentNode to the top of the stack
    } else {
        this->currentNode = NULL_TVALUE;  // if the stack is empty, set currentNode to NULL_TVALUE
    }
}

//O(1)
bool SMIterator::valid() const{
    // checks if the iterator is currently pointing to a valid node
    // returns false if the map is empty or the currentNode is NULL_TVALUE
    // otherwise, returns true

    if(map.isEmpty())
        return false;
	return this->currentNode != NULL_TVALUE;
}

//O(1)
TElem SMIterator::getCurrent() const{
    // returns the element at the current position of the iterator
    // throws an exception if the iterator is not valid
    // returns the info of the node stored at the currentNode in the SortedMap

    if(!valid())
        throw std::exception();
    return  map.bst.nodes[this->currentNode].info;
}


