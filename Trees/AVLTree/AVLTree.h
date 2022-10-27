/**
 * @file AVLTree.h
 * @author Kofi Boateng
 * @version 0.1
 * @date 2022-10-23
 *
 *
 */

#pragma once
#include <iostream>  // for cout & cerr
#include <stdexcept> // for runtime_error
#include <ostream>   // for::ostream
#include <queue>     //queue used for BFS algorithms

// This is an implementation of a AVL Tree (Self-Balancing BST). a AVL Tree
// follows the same princples of a Binary Search Tree, however upon insertion
// and deletion, the tree will rebalance itself to keep the tree balanced.

template <typename T>
class AVLBinaryTree
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

        // The height of the tree. (Refer to calculateHeight function).
        int height;

        // The balance factor of a node. If the node is a leaf node, its nullptr
        // will constitute a -1 value. We will perform right - left calculations
        // resulting in leaf nodes having a height of 0. A perfectly balanced
        // tree will have a balance factor (BF) of 0. if the BF is BF > 0, then
        // the tree is left heavy, and vice versa.
        int balanceFactor;

        // Default constructor: This lets data be constructed by
        // the default constructor of the T type.
        Node() : left(nullptr), right(nullptr), height(0) {}

        // Argument constructor
        Node(const T &dataArg) : left(nullptr), right(nullptr), data(dataArg), height(0) {}

        // Copy constructor: Constructs a new node to be identical to the node being
        // copied.
        Node(const Node &other) : left(other.left), right(other.right), data(other.data), height(other.height) {}

        // Copy assignment operator
        Node &operator=(const Node &other)
        {
            left = other.left;
            right = other.right;
            data = other.data;
            height = other.height;
            return *this;
        }

        // Destructor: Although we don't seem to write anything here,
        // the members of the node class will have their own destructors
        // automatically called afterward.
        ~Node() {}
    };

    // ====================================================================================
    // Private variables and methods
    // ====================================================================================

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

    // Calculates the height of the tree recursively. A leaf node will be calculated with -1 height,
    // and we will subtract the height of the right side of a parent node from the left side.
    int calculateHeightOfTree(Node *node) const;

    // Updates the height of a node
    void updateHeight(Node *node);

    // Performs a right rotation on the parent node. Right rotations are classified
    // as rotating a stick formation in a left subtree. A stick formation will
    // generate a +2 height when doing height calculations in the left subtree.
    // This rotation will rotate the stick formation in a clock-wise formation
    // to for a mountain, or a balanced subtree.
    Node *rightRotation(Node *node);

    // Performs a left rotation on the parent node. Left rotations are classified
    // as rotating a stick formation in a right subtree. A stick formation will
    // generate a -2 height when doing height calculations in the right subtree.
    // This rotation will rotate the stick formation in a clock-wise formation
    // to for a mountain, or a balanced subtree.
    Node *leftRotation(Node *node);

    // A function that indenties the formation of an elbow in a right subtree.
    // This function performs a right rotation first on the leaf node and its
    // parent to form a stick formation, and then a left rotation on the deep
    // -est identifying node (where heigh == -2) and its child. An elbow in the
    // right subtree will produce a height of -2 on the deepest identifer and its
    // child will produce a height of +1 meaning there is only a left node available.
    void rightLeftRotation(Node *node);

    // A function that indenties the formation of an elbow in a left subtree.
    // This function performs a left rotation first on the leaf node and its
    // parent to form a stick formation, and then a right rotation on the deep
    // -est identifying node (where heigh == +2) and its child. An elbow in the
    // left subtree will produce a height of +2 on the deepest identifer and its
    // child will produce a height of -1 meaning there is only a right node available.
    void leftRightRotation(Node *node);

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

    // A function that will determine if the tree is balanced or not.
    bool isBalanced() const;

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

    // Checks if the tree is balanced, and if not performs a specific
    // balancing algorithms based on the node's height.
    void checkBalance(Node *node);

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
    bool equals(const AVLBinaryTree<T> &obj) const;
    bool operator==(const AVLBinaryTree<T> &obj) const { return equals(obj); }
    bool operator!=(const AVLBinaryTree<T> &obj) const { return !equals(obj); }

    // Takes in a request (In,Pre, or Post) and then prints the tree in that order.
    std::ostream &print(std::ostream &os, const std::string &type);

    // Default Constructor: creates an empty tree
    AVLBinaryTree() : root(nullptr), treeSize(0) {}

    // We will run a BFS algorithm to copy nodes at each level
    AVLBinaryTree<T> &operator=(const AVLBinaryTree<T> &other)
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
    AVLBinaryTree(const AVLBinaryTree<T> &other) : AVLBinaryTree()
    {
        *this = other;
    }

    // Deconstructor of the tree;
    ~AVLBinaryTree()
    {
        clear();
    }
};

// =======================================================================================
// Implementation Section
// =======================================================================================

// =========================================================
// Private Helper Functions
// =========================================================

template <typename T>
typename AVLBinaryTree<T>::Node *AVLBinaryTree<T>::retrieveNodeDFS(const T &element, Node *node)
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
typename AVLBinaryTree<T>::Node *AVLBinaryTree<T>::retrieveNodeBFS(const T &element, Node *node)
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
typename AVLBinaryTree<T>::Node *AVLBinaryTree<T>::binarySearchDFS(Node *node, const T &src)
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
typename AVLBinaryTree<T>::Node *AVLBinaryTree<T>::binarySearchBFS(Node *node, const T &src)
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
void AVLBinaryTree<T>::inorderTreeTraversalPrint(Node *node)
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
void AVLBinaryTree<T>::preorderTreeTraversalPrint(Node *node)
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
void AVLBinaryTree<T>::postorderTreeTraversalPrint(Node *node)
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
typename AVLBinaryTree<T>::Node *AVLBinaryTree<T>::retrieveFurthestRightNodeDFS(Node *node)
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
typename AVLBinaryTree<T>::Node *AVLBinaryTree<T>::retrieveFurthestLeftNodeDFS(Node *node)
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
typename AVLBinaryTree<T>::Node *AVLBinaryTree<T>::retrieveFurthestRightNodeBFS(Node *node)
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
typename AVLBinaryTree<T>::Node *AVLBinaryTree<T>::retrieveFurthestLeftNodeBFS(Node *node)
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
typename AVLBinaryTree<T>::Node *AVLBinaryTree<T>::DFSInsertHelper(const T &element, Node *node)
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
void AVLBinaryTree<T>::BFSInsertHelper(const T &element, Node *node)
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
void AVLBinaryTree<T>::DFSRemoveHelper(const T &element, Node *node)
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
void AVLBinaryTree<T>::BFSRemoveHelper(const T &element, Node *node)
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
int AVLBinaryTree<T>::calculateHeightOfTree(Node *node) const
{
    return !node ? -1 : std::max(calculateHeightOfTree(node->right), calculateHeightOfTree(node->left)) + 1;
}

template <typename T>
void AVLBinaryTree<T>::updateHeight(Node *node)
{
    int left = !node->left ? -1 : node->left->height;
    int right = !node->right ? -1 : node->left->height;
    node->height = std::max(left, right) + 1;
    node->balanceFactor = right - left;
}

// ===============================
// Height balancing algorithms
// ===============================

template <typename T>
void AVLBinaryTree<T>::leftRightRotation(Node *node)
{
    node->left = leftRotation(node->left);
    Node *temp = rightRotation(node);
}

template <typename T>
void AVLBinaryTree<T>::rightLeftRotation(Node *node)
{
    node->right = leftRotation(node->right);
    Node *temp = rightRotation(node);
}

template <typename T>
typename AVLBinaryTree<T>::Node *AVLBinaryTree<T>::leftRotation(Node *node)
{
    Node *newParent = node->right;
    node->right = newParent->left;
    newParent->left = node;
    updateHeight(node);
    updateHeight(newParent);
    return newParent;
}

template <typename T>
typename AVLBinaryTree<T>::Node *AVLBinaryTree<T>::rightRotation(Node *node)
{
    Node *newParent = node->left;
    node->left = newParent->right;
    newParent->right = node;
    updateHeight(node);
    updateHeight(newParent);
    return newParent;
}

template <typename T>
void AVLBinaryTree<T>::checkBalance(Node *node)
{
    // Calculates the current node's height
    updateHeight(node);

    // If the balance factor == +2, then we know the tree is not balanced and is left heavy.
    if (node->balanceFactor == -2)
    {
        // Check for specfic rotation type by grabbing the balance factor of left child
        // Case One: Left-Left || Stick Formation
        if (node->left->balanceFactor < 0)
        {
            leftRotation(node);
        }
        // Greater than 0 means we have a elbow and perform
        // Case Two: Right-Left || Elbow Formation
        else
        {
            rightLeftRotation(node);
        }
    }
    // If the balance factor is == +2, then we know the tree is not balanced and is right heavy.
    else if (node->balanceFactor == 2)
    {
        // Check for specfic rotation type by grabbing the balance factor of right child
        // Case One: Right-Right || Stick Formation
        if (node->left->balanceFactor > 0)
        {
            rightRotation(node);
        }
        // Greater than 0 means we have a elbow and perform
        // Case Two: Left-Right || Elbow Formation
        else
        {
            leftRightRotation(node);
        }
    }
}

template <typename T>
void AVLBinaryTree<T>::rootRemover(const T &element, Node *node, const std::string &type)
{
    if (type == "DFS" || type == "dfs")
    {
        if (!node->left)
        {
            Node *foundNode = retrieveFurthestLeftNodeDFS(node->right);
            T temp = foundNode->data;
            foundNode->data = node->data;
            node->data = temp;

            DFSRemoveHelper(foundNode->data, node);
        }
        else if (!node->right)
        {
            Node *foundNode = retrieveFurthestRightNodeDFS(node->left);
            T temp = foundNode->data;
            foundNode->data = node->data;
            node->data = temp;

            DFSRemoveHelper(foundNode->data, node);
        }
        else
        {
            Node *foundNode = retrieveFurthestRightNodeDFS(node->left);
            std::cout << "Node to remove: " << node->data << std::endl;
            std::cout << "Founded node: " << foundNode->data << std::endl;
            T temp = foundNode->data;
            foundNode->data = node->data;
            node->data = temp;
            DFSRemoveHelper(element, root);
        }
    }
    // Refer to comments in DFS as the same concepts apply here but we will implement most work with a queue instead
    // instead of an inherited stack.
    else if (type == "BFS" || type == "bfs")
    {
        if (!node->left)
        {
            Node *foundNode = retrieveFurthestLeftNodeBFS(node->right);
            T temp = foundNode->data;
            foundNode->data = node->data;
            node->data = temp;

            BFSRemoveHelper(foundNode->data, node);
        }
        else if (!node->right)
        {
            Node *foundNode = retrieveFurthestRightNodeBFS(node->left);
            T temp = foundNode->data;
            foundNode->data = node->data;
            node->data = temp;

            BFSRemoveHelper(foundNode->data, node);
        }
        else
        {
            Node *foundNode = retrieveFurthestRightNodeBFS(node->left);
            std::cout << "Node to remove: " << node->data << std::endl;
            std::cout << "Founded node: " << foundNode->data << std::endl;
            T temp = foundNode->data;
            foundNode->data = node->data;
            node->data = temp;
            print(std::cout, "pre");
            BFSRemoveHelper(element, root);
            std::cout << "HERE" << std::endl;
        }
    }
    else
    {
        std::cout << "MADE IT" << std::endl;
        throw new std::runtime_error("Error in remove: incorrect type offered. Choose between DFS and BFS");
    }
}

template <typename T>
void AVLBinaryTree<T>::removeHelper(const T &element, Node *root, const std::string &type)
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

            DFSRemoveHelper(biggestRightNode->data, root);
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

            DFSRemoveHelper(biggestLeftNode->data, root);
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

            DFSRemoveHelper(biggestLeftNode->data, root);
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

            BFSRemoveHelper(biggestLeftNode->data, root);
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

            BFSRemoveHelper(biggestRightNode->data, root);
        }
        else
        {
            Node *biggestRightNode = retrieveFurthestLeftNodeBFS(foundNode->right);
            if (!biggestRightNode)
            {
                throw new std::runtime_error("Error retrieving parent. Check implementation of parent retrieval.");
            }
            T temp = biggestRightNode->data;
            biggestRightNode->data = foundNode->data;
            foundNode->data = temp;

            BFSRemoveHelper(biggestRightNode->data, root);
        }
    }
    else
    {
        std::cout << "MADE IT" << std::endl;
        throw new std::runtime_error("Error in remove: incorrect type offered. Choose between DFS and BFS");
    }
}

// =========================================================
// Public Methods
// =========================================================
template <typename T>
void AVLBinaryTree<T>::insert(const T &arg, std::string type)
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
void AVLBinaryTree<T>::remove(const T &element, std::string type)
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
bool AVLBinaryTree<T>::binarySearch(const T &src, std::string type)
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
bool AVLBinaryTree<T>::isBalanced() const
{
    if (!root)
    {
        return true;
    }

    return calculateHeightOfTree(root) == 0;
}

template <typename T>
bool AVLBinaryTree<T>::contains(const T &element)
{
    return binarySearch(element, "DFS");
}

template <typename T>
std::ostream &AVLBinaryTree<T>::print(std::ostream &os, const std::string &type)
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