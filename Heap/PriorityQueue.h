/**
 * @file PriortyQueue.h
 * @author Kofi Boateng
 * @version 0.1
 * @date 2022-10-17
 *
 *
 */

#pragma once
#include <iostream>  // for cout & cerr
#include <stdexcept> // for runtime_error
#include <ostream>   // for::ostream

// This is an implementation of the PriorityQueue Abstract Data Type. The underlying data structure
// that this API will interact with is a minimum heap. This API will allow the end user to retrieve
// The smallest element in constant time (O(1)) and insert elements to be ordered in logarithmic time (O(n*log(n))).

template <typename T>
class PriorityQueueADT
{

private:
    // Pointer to the Min Heap.
    T *minHeap;

    // Size of actual data stored within the heap.
    int size_ = {0};

    // Initial capacity of the array. This flag will
    // increase base on how many times we grow our array.
    int capacity_ = {8};

    // Grows the array when we have reached full capacity.
    // We will double the size of the array for every increase.
    // This will give us a amortized O(1*) runtime for this operation.
    void _increaseCapacity();

    // A function that will bubble up nodes after insertion to restore the heap invariant.
    void _heapifyUp(const int &index);

    // A function that will bubble down nodes after removal of the minimum node to restore
    // the heap invariant.
    void _heapifyDown(const int &index);

    // Finds where the leaf elements are in the heap.
    bool _isLeaf(const int &index) const;

    // Returns the index of the minimum child;
    int _minChild(const int &index);

    // Empties out the array.
    void _clear();

public:
    // Inserts an element in the heap.
    void insert(const T &element);

    // Removes the minimal element of the tree.
    void removeMin();

    // Returns a boolean signifying if the tree is
    // empty or not.
    bool isEmpty() const { return size_ == 0; }

    // Returns the size of the tree.
    int size() const { return size_; };

    // Retrieves the top element of the tree, but does not remove it.
    // To remove the element after peeking, call the removal function.
    T peek() const
    {
        if (size_ < 1)
        {
            throw new std::runtime_error("Error: Cannot peek empty array. Please check where peek is called.");
        }
        return minHeap[1];
    }

    // Outputs the cotnents of the heap into a string format.
    std::ostream &print(std::ostream &os) const;

    PriorityQueueADT() : minHeap(new T[8]), size_(0), capacity_(8) {}

    PriorityQueueADT(const PriorityQueueADT<T> &other) : minHeap(other.minHeap), size_(other.size_), capacity_(other.capacity_) {}

    ~PriorityQueueADT()
    {
        delete[] minHeap;
    }
};
// ======================================================================================================================================
// Implementation Section
// ======================================================================================================================================

template <typename T>
void PriorityQueueADT<T>::_increaseCapacity()
{
    // Double the capacity of the area and initial a working copy.
    int newSize = capacity_ * 2;
    T *copyArray = new T[newSize];

    // Copies over all elements into their correct spots
    for (int i = 1; i < size_; i++)
    {
        copyArray[i] = minHeap[i];
    }
    T *oldHeap = minHeap;
    // Set the new min heap.
    minHeap = copyArray;

    delete[] oldHeap;
}

template <typename T>
bool PriorityQueueADT<T>::_isLeaf(const int &index) const
{
    return index == size_ || index == size_ - 1;
}

template <class T>
int PriorityQueueADT<T>::_minChild(const int &index)
{
    T leftChild = minHeap[index * 2];
    T rightChild = minHeap[index * 2 + 1];
    if (leftChild < rightChild)
    {
        return index * 2;
    }
    return index * 2 + 1;
}

template <typename T>
void PriorityQueueADT<T>::_heapifyUp(const int &index)
{
    if (index > 1)
    {
        if (minHeap[index] < minHeap[index - 1])
        {
            std::swap(minHeap[index], minHeap[index / 2]);
            _heapifyUp(index / 2);
        }
    }
}

template <typename T>
void PriorityQueueADT<T>::_heapifyDown(const int &index)
{
    if (!_isLeaf(index))
    {
        int minChildIndex = _minChild(index);
        if (minHeap[index] > minHeap[minChildIndex])
        {
            std::swap(minHeap[index], minHeap[minChildIndex]);
            _heapifyDown(minChildIndex);
        }
    }
}

template <class T>
void PriorityQueueADT<T>::insert(const T &element)
{

    // Null root edge case
    // The array acts as tree when we consider position 0 in the array
    // to be a placeholder, or "occupied". Therefore, our initial "tree"
    // will begin at position 1. The left and right childs will be placed
    // into the array as if they are following the pattern of a tree as
    // follows: Left child = parents key(index) * 2, Right child = (parent's key(index) * 2) + 1.
    if (size_ == capacity_)
    {
        _increaseCapacity();
    }

    // Increments the size key first before insertion.
    minHeap[++size_] = element;
    _heapifyUp(size_);
}

template <typename T>
void PriorityQueueADT<T>::removeMin()
{

    if (size_ == 0)
    {
        throw new std::runtime_error("Error: Trying to remove an element on an empty heap.");
    }

    // We will swap the top element with the last element in our heap
    // and decrement the size.
    T tempLastElement = minHeap[size_];
    minHeap[1] = tempLastElement;
    size_--;

    _heapifyDown(1);
}

template <typename T>
std::ostream &PriorityQueueADT<T>::print(std::ostream &os) const
{
    os << "[";

    // Note that this works correctly for an empty list.
    for (int i = 1; i < capacity_; i++)
    {
        os << "(" << minHeap[i] << ")";
    }
    os << "] \n";

    return os;
}
