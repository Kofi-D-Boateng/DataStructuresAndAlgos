/**
 * @file Stack.h
 * @author Kofi Boateng
 * @version 0.1
 * @date 2022-10-11
 *
 *
 */

#pragma once
#include <iostream>  // for cout & cerr
#include <stdexcept> // for runtime_error
#include <ostream>   // for::ostream

// This is an implementation of a Stack.
// The class Stack will have access to a pointer to
// a node class that hold actual data and not pointers
// to data.
// All stack nodes will be created on the heap memory as well.

template <typename T>
class Stack
{
public:
    class Node
    {
        // Pointer to the next node in the list
        Node *next;
        // Actual Data
        T data;
        // Default constructor: This lets data be constructed by
        // the default constructor of the T type.
        Node() : next(nullptr) {}
        // Argument constructor: Specifies the data that should be copied into
        // the T data member variable.
        Node(const T &dataArg) : next(nullptr), data(dataArg) {}
        // Copy constructor: Constructs this node to be identical to the other,
        // although this one does not exist at the same memory location as other,
        // and so anything that referred to other by address is NOT automatically
        // referring to this node. (Please see the LinkedList class copy constructor.)
        Node(const Node &other) : next(other.next), data(other.data) {}
        // Copy assignment operator: Please see the note above.
        Node &operator=(const Node &other)
        {
            next = other.next;
            data = other.data;
            return *this
        }
        // Destructor: Although we don't seem to write anything here,
        // the members of the node class will have their own destructors
        // automatically called afterward.
        ~Node() {}
    };

private:
    Node *head_;
    int size_;

public:
    // Retrieve Head Pointer of the list
    Node *getHeadPtr() { return head_; }
    // Retrieve Size of the list
    int size() const { return size_; }
    // Checks for equality between two list.
    // Two list are equal if they have the same
    // length and same data at each position. O(n).
    bool equals(const Stack<T> &obj) const;
    bool operator==(const Stack<T> &obj) const { return equals(obj) }
    bool operator!=(const Stack<T> &obj) const {return !equals(obj)}

    // Returns a reference to the top element's data in the stack.
    // This can be used to change data directly within its respective node.
    T &top()
    {
        if (!head_)
        {
            throw std::runtime_error("Error: There is no head pointer for Stack")
        }
        else
        {
            return head_->data
        }
    }
    // (Reference above comments). Satisfies passes by refernece between through functions.
    const T &top() const
    {
        if (!head_)
        {
            throw new std::runtime_error("Error: There is no head pointer for Stack")
        }
        else
        {
            return head_->data
        }
    }
    // Checks if the stack is empty.
    bool isEmpty() const { return !head_; }
    // Push a node on top of the stack.
    void push(const T &dataArg);
    // Pop a node from the top of the stack.
    void pop();
    // Peek at the top of the stack without removing it from the stack.
    T &peek() const;

    // Delete all items from the stack, rendering it empty.
    void clear()
    {
        while (head_)
        {
            pop();
        }
        if (size_ != 0)
            throw new std::runtime_error("Error nodes from the stack still exist in heap memory.")
    }
    // Output a string representation of the list.
    // This requires that the data type T supports stream output itself.
    // This is used by the operator<< overload defined in this file.
    std::ostream &print(std::ostream &os) const;

    // Default Stack Destructor
    Stack() : head_(nullptr), size_(0) {}

    // The copy assignment replicates the content of the other list
    // one element at a time.
    Stack<T> &operator=(const Stack<T> &other)
    {
        // Clear the current list
        clear();

        Node *curr = other.head_;
        while (curr)
        {
            // Push an element to the top of the list
            push(curr->data);
            // Iterate to next node;
            curr = curr->next;
        }

        return *this
    }

    // The copy constructor begins by constructing the default of Stack,
    // then it copies the other list
    Stack(const Stack<T> &other) : Stack()
    {
        *this = other;
    }

    // The destructor calls clear for deallocation
    ~Stack()
    {
        clear();
    }
};

// ===================================================================================
// Implementation Section
// ===================================================================================

template <typename T>
std::ostream &Stack<T>::print(std::ostream &os) const
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
void Stack<T>::push(const T &dataArg)
{
    Node *newData = new Node(dataArg);

    if (!head_)
    {
        head_ = newData;
    }
    else
    {
        Node *oldHead = getHeadPtr();
        newData->next = oldHead;
    }
    size_++;
}

template <typename T>
void Stack<T>::pop()
{
    if (!head_)
    {
        return;
    }
    else
    {
        Node *oldHead = getHeadPtr();
        Node *newHead = oldHead->next;

        head_ = newHead;

        delete oldHead;
        oldHead->next = nullptr;
    }
    size_--;
}

template <typename T>
bool Stack<T>::equals(const Stack<T> &other) const
{
    if (size_ != other.size_)
        return false;

    Node *currHead = head_;
    Node *otherHead = other->head_;

    while (currHead)
    {
        if (!otherHead)
        {
            throw new std::runtime_error("Error: Size of list is unbalanced compared to current list.");
        }
        else
        {
            if (currHead->data != otherHead->data)
            {
                return false;
            }
            currHead = currHead->next;
            otherHead = otherHead->next;
        }

        return true;
    }
}

template <typename T>
T &Stack<T>::peek() const
{

    return head_->data
}
