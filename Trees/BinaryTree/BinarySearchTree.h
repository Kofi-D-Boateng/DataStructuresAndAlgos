/**
 * @file BinarySearchTree.h
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
#include <queue>     // For BFS algorithms

// This is an implementation of a BinarySearchTree (BST). A BST is a type
// of tree which follows the the tree invariant as well as every node to
// the left of the parent node decreases and every node to right increases.

template <typename T>
class BinarySearchTree
{
public:
    class Node
    {
    public:
        // The left pointer of the parent node. The data in this node should be
        // less than the parent node.
        Node *left;
        // The left pointer of the parent node. The data in this node should be
        // less than the parent node.
        Node *right;

        // The data of the node.
        T data;

        // Default constructor: This lets data be constructed by
        // the default constructor of the T type.
        Node() : left(nullptr), right(nullptr) {}

        // Argument constructor
        Node(const T &dataArg) : left(nullptr), right(nullptr), data(dataArg) {}

        // Copy constructor: Constructs a new node to be identical to the node being
        // copied.
        Node(const Node &other) : left(other.left), right(other.right), data(other.data) {}

        // Copy assignment operator
        Node &operator=(const Node &other)
        {
            left = other.left;
            right = other.right;
            data = other.data;
            return *this
        }

        // Destructor: Although we don't seem to write anything here,
        // the members of the node class will have their own destructors
        // automatically called afterward.
        ~Node() {}
    };

private:
    Node *root;
    int treeSize;

    // ClearTree is used to recursively remove elements from the
    // the tree during deallocation via a Post-Order Traversal.
    void clearTree(Node *node);

    // Searches the tree for an element and will return a bool.
    // Type will refer to which type of search to perform DFS or BFS.
    bool binarySearch(const T &src, std::string type);

    // DFS Binary search to be called with binarySearch
    BinarySearchTree<T>::Node *binarySearchDFS(Node *node, const T &src);

    // BFS Binary search to be called with binarySearch
    BinarySearchTree<T>::Node *binarySearchBFS(Node *node, const T &src);

    // DFS helper for insertions
    void DFSInsertHelper(const T &element, Node *node);
    // BFS helper for insertions
    void BFSInsertHelper(const T &element, Node *node);

    // DFS helper for removals
    void DFSRemoveHelper(const T &element, Node *node);
    // BFS helper for removals
    void BFSRemoveHelper(const T &element, Node *node);

    // Retrieves the pointer to parent node of the node we are trying to remove.
    // We want to retrieve this node instead of the actual node itself in order to
    // manually update pointers correctly.
    BinarySearchTree<T>::Node *retrieveParentNodeDFS(const T &element, Node *node);

    // Refer to above comment in DFS version.
    BinarySearchTree<T>::Node *retrieveParentNodeBFS(const T &element, Node *node);

    // A helper function that will retrieve the parent of the farthest node right
    // in the left subtree where the Ancestor node that calls the function will be swapped
    // with the child of the parent node.
    BinarySearchTree<T>::Node *retrieveFurthestRightNodeDFS(Node *node);

    // Refer to notes in the DFS version.
    BinarySearchTree<T>::Node *retrieveFurthestRightNodeBFS(Node *node);

    // Returns the calculated height of the tree. A leaf node will be calculated with -1 height,
    // and we will subtract the height of the right side of a parent node from the left side.
    int calculateHeightOfTree(Node *node) const;

public:
    // Gets the root of the tree
    Node *
    getRoot()
    {
        return root;
    }

    // Gets the amount of elements currently in the tree
    int size() const { return treeSize; }

    // Returns a boolean whether the exists or not
    bool isEmpty() const { return !getRoot() }

    // Returns a boolean if the element is already in the tree.
    // We will not, for intents and purpose, allow duplicate
    // values in the tree. This fucntion will check for that as
    // a by product.
    bool contains(const T &element);

    // Inserts an element into the tree
    // Type references wheter you use DFS or BFS Helper function.
    void insert(const T &element, std::string type);

    // Removes an element from the tree
    // Type references wheter you use DFS or BFS Helper function.
    void remove(const T &element, std::string type);

    // Checks if the tree is balanced
    bool isBalanced();

    // Deletes all the elements from the tree
    void clear()
    {
        if (root)
            clearTree(root);

        if (treeSize != 0)
        {
            throw new std::runtime_error("Error in clear: elements still exist on the heap... please check");
        }
    }

    // Checks for equality between two list.
    // Two list are equal if they have the same
    // length and same data at each position. O(n).
    bool equals(const BinarySearch<T> &obj) const;
    bool operator==(const BinarySearch<T> &obj) const { return equals(obj) }
    bool operator!=(const BinarySearch<T> &obj) const {return !equals(obj)}

    // Output a string representation of the list.
    // This requires that the data type T supports stream output itself.
    // This is used by the operator<< overload defined in this file.
    std::ostream &print(std::ostream &os) const;

    // Default Constructor: creates an empty tree
    BinarySearchTree() : root(nullptr), treeSize(0) {}

    // We will run a BFS algorithm to copy nodes at each level
    BinarySearchTree<T> &operator=(const BinarySearchTree<T> &other)
    {

        clear();

        // Queue for going through list;
        std::queue<Node *> queue;

        queue.back(other.root);

        // This loop will do an preOrder traversal constructing the tree at
        // each level based on the snapshot size of the queue with a TC: O(n).
        while (!queue.empty())
        {
            int size = queue.size();

            while (size > 0)
            {
                Node *currNode = queue.front();
                queue.pop();
                insert(currNode->data);
                if (currNode->left)
                    queue.back(currNode->left.data);
                if (currNode->right)
                    queue.back(currNode->right.data);
            }
        }

        return *this
    }

    // The copy constructor begins by constructing the default LinkedList,
    // then it does copy assignment from the other list.
    BinarySearchTree(const BinarySearchTree<T> &other) : BinarySearchTree()
    {
        *this = other;
    }

    // Deconstructor of the tree;
    ~BinarySearchTree()
    {
        clear();
    }
};

// ===================================================================================
// Implementation Section
// ===================================================================================
// Output a string representation of the list.
// This requires that the data type T supports stream output itself.
// This is used by the operator<< overload defined in this file.
template <typename T>
std::ostream &BinarySearchTree<T>::print(std::ostream &os) const
{
    // List format will be [(1)(2)(3)], etc.
    os << "[";

    // Note that this works correctly for an empty list.
    Node *cur = root;
    while (cur)
    {
        os << "(" << cur->data << ")";
        cur = cur->next;
    }

    os << "]";

    return os;
}

template <typename T>
BinarySearchTree<T>::Node *BinarySearchTree<T>::retrieveFurthestRightNodeDFS(Node *node)
{
    if (!node->right->left && !node->right->right)
        return node;

    return retrieveFurthestRightNodeDFS(node->right);
}

template <typename T>
BinarySearchTree<T>::Node *BinarySearchTree<T>::retrieveFurthestRightNodeBFS(Node *node)
{
    std::queue<Node *> queue;
    Node *parent = nullptr;
    queue.back(node);

    while (!queue.empty())
    {
        Node *currNode = queue.front();
        queue.pop();
        if (!currNode->right->left && !currNode->left->right)
        {
            parent = currNode;
            break;
        }
        else
        {
            if (currNode->right)
                queue.back(currNode->right);
        }
    }

    return parent;
}

// Helper function for Insert
template <typename T>
void BinarySearchTree<T>::DFSInsertHelper(const T &element, Node *node)
{

    if (element > node->data && node->right)
    {
        DFSInsertHelper(element, node->right);
    }
    if (element < node->data && node->left)
    {
        DFSInsertHelper(element, node->left);
    }

    Node newNode = new Node(element);
    if (newNode->data < node->data)
    {
        node->left = newNode;
    }
    else
    {
        node->right = newNode;
    }
    treeSize++;

    return;
}

// Helper function for Insert
template <typename T>
void BinarySearchTree<T>::BFSInsertHelper(const T &element, Node *node)
{
    std::queue<Node *> queue;
    queue.back(node);

    while (!queue.empty())
    {
        Node *currNode = queue.front();
        queue.pop();
        if (element < currNode->data && currNode->left)
        {
            queue.back(currNode->left);
        }
        else if (element > currNode->data && currNode->right)
        {
            queue.back(currNode->right);
        }
        else
        {
            Node *newNode = new Node(element);

            if (newNode->data < currNode->data)
            {
                currNode->left = newNode;
            }
            else
            {
                currNode->right = newNode;
            }
            treeSize++;
        }
    }
}

template <typename T>
void BinarySearchTree<T>::DFSRemoveHelper(const T &element, Node *node)
{
    if (!node->left && !node->right)
        return;

    if (element > node->data)
    {
        DFSRemoveHelper(element, node->right);
    }
    else
    {
        DFSRemoveHelper(element, node->left);
    }

    delete node;
    node->right = nullptr;
    node->left = nullptr;

    treeSize--;
    return;
}

template <typename T>
int BinarySearchTree<T>::calculateHeightOfTree(Node *node) const
{
    return !node ? -1 : calculateHeightOfTree(node->right) - calculateHeightOfTree(node->left);
}

// DFS search algorithm that returns a pointer to a node on the heap.
template <typename T>
BinarySearchTree<T>::Node *BinarySearchTree<T>::binarySearchDFS(Node *node, const T &src)
{
    if (!node)
        return nullptr;
    if (node->data == src)
        return node;
    if (src > node->data)
        binarySearchDFS(node->right, src);
    if (src < node->data)
        binarySearchDFS(node->left, src);

    return binarySearch(node, src);
}

// BFS search algorithm that returns a pointer to a node on the heap.
template <typename T>
BinarySearchTree<T>::Node *BinarySearchTree<T>::binarySearchBFS(Node *node, const T &src)
{
    std::queue<Node *> queue;
    queue.back(node);

    while (!queue.empty())
    {
        Node *currNode = queue.front();
        queue.pop();
        if (currNode->data == src)
        {
            return currNode;
        }
        else if (src > currNode->data && currNode->right)
        {
            queue.back(currNode->right);
        }
        else if (src < currNode->data && currNode->left)
        {
            queue.back(currNode->left);
        }
    }

    return nullptr;
}

template <typename T>
void BinarySearchTree<T>::clearTree(Node *node)
{
    if (!node->left && !node->right)
        return;

    clearTree(node->left);
    clearTree(node->right);

    delete node;
    node->left = nullptr;
    node->right = nullptr;

    treeSize--;
    return;
}

template <typename T>
void BinarySearchTree<T>::insert(const T &arg, std::string type)
{
    if (!root)
    {
        Node *newRoot = new Node(arg);
        root = newRoot;
        treeSize++;
    }
    else
    {
        if (type == "DFS")
        {
            DFSInsertHelper(arg, root)
        }
        else if (type == "BFS")
        {
            BFSInsertHelper(arg, root);
        }
        else
        {
            throw new std::runtime_error("Error in insert: Type was not BFS or DFS. Please enter correct type");
        }
    }
}

template <typename T>
void BinarySearchTree<T>::remove(const T &element, std::string type)
{
    if (type != "DFS" || type != "BFS")
    {
        throw new std::runtime_error("Error in remove: incorrect type offered. Choose between DFS and BFS");
    }
    if (type == "DFS")
    {
        // Phase One: Find the parent node of the node designated for removal;
        Node *parent = retrieveParentNodeDFS(element, root);
        // Phase One-B: Edit the correct side of the tree
        if (parent->left && parent->left->data == element)
        {
            // Phase Two: Removal of node. (Bound by 3 cases).
            Node *foundNode = parent->left;
            // Case 1 (Leaf Node || No children)
            if (!foundNode->left || !foundNode->right)
            {
                DFSRemoveHelper(element, root)
                    parent->left = nullptr;
            }
            // Case 2 (Single Child)
            // Note: Each node should be regarded as a sub-tree and the parent node it
            // comes from as a root node itself. Therefore, even if you swap the left
            // or right of a sub-tree pointer, the pointer will hold data that will still
            // keep the BST invariant when compared to the parent from which it came from.
            // In case of a root modification, we would still hold the invariant or the pointer
            // will refer to a nullptr.
            else if (!foundNode->left)
            {
                // Refer to note.
                parent->left = foundNode->right;
                delete foundNode;
                foundNode->left = nullptr;
                foundNode->right = nullptr;

                treeSize--;
            }
            else if (!foundNode->right)
            {
                parent->left = foundNode->left;
                delete foundNode;
                foundNode->left = nullptr;
                foundNode->right = nullptr;

                treeSize--;
            }
            // Case 3 (Two Children)
            else
            {
                // Two choice:
                // Choice One: Dig far right into the sub tree and swap the found node with the
                // the biggest node found in its subtree (will end up being a leaf node) and call
                // Remove.
                // Choice Two: Get the biggest smallest sub-node in the left tree (will end up being a leaf node),
                // and remove it.

                // Retreieves the parent of the biggest node in the left subtree of the node we are trying to remove.
                Node *biggestLeftNodeParent = retrieveFurthestLeftNodeDFS(foundNode->left);

                // Here we are ONLY swapping the data and will not perform pointer manipulation. We only care to remove
                // the foundNode element from existence while keeping the invariant in tact, therefore swapping foundNode's
                // data with a leaf node of the biggestLeftNodeParent's right element will allow for use to call a regular
                // removal function on that element later.
                T temp = biggestLeftNodeParent->right->data;
                biggestLeftNodeParent->right->data = foundNode->data;
                foundNode->data = temp;

                DFSRemoveHelper(biggestLeftNodeParent->right->data, root);
            }
        }
        // The right side contains the actuall element
        else if (parent->right && parent->right->data == element)
        {
            // Phase Two: Removal of node. (Bound by 3 cases).
            Node *foundNode = parent->right;
            // Case 1 (Leaf Node || No children)
            if (!foundNode->left || !foundNode->right)
            {
                DFSRemoveHelper(element, root)
                {
                    parent->left = nullptr;
                }
            }
            // Case 2 (Single Child)
            // Note: Each node should be regarded as a sub-tree and the parent node it
            // comes from as a root node itself. Therefore, even if you swap the left
            // or right of a sub-tree pointer, the pointer will hold data that will still
            // keep the BST invariant when compared to the parent from which it came from.
            // In case of a root modification, we would still hold the invariant or the pointer
            // will refer to a nullptr.
            else if (!foundNode->left)
            {
                // Refer to note.
                parent->left = foundNode->right;
                delete foundNode;
                foundNode->left = nullptr;
                foundNode->right = nullptr;

                treeSize--;
            }
            else if (!foundNode->right)
            {
                parent->left = foundNode->left;
                delete foundNode;
                foundNode->left = nullptr;
                foundNode->right = nullptr;

                treeSize--;
            }
            // Case 3 (Two Children)
            else
            {
                // Two choice:
                // Choice One: Dig far right into the sub tree and swap the found node with the
                // the biggest node found in its subtree (will end up being a leaf node) and call
                // Remove.
                // Choice Two: Get the biggest smallest sub-node in the left tree (will end up being a leaf node),
                // and remove it.

                // Retreieves the parent of the biggest node in the left subtree of the node we are trying to remove.
                Node *biggestLeftNodeParent = retrieveFurthestRightNodeDFS(foundNode->left);
                if (!biggestLeftNodeParent)
                {
                    throw new std::runtime_error("Error retrieving parent. Check implementation of parent retrieval.")
                }
                // Here we are ONLY swapping the data and will not perform pointer manipulation. We only care to remove
                // the foundNode element from existence while keeping the invariant in tact, therefore swapping foundNode's
                // data with a leaf node of the biggestLeftNodeParent's right element will allow for use to call a regular
                // removal function on that element later.
                T temp = biggestLeftNodeParent->right->data;
                biggestLeftNodeParent->right->data = foundNode->data;
                foundNode->data = temp;

                DFSRemoveHelper(biggestLeftNodeParent->right->data, root);
            }
        }
        else
        {
            throw new std::runtime_error("Error in removal: Neither child of parent was able to perform removal. Check implementation")
        }
    }
    // Refer to comments in DFS as the same concepts apply here but we will implement most work with a queue instead
    // instead of an inherited stack.
    else
    {
        Node *parent = retrieveParentNodeBFS(element, root);

        if (parent->left && parent->left->data == element)
        {
            Node *foundNode = parent->left;

            if (!foundNode->left || !foundNode->right)
            {
                BFSRemoveHelper(element, root);
                parent->left = nullptr;
                // delete foundNode;
                // foundNode = nullptr;
            }
            else if (!foundNode->left)
            {
                parent->left = foundNode->right;
                delete foundNode;
                foundNode->left = nullptr;
                foundNode->right = nullptr;

                treeSize--;
            }
            else if (!foundNode->right)
            {
                parent->left = foundNode->left;
                delete foundNode;
                foundNode->left = nullptr;
                foundNode->right = nullptr;

                treeSize--;
            }
            else
            {
                Node *biggestLeftNodeParent = retrieveFurthestRightNodeBFS(foundNode->left);
                if (!biggestLeftNodeParent)
                {
                    throw new std::runtime_error("Error retrieving parent. Check implementation of parent retrieval.")
                }
                T temp = biggestLeftNodeParent->right->data;
                biggestLeftNodeParent->right->data = foundNode->data;
                foundNode->data = temp;

                BFSRemoveHelper(biggestLeftNodeParent->right->data, root);
            }
        }
    }
}

template <typename T>
bool BinarySearchTree<T>::binarySearch(const T &src, std::string type)
{
    if (!src)
        return false;
    if (!root && src)
        return false;
    if (type != "DFS" || type != "BFS")
    {
        throw new std::runtime_error("Incorrect type passed to binarySearch. Choose DFS or BFS");
    }
    if (type == "DFS")
    {
        Node *prospect = binarySearchDFS(root, src);
        return prospect != nullptr;
    }
    Node *prospect = binarySearchBFS(root, src);
    return prospect != nullptr
}

template <typename T>
bool BinarySearchTree<T>::isBalanced()
{
    if (!root)
    {
        return true;
    }

    return calculateHeightOfTree(root) == 0;
}

template <typename T>
bool BinarySearchTree<T>::contains(const T &element)
{
    return binarySearch(element, "DFS");
}