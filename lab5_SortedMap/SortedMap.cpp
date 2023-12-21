#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>
#include <vector>
using namespace std;

//O(n) - n capacity
SortedMap::SortedMap(Relation r)
{
    // Constructor for SortedMap
    this->relation = r;

    // Initialize the Binary Search Tree
    this->bst.capacity = 10;
    this->bst.root = 0;
    this->bst.size = 0;
    this->bst.nodes = new BSTNode[this->bst.capacity];
    this->bst.firstEmpty = 0;

    // Initialize all nodes in the array to have NULL_TPAIR and NULL_TVALUE
    for (int i = 0; i < this->bst.capacity; i++)
    {
        this->bst.nodes[i].info = NULL_TPAIR;
        this->bst.nodes[i].left = NULL_TVALUE;
        this->bst.nodes[i].right = NULL_TVALUE;
    }

}

// O(n), n - capacity of the BST
void SortedMap::recomputeFirstEmpty()
{
    // Recompute the index of the first empty node in the Binary Search Tree
    while (this->bst.nodes[this->bst.firstEmpty].info.first != NULL_TVALUE || this->bst.nodes[this->bst.firstEmpty].info.second != NULL_TVALUE)
    {
        this->bst.firstEmpty++;
    }
}

// O(n), n - the current capacity
void SortedMap::resize()
{
    // Resizes the Binary Search Tree by doubling its capacity
    BSTNode* aux = new BSTNode[this->bst.capacity * 2];

    // Copy the existing nodes to the new array
    for (int i = 0; i < this->bst.capacity; i++) {
        aux[i] = this->bst.nodes[i];
    }

    // Initialize the new nodes in the array to have NULL_TPAIR and NULL_TVALUE
    for (int i = this->bst.capacity; i < this->bst.capacity * 2; i++)
    {
        aux[i].info = NULL_TPAIR;
        aux[i].left = NULL_TVALUE;
        aux[i].right = NULL_TVALUE;
    }

    // Recompute the index of the first empty node
    recomputeFirstEmpty();

    // Update the capacity and replace the old array with the new one
    this->bst.capacity *= 2;
    delete[]this->bst.nodes;
    this->bst.nodes = aux;
}

// O(log n), n - the number of nodes in the BST
int SortedMap::insert_recursively(int node, TElem e)
{
    // Inserts a node with the given element recursively in the Binary Search Tree
    if (node == NULL_TVALUE)
    {
        // Found an empty node, insert the element and update the first empty index
        this->bst.nodes[this->bst.firstEmpty].info = e;
        int old = this->bst.firstEmpty;
        recomputeFirstEmpty();
        return old;
    }
    else if (!this->relation(this->bst.nodes[node].info.first, e.first))
    {
        // Element should be inserted in the left subtree
        this->bst.nodes[node].left = insert_recursively(this->bst.nodes[node].left, e);
        return node;
    }
    else {
        // Element should be inserted in the right subtree
        this->bst.nodes[node].right = insert_recursively(this->bst.nodes[node].right, e);
        return node;
    }
}

//O(log n), n - the number of nodes in the BST
TValue SortedMap::add(TKey k, TValue v) {
    // adds a pair (key,value) to the map
    //if the key already exists in the map, then the value associated to the key is replaced by the new value and the old value is returned
    //if the key SMes not exist, a new pair is added and the value null is returned
    if (search(k) == NULL_TVALUE)
    {
        // Key does not exist, add a new node with the given key-value pair
        if (isEmpty())
        {
            // Empty tree, create the root node
            this->bst.nodes[0].info = make_pair(k, v);
            this->bst.firstEmpty = 1;
            this->bst.size++;
            return NULL_TVALUE;
        }
        if (this->bst.size == this->bst.capacity - 1)
            resize();
        this->insert_recursively(this->bst.root, make_pair(k, v));
        this->bst.size++;
        return NULL_TVALUE;
    }
    else
    {
        //replace the old value associated with key k with the new one
        TValue old = search(k);
        int current = this->bst.root;
        while (current != NULL_TVALUE)
        {
            if (this->relation(this->bst.nodes[current].info.first, k))
            {
                if (k == this->bst.nodes[current].info.first)
                {
                    this->bst.nodes[current].info.second = v;
                    return old;
                }
                current = this->bst.nodes[current].right;
            }
            else
            {
                current = this->bst.nodes[current].left;
            }
        }
        return old;

    }
}

// O(log n), n - the number of nodes in the BST
TValue SortedMap::search(TKey k)
{
    // returns the value associated with a key, if the key exists in the map, or null: NULL_TVALUE otherwise
    int current = this->bst.root;
    while (current != NULL_TVALUE) {
        if (this->relation(this->bst.nodes[current].info.first, k)) {
            if (k == this->bst.nodes[current].info.first)
                return this->bst.nodes[current].info.second;
            current = this->bst.nodes[current].right;
        }
        else {
            current = this->bst.nodes[current].left;
        }
    }
    return NULL_TVALUE;
}

// O(log n), n - the number of nodes in the BST
TValue SortedMap::remove(TKey k) {
    // Removes a key from the map and returns the value associated with the key if the key exists, or null: NULL_TVALUE otherwise
    int current = this->bst.root;
    int parent = NULL_TVALUE;

    // Find the node to be removed and its parent node
    while (current != NULL_TVALUE && this->bst.nodes[current].info.first != k) {
        parent = current;
        if (this->relation(this->bst.nodes[current].info.first, k))
            current = this->bst.nodes[current].right;
        else
            current = this->bst.nodes[current].left;
    }

    // Key not found, return NULL_TVALUE
    if (current == NULL_TVALUE)
        return NULL_TVALUE;

    // Save the value of the node to be removed
    TValue value = this->bst.nodes[current].info.second;

    // Case 1: Node has no children
    if (this->bst.nodes[current].left == NULL_TVALUE && this->bst.nodes[current].right == NULL_TVALUE) {
        if (parent == NULL_TVALUE)
            this->bst.root = NULL_TVALUE;
        else if (this->bst.nodes[parent].left == current)
            this->bst.nodes[parent].left = NULL_TVALUE;
        else
            this->bst.nodes[parent].right = NULL_TVALUE;
    }
        // Case 2: Node has only left child
    else if (this->bst.nodes[current].right == NULL_TVALUE) {
        if (parent == NULL_TVALUE)
            this->bst.root = this->bst.nodes[current].left;
        else if (this->bst.nodes[parent].left == current)
            this->bst.nodes[parent].left = this->bst.nodes[current].left;
        else
            this->bst.nodes[parent].right = this->bst.nodes[current].left;
    }
        // Case 3: Node has only right child
    else if (this->bst.nodes[current].left == NULL_TVALUE) {
        if (parent == NULL_TVALUE)
            this->bst.root = this->bst.nodes[current].right;
        else if (this->bst.nodes[parent].left == current)
            this->bst.nodes[parent].left = this->bst.nodes[current].right;
        else
            this->bst.nodes[parent].right = this->bst.nodes[current].right;
    }
        // Case 4: Node has both left and right children
    else {
        int successor = this->bst.nodes[current].right;
        int successorParent = current;

        // Find the minimum value in the right subtree as the successor
        while (this->bst.nodes[successor].left != NULL_TVALUE) {
            successorParent = successor;
            successor = this->bst.nodes[successor].left;
        }

        // Replace the key-value pair of the current node with the successor's key-value pair
        this->bst.nodes[current].info = this->bst.nodes[successor].info;

        // Update the parent's left or right child reference to the successor's right child
        if (successorParent == current)
            this->bst.nodes[successorParent].right = this->bst.nodes[successor].right;
        else
            this->bst.nodes[successorParent].left = this->bst.nodes[successor].right;

        current = successor; // Set the current node to the successor for further deletion
    }


    this->bst.nodes[current].info = NULL_TPAIR;
    this->bst.nodes[current].left = NULL_TVALUE;
    this->bst.nodes[current].right = NULL_TVALUE;

    this->bst.size--; // Decrease the size of the map

    return value; // Return the value associated with the removed key
}

//O(1)
int SortedMap::size() const {
    // returns the number of key-value pairs in the map
    return this->bst.size;
}

//O(1)
bool SortedMap::isEmpty() const {
    // Checks if the map is empty
    if (this->bst.size == 0)
        return true;
    return false;
}

SMIterator SortedMap::iterator() const {
    // returns an iterator over the map
    return SMIterator(*this);
}

SortedMap::~SortedMap() {
    // destructor for SortedMap
    delete[]this->bst.nodes;
}

int SortedMap::addIfNotPresent(SortedMap &sm) {
    int addedPairs = 0;
    SMIterator it(sm);

    while(it.valid()) {
        TKey key = it.getCurrent().first;

        if(this->search(key) == NULL_TVALUE)  // Check if the key is not already present in the current SortedMap
        {
            TValue  value = it.getCurrent().second;
            add(key, value);  // Add the key-value pair to the current SortedMap
            addedPairs++;
        }

        it.next();  // Move to the next element in the sm SortedMap
    }

    return addedPairs;
}
