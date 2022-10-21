
/**
 * @file Queue.h
 * @author Kofi Boateng
 * @version 0.1
 * @date 2022-11-02
 *
 *
 */

#pragma once
#include <iostream>  // for cout & cerr
#include <stdexcept> // for runtime_error
#include <ostream>   // for::ostream

// This is an implementation of a Queue ADT. A Queue is a a structure that
// follows the LIFO principles. Therefore, we can use a doubly linked list
// As the base data structure to hold info (Please look at LinkedList.h for
// for more on implementation.)

template <typename T>
class QueueADT
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

        // Copy constructor: Constructs this node to be identical to the other,
        // although this one does not exist at the same memory location as other,
        // and so anything that referred to other by address is NOT automatically
        // referring to this node.
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

    // Pushes element into the queue.
    void enqueue(const T &elem);

    // Remove element from the front of the queue.
    void dequeue();

    // Delete all items in the list, leaving it empty
    void clear()
    {
        while (head_)
        {
            dequeue();
        }
        if (size_ != 0)
            throw new std::runtime_error("Error in clear: elements still exist on the heap... please check");
    }

    // Checks for equality between two list.
    // Two list are equal if they have the same
    // length and same data at each position. O(n).
    bool equals(const QueueADT<T> &obj) const;
    bool operator==(const QueueADT<T> &obj) const { return equals(obj); }
    bool operator!=(const QueueADT<T> &obj) const { return !equals(obj); }

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

    // Default constructor: The list will be empty.
    QueueADT() : head_(nullptr), tail_(nullptr), size_(0) {}

    // The copy assignment operator replicates the content of the other list
    // one element at a time so that pointers between nodes will be correct
    // for this copy of the list.
    QueueADT<T> &operator=(const QueueADT<T> &other)
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
            push(cur->data);
            // Iterate
            cur = cur->next;
        }

        return *this;
    }

    // The copy constructor begins by constructing the default LinkedList,
    // then it does copy assignment from the other list.
    QueueADT(const QueueADT<T> &other) : QueueADT()
    {
        *this = other;
    }

    // The destructor calls clear to deallocate all of the nodes.
    ~QueueADT()
    {
        clear();
    }
};

// =====================================================================================
// Implementation Stage
// =====================================================================================

template <typename T>
std::ostream &QueueADT<T>::print(std::ostream &os) const
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

    os << "]\n";

    return os;
}

template <typename T>
void QueueADT<T>::enqueue(const T &elem)
{
    Node *newNode = new Node(elem);

    if (!head_)
    {
        head_ = newNode;
        tail_ = newNode;
    }
    else
    {

        tail_->next = newNode;
        newNode->prev = tail_;
        tail_ = newNode;
    }
    size_++;
}

template <typename T>
void QueueADT<T>::dequeue()
{
    if (!head_)
        return;
    else
    {
        Node *oldHead = this->head_;
        head_ = oldHead->next;

        delete oldHead;

        oldHead->next = nullptr;
        oldHead->prev = nullptr;
    }
    this->size_--;
}

template <typename T>
bool QueueADT<T>::equals(const QueueADT<T> &other) const
{
    if (size_ != other.size_)
        return false;

    Node *thisNode = getHeadPtr();
    Node *otherNode = other.getHeadPtr();
    while (thisNode)
    {
        if (!otherNode)
        {
            throw new std::runtime_error("Error: Passed queue is missing references to nodes on the heap. Check implementation.");
        }
        else if (thisNode->data != otherNode->data)
        {
            return false;
        }
        else
        {
            thisNode = thisNode->next;
            otherNode = otherNode->next;
        }
    }

    return true;
}
