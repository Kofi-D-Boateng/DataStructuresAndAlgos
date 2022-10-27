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
            return *this;
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
    Node *binarySearchDFS(Node *node, const T &src);

    // BFS Binary search to be called with binarySearch
    Node *binarySearchBFS(Node *node, const T &src);

    // DFS helper for insertions
    Node *DFSInsertHelper(const T &element, Node *node);
    // BFS helper for insertions
    void BFSInsertHelper(const T &element, Node *node);

    // DFS helper for removals
    void DFSRemoveHelper(const T &element, Node *node);

    // BFS helper for removals
    void BFSRemoveHelper(const T &element, Node *node);

    // A helper function that is called if the element passed
    // is not the root
    void removeHelper(const T &element, Node *root, const std::string &type);

    // A helper function that is called if the element passed
    // is the root
    void rootRemover(const T &element, Node *root, const std::string &type);

    // Retrieves the pointer to parent node of the node we are trying to remove.
    // We want to retrieve this node instead of the actual node itself in order to
    // manually update pointers correctly.
    Node *retrieveNodeDFS(const T &element, Node *node);

    // Refer to above comment in DFS version.
    Node *retrieveNodeBFS(const T &element, Node *node);

    // A helper function that will retrieve the farthest node right
    // in the left subtree where the Ancestor node that calls the function will be swapped
    // with the child of the parent node.
    Node *retrieveFurthestRightNodeDFS(Node *node);

    // Follows same principles as above function but finds the smallest element in the right
    // subtree of a node by taking a step right and digging as far left as possible. This will
    // still result in an element bigger than the node needing to be removed.
    Node *retrieveFurthestLeftNodeDFS(Node *node);

    // Refer to notes in the DFS version.
    Node *retrieveFurthestRightNodeBFS(Node *node);

    // Refer to notes in the DFS version.
    Node *retrieveFurthestLeftNodeBFS(Node *node);

    // Returns the calculated height of the tree. A leaf node will be calculated with -1 height,
    // and we will subtract the height of the right side of a parent node from the left side.
    int
    calculateHeightOfTree(Node *node) const;

    // Prints the tree in order.
    void inorderTreeTraversalPrint(Node *node);

    // Prints the tree pre order.
    void preorderTreeTraversalPrint(Node *node);

    // Prints the tree post order.
    void postorderTreeTraversalPrint(Node *node);

public:
    // Gets the root of the tree
    Node *getRoot()
    {
        return root;
    }

    // Gets the amount of elements currently in the tree
    int size() const { return treeSize; }

    // Returns a boolean whether the exists or not
    bool isEmpty() const { return !root; }

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
    bool equals(const BinarySearchTree<T> &obj) const;
    bool operator==(const BinarySearchTree<T> &obj) const { return equals(obj); }
    bool operator!=(const BinarySearchTree<T> &obj) const { return !equals(obj); }

    // Takes in a request (In,Pre, or Post) and then prints the tree in that order.
    std::ostream &print(std::ostream &os, const std::string &type);

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
                insert(currNode->data, "BFS");
                if (currNode->left)
                    queue.back(currNode->left.data);
                if (currNode->right)
                    queue.back(currNode->right.data);
            }
        }

        return *this;
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

template <typename T>
void BinarySearchTree<T>::inorderTreeTraversalPrint(Node *node)
{
    if (!node)
    {
        return;
    }

    inorderTreeTraversalPrint(node->left);
    std::cout << node->data << "-";
    inorderTreeTraversalPrint(node->right);
}

template <typename T>
void BinarySearchTree<T>::preorderTreeTraversalPrint(Node *node)
{
    if (!node)
    {
        return;
    }
    std::cout << node->data << "-";
    inorderTreeTraversalPrint(node->left);
    inorderTreeTraversalPrint(node->right);
}

template <typename T>
void BinarySearchTree<T>::postorderTreeTraversalPrint(Node *node)
{
    if (!node)
    {
        return;
    }
    inorderTreeTraversalPrint(node->left);
    inorderTreeTraversalPrint(node->right);
    std::cout << node->data << "-";
}

template <typename T>
typename BinarySearchTree<T>::Node *BinarySearchTree<T>::retrieveNodeDFS(const T &element, Node *node)
{
    if (!node)
    {
        return nullptr;
    }

    if (node->data == element)
    {
        return node;
    }

    Node *value = nullptr;

    if (element > node->data)
    {
        value = retrieveNodeDFS(element, node->right);
    }
    else
    {
        value = retrieveNodeDFS(element, node->left);
    }

    return value;
}

template <typename T>
typename BinarySearchTree<T>::Node *BinarySearchTree<T>::retrieveNodeBFS(const T &element, Node *node)
{
    if (!node || !element)
    {
        return nullptr;
    }

    std::queue<Node *> queue;
    queue.push(node);
    Node *value = nullptr;
    while (!queue.empty())
    {
        Node *temp = queue.front();
        queue.pop();
        if (temp->data == element)
        {
            value = temp;
            break;
        }
        else if (element > temp->data && temp->right)
        {
            queue.push(temp->right);
        }
        else if (element < temp->data && temp->left)
        {
            queue.push(temp->left);
        }
    }

    return value;
}

template <typename T>
typename BinarySearchTree<T>::Node *BinarySearchTree<T>::retrieveFurthestRightNodeDFS(Node *node)
{
    if (!node->right && !node->left)
    {
        return node;
    }

    Node *value = nullptr;
    if (!node->right && node->left)
    {
        value = retrieveFurthestRightNodeDFS(node->left);
    }
    else
    {
        value = retrieveFurthestRightNodeDFS(node->right);
    }
    return value;
}

template <typename T>
typename BinarySearchTree<T>::Node *BinarySearchTree<T>::retrieveFurthestLeftNodeDFS(Node *node)
{
    if (!node->left && !node->right)
    {
        return node;
    }

    Node *value = nullptr;
    if (!node->left && node->right)
    {
        value = retrieveFurthestLeftNodeDFS(node->right);
    }
    else
    {
        value = retrieveFurthestLeftNodeDFS(node->left);
    }
    return value;
}

template <typename T>
typename BinarySearchTree<T>::Node *BinarySearchTree<T>::retrieveFurthestRightNodeBFS(Node *node)
{
    std::queue<Node *> queue;
    Node *value = nullptr;
    queue.push(node);

    while (!queue.empty())
    {
        Node *currNode = queue.front();
        queue.pop();
        if (!currNode->left && !currNode->right)
        {
            value = currNode;
            break;
        }
        else
        {
            if (!currNode->right && currNode->left)
            {
                queue.push(currNode->left);
            }
            else
            {
                queue.push(currNode->right);
            }
        }
    }

    return value;
}

template <typename T>
typename BinarySearchTree<T>::Node *BinarySearchTree<T>::retrieveFurthestLeftNodeBFS(Node *node)
{
    std::queue<Node *> queue;
    Node *value = nullptr;
    queue.push(node);

    while (!queue.empty())
    {
        Node *currNode = queue.front();
        queue.pop();
        if (!currNode->left && !currNode->right)
        {
            value = currNode;
            break;
        }
        else
        {
            if (!currNode->left && currNode->right)
            {
                queue.push(currNode->right);
            }
            else
            {
                queue.push(currNode->left);
            }
        }
    }

    return value;
}

// Helper function for Insert
template <typename T>
typename BinarySearchTree<T>::Node *BinarySearchTree<T>::DFSInsertHelper(const T &element, Node *node)
{
    if (!node)
    {
        treeSize++;
        Node *newNode = new Node(element);
        return newNode;
    }
    if (element > node->data)
    {
        node->right = DFSInsertHelper(element, node->right);
    }
    else
    {
        node->left = DFSInsertHelper(element, node->left);
    }

    return node;
}

// Helper function for Insert
template <typename T>
void BinarySearchTree<T>::BFSInsertHelper(const T &element, Node *node)
{
    std::queue<Node *> queue;
    queue.push(node);

    while (!queue.empty())
    {
        Node *currNode = queue.front();
        queue.pop();
        if (element < currNode->data && currNode->left)
        {
            queue.push(currNode->left);
        }
        else if (element > currNode->data && currNode->right)
        {
            queue.push(currNode->right);
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
    if (!node)
    {
        return;
    }

    DFSRemoveHelper(element, node->left);
    DFSRemoveHelper(element, node->right);
    if (node->left && node->left->data == element)
    {
        treeSize--;
        delete node->left;
        node->left->left = nullptr;
        node->left->right = nullptr;
        node->left = nullptr;
        return;
    }
    else if (node->right && node->right->data == element)
    {
        treeSize--;
        delete node->right;
        node->right->left = nullptr;
        node->right->right = nullptr;
        node->right = nullptr;
        return;
    }
}

template <typename T>
void BinarySearchTree<T>::BFSRemoveHelper(const T &element, Node *node)
{
    if (!node)
    {
        return;
    }

    std::queue<Node *> queue;
    queue.push(node);

    while (!queue.empty())
    {
        Node *temp = queue.front();
        queue.pop();
        // Check for parent of the element;
        if (temp->right)
        {
            if (temp->right->data != element)
            {
                queue.push(temp->right);
            }
            else
            {
                delete temp->right;
                temp->right = nullptr;
                treeSize--;
                break;
            }
        }
        if (temp->left)
        {
            if (temp->left->data != element)
            {
                queue.push(temp->left);
            }
            else
            {
                delete temp->left;
                temp->left = nullptr;
                treeSize--;
                break;
            }
        }
    }
}

template <typename T>
int BinarySearchTree<T>::calculateHeightOfTree(Node *node) const
{
    return !node ? -1 : calculateHeightOfTree(node->right) - calculateHeightOfTree(node->left);
}

// DFS search algorithm that returns a pointer to a node on the heap.
template <typename T>
typename BinarySearchTree<T>::Node *BinarySearchTree<T>::binarySearchDFS(Node *node, const T &src)
{
    if (!node)
    {
        return nullptr;
    }

    if (node->data == src)
    {
        return node;
    }

    Node *value = nullptr;

    if (src > node->data)
    {
        value = binarySearchDFS(node->right, src);
    }
    else
    {
        value = binarySearchDFS(node->left, src);
    }

    return value;
}

// BFS search algorithm that returns a pointer to a node on the heap.
template <typename T>
typename BinarySearchTree<T>::Node *BinarySearchTree<T>::binarySearchBFS(Node *node, const T &src)
{
    std::queue<Node *> queue;
    queue.push(node);

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
            queue.push(currNode->right);
        }
        else if (src < currNode->data && currNode->left)
        {
            queue.push(currNode->left);
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
void BinarySearchTree<T>::removeHelper(const T &element, Node *root, const std::string &type)
{
    if (type == "DFS" || type == "dfs")
    {
        // Phase One: Find the parent node of the node designated for removal;
        Node *foundNode = retrieveNodeDFS(element, root);
        // Phase Two: Check which side we can swap the data with.

        if (!foundNode->left && !foundNode->right)
        {
            // We have found a leaf node
            DFSRemoveHelper(element, root);
        }

        else if (!foundNode->left)
        {
            Node *biggestRightNode = retrieveFurthestLeftNodeDFS(foundNode->right);
            if (!biggestRightNode)
            {
                throw new std::runtime_error("Error retrieving parent. Check implementation of parent retrieval.");
            }
            // Here we are ONLY swapping the data and will not perform pointer manipulation. We only care to remove
            // the foundNode element from existence while keeping the invariant in tact, therefore swapping foundNode's
            // data with a leaf node of the biggestLeftNodeParent's right element will allow for use to call a regular
            // removal function on that element later.
            T temp = biggestRightNode->data;
            biggestRightNode->data = foundNode->data;
            foundNode->data = temp;

            DFSRemoveHelper(element, root);
        }
        // The right side contains the actuall element
        else if (!foundNode->right)
        {

            Node *biggestLeftNode = retrieveFurthestRightNodeDFS(foundNode->left);

            if (!biggestLeftNode)
            {
                throw new std::runtime_error("Error retrieving parent. Check implementation of parent retrieval.");
            }
            // Here we are ONLY swapping the data and will not perform pointer manipulation. We only care to remove
            // the foundNode element from existence while keeping the invariant in tact, therefore swapping foundNode's
            // data with a leaf node of the biggestLeftNodeParent's right element will allow for use to call a regular
            // removal function on that element later.
            T temp = biggestLeftNode->data;
            biggestLeftNode->data = foundNode->data;
            foundNode->data = temp;

            DFSRemoveHelper(element, root);
        }
        // Node has two children swap so you can swap from any side.
        else
        {
            Node *biggestLeftNode = retrieveFurthestLeftNodeDFS(foundNode->right);

            if (!biggestLeftNode)
            {
                throw new std::runtime_error("Error retrieving parent. Check implementation of parent retrieval.");
            }
            // Here we are ONLY swapping the data and will not perform pointer manipulation. We only care to remove
            // the foundNode element from existence while keeping the invariant in tact, therefore swapping foundNode's
            // data with a leaf node of the biggestLeftNodeParent's right element will allow for use to call a regular
            // removal function on that element later.
            T temp = biggestLeftNode->data;
            biggestLeftNode->data = foundNode->data;
            foundNode->data = temp;

            DFSRemoveHelper(element, root);
        }
    }
    // Refer to comments in DFS as the same concepts apply here but we will implement most work with a queue instead
    // instead of an inherited stack.
    else if (type == "BFS" || type == "bfs")
    {
        Node *foundNode = retrieveNodeBFS(element, root);

        if (!foundNode->left && !foundNode->right)
        {
            BFSRemoveHelper(element, root);
        }
        else if (!foundNode->right)
        {
            Node *biggestLeftNode = retrieveFurthestRightNodeBFS(foundNode->left);

            if (!biggestLeftNode)
            {
                throw new std::runtime_error("Error retrieving parent. Check implementation of parent retrieval.");
            }
            T temp = biggestLeftNode->data;
            biggestLeftNode->data = foundNode->data;
            foundNode->data = temp;

            BFSRemoveHelper(element, root);
        }
        else if (!foundNode->left)
        {
            Node *biggestRightNode = retrieveFurthestLeftNodeBFS(foundNode->right);
            if (!biggestRightNode)
            {
                throw new std::runtime_error("Error retrieving parent. Check implementation of parent retrieval.");
            }
            T temp = biggestRightNode->data;
            biggestRightNode->data = foundNode->data;
            foundNode->data = temp;

            BFSRemoveHelper(element, root);
        }
        else
        {
            Node *biggestRightNode = retrieveFurthestLeftNodeBFS(foundNode->right);
            std::cout << "Node to remove: " << foundNode->data << std::endl;
            std::cout << "Founded node: " << biggestRightNode->data << std::endl;
            if (!biggestRightNode)
            {
                throw new std::runtime_error("Error retrieving parent. Check implementation of parent retrieval.");
            }
            T temp = biggestRightNode->data;
            biggestRightNode->data = foundNode->data;
            foundNode->data = temp;
            print(std::cout, "in");
            BFSRemoveHelper(element, root);
        }
    }
    else
    {
        std::cout << "MADE IT" << std::endl;
        throw new std::runtime_error("Error in remove: incorrect type offered. Choose between DFS and BFS");
    }
}

// ===================================================================================================
// Public Methods
// ===================================================================================================

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
            if (arg > root->data)
            {
                root->right = DFSInsertHelper(arg, root->right);
            }
            else
            {
                root->left = DFSInsertHelper(arg, root->left);
            }
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
    if (!root)
    {
        return;
    }

    else if (treeSize == 1)
    {
        delete root;
        treeSize--;
    }

    else
    {
        removeHelper(element, root, type);
    }
}

template <typename T>
bool BinarySearchTree<T>::binarySearch(const T &src, std::string type)
{
    if (!src || !root)
    {
        return false;
    }

    if (type == "DFS")
    {
        Node *prospect = binarySearchDFS(root, src);
        return prospect != nullptr;
    }

    Node *prospect = binarySearchBFS(root, src);
    return prospect != nullptr;
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

template <typename T>
std::ostream &BinarySearchTree<T>::print(std::ostream &os, const std::string &type)
{
    // List format will be [1-2-3], etc.
    if (!root)
    {
        return os;
    }
    os << "[";
    if (type == "Pre" || type == "pre")
    {
        preorderTreeTraversalPrint(root);
    }
    else if (type == "Post" || type == "post")
    {
        postorderTreeTraversalPrint(root);
    }
    else if (type == "In" || type == "in")
    {
        inorderTreeTraversalPrint(root);
    }
    else
    {
        std::cout << "[ERROR]: Could not identify type. Please check print method to see which types are accepted.";
    }

    os << "]\n";

    return os;
}