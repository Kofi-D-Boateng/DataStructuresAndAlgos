
/**
 * @file LinkedList.h
 *
 * @author Kofi Boateng
 */

#pragma once
#include <iostream>  // for cout & cerr
#include <stdexcept> // for runtime_error
#include <ostream>   // for::ostream

// This is an implementation of a Doubly-Linked List. Since it is a doubly linked list, it can be treated similarly
// to a double ended queue.
// The nodes will hold actual data and not pointers or reference to data somewhere esle in memory.
// The nodes will be created on the heap and linked together via pointers by the nodes.
template <typename T>
class LinkedList
{
public:
    class Node
    {
    public:
        // The next node in the list, or nullptr if this is the last node.
        Node *next;
        // The previous node in the list, or nullptr if this is the first node.
        Node *prev;
        // An actual data item that this node contains.
        T data;

        // Default constructor: This lets data be constructed by
        // the default constructor of the T type.
        Node() : next(nullptr), prev(nullptr) {}

        // Argument constructor: Specifies the data that should be copied into
        // the T data member variable.
        Node(const T &dataArg) : next(nullptr), prev(nullptr), data(dataArg) {}

        // Note that although the Node class has its own copy constructor,
        // when copying an actual LinkedList, we must perform manual copying
        // by creating new nodes one at a time with the appropriate data,
        // so that the pointer connections between nodes are actually correct
        // for the copy of the list. Please refer to the copy constructor for
        // the LinkedList class itself.

        // Copy constructor: Constructs this node to be identical to the other,
        // although this one does not exist at the same memory location as other,
        // and so anything that referred to other by address is NOT automatically
        // referring to this node. (Please see the LinkedList class copy constructor.)
        Node(const Node &other) : next(other.next), prev(other.prev),
                                  data(other.data) {}

        // Copy assignment operator: Please see the note above.
        Node &operator=(const Node &other)
        {
            next = other.next;
            prev = other.prev;
            data = other.data;
            return *this;
        }

        // Destructor: Although we don't seem to write anything here,
        // the members of the node class will have their own destructors
        // automatically called afterward.
        ~Node() {}
    };

private:
    Node *head_;
    Node *tail_;
    int size_;

public:
    // getHeadPtr returns pointer to the head of the list;
    // getTailPtr returns pointer to the tail of the list;
    Node *getHeadPtr() { return head_; }
    Node *getTailPtr() { return tail_; }

    // Returns size of the list
    int size() const { return size_; }

    // Returns a boolean value based on the existence of the head pointer
    bool isEmpty() const { return !head_; }

    // pushes element to the front of the list
    void pushFront(const T &elem);
    // Pushes element to the back of the list
    void pushBack(const T &elem);
    // Delete the front element of the list;
    void popFront();
    // Delete the back element of the list;
    void popBack();

    // Delete all items in the list, leaving it empty
    void clear()
    {
        while (head_)
        {
            popBack();
        }
        if (size_ != 0)
            throw std::runtime_error("Error in clear: elements still exist on the heap... please check");
    }

    // Checks for equality between two list.
    // Two list are equal if they have the same
    // length and same data at each position. O(n).
    bool equals(const LinkedList<T> &obj) const;
    bool operator==(const LinkedList<T> &obj) const { return equals(obj) }
    bool operator!=(const LinkedList<T> &obj) const {return !equals(obj)}

    // Returns a reference to the actual front data item in the list.
    // This can be used to directly change the data in that node.
    // You must make sure that the list is not empty before you call this.
    T &front()
    {
        if (!head_)
        {
            throw std::runtime_error("front() called on empty LinkedList");
        }
        else
        {
            return head_->data;
        }
    }

    // This version of front() is overloaded to be compatible with
    // const instances of this LinkedList type.
    // For example, when you pass a LinkedList<int> by reference to a function
    // that takes an argument of type const LinkedList<int>&, then within that
    // function, the list is treated as const, and this version of front() must
    // exist or front() cannot be called on the object.
    const T &front() const
    {
        if (!head_)
        {
            throw std::runtime_error("front() called on empty LinkedList");
        }
        else
        {
            return head_->data;
        }
    }

    // Please see the note about "T& front()" above.
    T &back()
    {
        if (!tail_)
        {
            throw std::runtime_error("back() called on empty LinkedList");
        }
        else
        {
            return tail_->data;
        }
    }

    // Please see the note about "const T& front() const" above.
    const T &back() const
    {
        if (!tail_)
        {
            throw std::runtime_error("back() called on empty LinkedList");
        }
        else
        {
            return tail_->data;
        }
    }

    // Output a string representation of the list.
    // This requires that the data type T supports stream output itself.
    // This is used by the operator<< overload defined in this file.
    std::ostream &print(std::ostream &os) const;

    // Checks for pointer correctness in list. If a cycle is detected in the list,
    // we will through an error.
    // We will implement the runner algorithm (Tortoise-Haire) for cycle detection.
    void hasCycle();

    // was previously sorted. The item should be inserted before the earliest
    // item in the list that is greater. (This definition is in a separate
    // file for the homework exercises.)
    void insertOrdered(const T &newData);

    // Checks whether the list is currently sorted in increasing order.
    // This is true if for all adjacent pairs of items A and B in the list: A <= B.
    bool isSorted() const;

    // This returns a sorted copy of the current list. The sort is performed
    // using the insertion sort algorithm that relies on insertOrdered.
    // This is not an efficient operation; insertion sort is O(n^2).
    LinkedList<T> insertionSort() const;

    // Create a list of two lists, where the first list contains the first
    // half of the original list, and the second list contains the second half.
    // If the list has an odd number of elements, the first list will be larger
    // by one element. (The lists returned have copies of data and the original
    // list is unaltered.)
    LinkedList<LinkedList<T>> splitHalves() const;

    // Returns a list of new lists, where each list contains a single element
    // of the original list. For example, the original list [1, 2, 3] would be
    // returned as [[1],[2],[3]]. The data are copies, and the original list is
    // not altered.
    LinkedList<LinkedList<T>> explode() const;

    // Assuming this list instance is currently sorted, and the "other" list is
    // also already sorted, then merge returns a new sorted list containing all
    // of the items from both of the original lists, in linear time.
    // (This definition is in a separate file for the homework exercises.)
    LinkedList<T> merge(const LinkedList<T> &other) const;

    // This is a wrapper function that calls one of either mergeSortRecursive
    // or mergeSortIterative.
    LinkedList<T> mergeSort(const std::string &type) const;

    // The recursive version of the merge sort algorithm, which returns a new
    // list containing the sorted elements of the current list, in O(n log n) time.
    LinkedList<T> mergeSortRecursive() const;

    // The iterative version of the merge sort algorithm, which returns a new
    // list containing the sorted elements of the current list, in O(n log n) time.
    LinkedList<T> mergeSortIterative() const;

    // Default constructor: The list will be empty.
    LinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    // The copy assignment operator replicates the content of the other list
    // one element at a time so that pointers between nodes will be correct
    // for this copy of the list.
    LinkedList<T> &operator=(const LinkedList<T> &other)
    {
        // Clear the current list.
        clear();

        // We'll walk along the other list and push copies of its data.
        // (The declaration "const Node*" means a pointer to a const Node,
        //  not a const pointer to Node.)
        const Node *cur = other.head_;
        while (cur)
        {
            // Push back a copy of this node's data
            pushBack(cur->data);
            // Iterate
            cur = cur->next;
        }

        return *this;
    }

    // The copy constructor begins by constructing the default LinkedList,
    // then it does copy assignment from the other list.
    LinkedList(const LinkedList<T> &other) : LinkedList()
    {
        *this = other;
    }

    // The destructor calls clear to deallocate all of the nodes.
    ~LinkedList()
    {
        clear();
    }

    // Checks whether the size has been correctly updated by member functions,
    // and otherwise throws an exception. This is for testing only.
    bool assertCorrectSize() const;

    // Checks whether the reverse-direction links in the list, given by
    // the prev pointers on the nodes, are correct. If an error is found,
    // this throws an exception. This is for testing only.
    bool assertPrevLinks() const;
};

// =======================================================================
// Implementation Section
// =======================================================================

// Output a string representation of the list.
// This requires that the data type T supports stream output itself.
// This is used by the operator<< overload defined in this file.
template <typename T>
std::ostream &LinkedList<T>::print(std::ostream &os) const
{
    // List format will be [(1)(2)(3)], etc.
    os << "[";

    // Note that this works correctly for an empty list.
    Node *cur = head_;
    while (cur)
    {
        os << "(" << cur->data << ")";
        cur = cur->next;
    }

    os << "]";

    return os;
}

template <typename T>
void LinkedList<T>::hasCycle()
{
    if (this->size_ < 2)
        return;

    Node *tortoise = this->head_;
    Node *haire = this->head_;

    // While will run until the haire breaks the loop or cycle detection is confirmed
    while (tortoise && haire)
    {
        tortoise = tortoise->next->;
        haire = haire->next->next;
        // Checking for same memory address instead of data duplication.
        if (*tortoise == *haire)
            throw std::runtime_error("Error: Cycle has been detected")
    }
}

template <typename T>
void LinkedList<T>::pushBack(const T &elem)
{
    Node *newNode = new Node(elem);

    if (!head_)
    {
        this->head_ = newNode;
        this->tail_ = newNode;
    }
    else
    {
        Node *oldTail = this->tail_;
        oldTail->next = newNode;
        newNode->prev = oldTail;
        oldTail = newNode
    }
    hasCycle() this->size_++;
}

template <typename T>
void LinkedList<T>::pushFront(const T &elem)
{
    Node *newNode = new Node(elem);

    if (!head_)
    {
        this->head_ = newNode;
        this->tail_ = newNode;
    }
    else
    {
        Node *oldHead = this->head_;
        oldHead->prev = newNode;
        newNode->next = oldHead;
        oldHead = newNode;
    }
    hasCycle();
    this->size_++;
}

template <typename T>
void LinkedList<T>::popFront()
{
    if (!head_)
        return;

    if (!head_->next)
    {
        delete head_;
        head_ = nullptr;
        tail_ = nullptr;
        this->size_--;
        if (this->size_ != 0)
            throw std::runtime_error("Error in popFront: a Node still exists on the heap. Check deallocation or how you are adding nodes to list.") return;
    }

    Node *currHead = this->head_;
    head_ = currHead->next;
    head_->prev = nullptr;
    delete currHead;
    currHead = nullptr;
    this->size_--;
    return;
}

template <typename T>
void LinkedList<T>::popBack()
{
    if (!head_)
        return;

    if (!tail_->prev)
    {
        delete tail_;
        head_ = nullptr;
        tail_ = nullptr;
        this->size_--;
        if (this->size_ != 0)
            throw std::runtime_error("Error in popFront: a Node still exists on the heap. Check deallocation or how you are adding nodes to list.") return;
    }

    Node *currTail = this->head_;
    tail_ = currTail->prev;
    tail_->next = nullptr;
    delete currTail;
    currTail = nullptr;
    this->size_--;
    return;
}

template <typename T>
void LinkedList<T>::insertOrdered(const T &elem)
{
    if (this->size_ == 0)
    {
        pushFront(elem);
        return;
    }

    Node *newNode = new (elem);
    Node *currNode = this->head_;
    while (currNode && currNode->next && currNode->data < elem)
    {
        currNode = currNode->next
    }

    currNode->prev->next = newNode;
    newNode->next = currNode;
    newNode->prev = currNode->prev;
    currNode->prev = newNode;

    hasCycle();

    this->size_++;
}

template <typename T>
bool LinkedList<T>::isSorted() const
{
    if (this->size_ < 2)
        return true;

    Node *prevNode = nullptr;
    Node *currNode = this->head_;
    while (currNode->next)
    {
        prevNode = currNode;
        currNode = currNode->next;
        if (!(prevNode->data < currNode->data))
            return false;
    }

    return true;
}

template <typename T>
LinkedList<LinkedList<T>> LinkedList<T>::splitHalves() const
{

    // Returning List
    LinkedList<LinkedList<T>> halves;

    LinkedList<T> leftHalf = *this;

    LinkedList<T> rightHalf;

    if (this->size_ < 2)
    {
        halves.pushBack(leftHalf);
        halves.pushBack(rightHalf);
        return halves;
    }

    for (int i = 0; i < this->size_; i++)
    {
        T copyOfData = leftHalf.back();
        rightHalf.pushBack(copyOfData);

        leftHalf.popBack();
    }

    halves.pushBack(leftHalf);
    havles.pushBack(rightHalf);

    return halves;
}

template <typename T>
LinkedList<LinkedList<T>> LinkedList<T>::explode() const
{

    LinkedList<LinkedList<T>> explodedList;
    LinkedList<T> copy = *this;
    while (!copy.isEmpty())
    {
        LinkedList<T> singletonList;
        singletonList.pushBack(copy.front());
        explodedList.pushBack(singletonList);
        copy.popFront();
    }
    return explodedList;
}

template <typename T>
LinkedList<T> LinkedList<T>::merge(const LinkedList<T> &obj) const
{

    LinkedList<T> copy = *this, LinkedList<T> mergedList;
    Node *copyNode = copy.getHeadPtr(), objNode = obj.getHeadPtr();
    int copyCount = 0, objCount = 0;
    while (copyCount < copy.size_ && objCount < obj.size_)
    {
        while (copyNode->data < objNode->data)
        {
            mergedList.pushBack(copyNode->data);
            copyNode = copyNode->next;
            copyCount++;
        }

        while (objNode->data < copyNode->data)
        {
            mergedList.pushBack(objNode->data);
            objNode = objNode->next;
            objCount++;
        }
    }

    // Clean up left over nodes (i.e. if Copy size was originally 4 and obj's size was 2) that were not merged over.
    while (copyCount < copy.size_)
    {
        mergedList.pushBack(copyNode->data);
        copyNode = copyNode->next;
        copyCount++;
    }

    while (objCount < obj.size_)
    {
        mergedList.pushBack(objNode->data);
        objNode = objNode->next;
        objCount++;
    }

    return mergedList;
}

template <typename T>
LinkedList<T> LinkedList<T>::mergeSortRecursive() const
{
    // Base case
    if (size_ < 2)
        return *this;

    // Merge sort requires a Divide and Conquer approach.
    // Split the array from the mid point until singleton remains.
    LinkedList<T> halves = splitHalves();

    // Get the halves of each side into two separate variables;
    LinkedList<T> leftSide = halves.front();
    LinkedList<T> rightSide = halves.back();

    // Recurse above procedures
    leftSide = leftSide.mergeSortRecursive();
    rightSide = rightSide.mergeSortRecursive();

    // Merge halves back into a single list.
    return leftSide.merge(rightSide);
}

template <typename T>
LinkedList<T> LinkedList<T>::mergeSortIterative() const
{

    // Create a queue (we will use built-in class functionality).
    // Please see explode function for understanding of implementation.
    LinkedList<LinkedList<T>> queue = explode();

    while (queue.size_ != 1)
    {
        LinkedList<T> left = queue.front();
        queue.popFront();
        LinkedList<T> right = queue.front();
        queue.popFront();

        LinkedList<T> merged = left.merge(right);

        queue.pushBack(merged);
    }

    // The last element in the queue should be the merged list.
    return queue.front()
}

template <typename T>
LinkedList<T> LinkedList<T>::mergeSort(const std::string &type) const
{
    if (type == "recursive")
    {
        return mergeSortRecursive();
    }
    return mergeSortIterative()
}
