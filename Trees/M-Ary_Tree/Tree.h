/**
 * @file Tree.h
 * @author Kofi Boateng
 * @brief
 * @version 0.1
 * @date 2022-10-27
 *
 *
 */

#pragma once
#include <iostream>  // for cout & cerr
#include <stdexcept> // for runtime_error
#include <ostream>   // for::ostream
#include <vector>    // used to hold pointers to new nodes.
#include <queue>

// This is the implementation of a M-ary tree. An M-ary tree is tree graph that has at least two child nodes
// and up to M child nodes, where M is the number of children each node can have. For example, A binary tree
// is an 2-ary tree, and a Trie or Prefix tree is a 26-ary tree where each node can have up to 26 children
// represented by characters in the alphabet.

template <typename T>
class Tree
{
public:
    class TreeNode
    {
    public:
        // The amount of children each node is able to contain. The default value is always 2.
        int order_;

        // A static array of Node pointers, or children of each node.
        std::vector<TreeNode *> values_;

        // a flag to keep track of pointers added to the array. If the flag is 0, then we have
        // reached a leaf node.
        int valueSize_;

        // Data held by the node.
        T data_;

        TreeNode() : order_(2), valueSize_(0) {}
        TreeNode(int &order) : order_(order), valueSize_(0) {}
        TreeNode(int &order, const T &value) : order_(order), valueSize_(0), data_(value) {}

        TreeNode(const TreeNode &other) : order_(other.order_), valueSize_(other.valueSize_), values_(other.values_), data_(other.data_) {}

        TreeNode &operator=(const TreeNode &other)
        {
            order_ = other.order_;
            valueSize_ = other.valueSize_;
            values_ = other.values_;
            data_ = other.data_;
            return *this;
        }

        ~TreeNode()
        {
            for (auto node : values_)
            {
                delete node;
            }
            values_.clear();
        }
    };

private:
    // Root of the tree.
    TreeNode *root;
    // Reflects how many elements are actually in the tree.
    int size_;
    // Actual order of the tree. (e.g 2-ary, 15-ary, etc).
    int order_;

    // ClearTree is used to recursively remove elements from the
    // the tree during deallocation via a Post-Order Traversal.
    void clearTree(TreeNode *node);

    // A helper function that will return a pointer to an element
    // in the tree or return false.
    TreeNode *searchHelperDFS(TreeNode *node, const T &element);

    // A helper function that will return a pointer to an element
    // in the tree or return false.
    TreeNode *searchHelperBFS(TreeNode *node, const T &element);

    // A helper function that is called if the element passed
    void removerHelper(const T &element, TreeNode *node);

    // Returns the calculated height of the tree. A leaf node will be calculated with -1 height,
    // and we will subtract the height of the right side of a parent node from the left side.
    int calculateHeightOfTree(TreeNode *node) const;

    // Prints the tree in order.
    void inorderTreeTraversalPrint(TreeNode *node);

    // Prints the tree pre order.
    void preorderTreeTraversalPrint(TreeNode *node);

    // Prints the tree post order.
    void postorderTreeTraversalPrint(TreeNode *node);

    // helper function that will create a node and insert it into
    // childrens area if possible.
    bool insertionHelperDFS(const T &element, TreeNode *node);

    // helper function that will create a node and insert it into
    // childrens area if possible.
    void insertionHelperBFS(const T &element, TreeNode *node);

    // helper function that will remove a node from the tree if possible.
    bool removalHelperDFS(const T &element, TreeNode *node);

    // helper function that will remove a node from the tree if possible.
    void removalHelperBFS(const T &element, TreeNode *node);

public:
    // Returns the pointer to the root of the tree.
    TreeNode *getRoot() { return root; }

    // Returns current size of tree.
    int size() { return size_; }

    // Returns a boolean whether the exists or not
    bool isEmpty() const { return !root; }

    // Returns a boolean if the element is already in the tree.
    // We will not, for intents and purpose, allow duplicate
    // values in the tree. This fucntion will check for that as
    // a by product.
    bool contains(const T &element, const std::string &type);

    // Inserts an element into the tree
    // Type references wheter you use DFS or BFS Helper function.
    void insert(const T &element, const std::string &type);

    // Removes an element from the tree
    // Type references wheter you use DFS or BFS Helper function.
    void remove(const T &element, const std::string &type);

    // Returns either a reference to the element or a nullptr.
    TreeNode &retrieve(const T &element) const;

    // Deletes all the elements from the tree
    void clear()
    {
        if (root)
            clearTree(root);

        if (size_ != 0)
        {
            throw new std::runtime_error("Error in clear: elements still exist on the heap... please check");
        }
    }

    // Checks for equality between two list.
    // Two list are equal if they have the same
    // length and same data at each position. O(n).
    bool equals(const Tree<T> &obj) const;
    bool operator==(const Tree<T> &obj) const { return equals(obj); }
    bool operator!=(const Tree<T> &obj) const { return !equals(obj); }

    // Takes in a request (In,Pre, or Post) and then prints the tree in that order.
    std::ostream &print(std::ostream &os, const std::string &type);

    // Default constructor. The default order will always be two.
    Tree() : root(nullptr), size_(0), order_(2) {}

    // Constructor to specifc what type of tree it will be.
    Tree(int order) : root(nullptr), size_(0), order_(order){};

    Tree<T> &operator=(const Tree<T> &other)
    {
        clear();
        std::queue<TreeNode *> queue;
        queue.push(other.getRoot());
        while (!queue.empty())
        {
            TreeNode *curr = queue.front();
            queue.pop();
            insert(curr->data_, "DFS");
            int size{};
            while (size < curr->valueSize_)
            {
                queue.push(curr->values_[size++]);
            }
        }
        return *this;
    }

    Tree(const Tree<T> &other) : Tree()
    {
        *this = other;
    }

    ~Tree() { clear(); }
};

// ==================================================================================
// Implementation section
// ==================================================================================

template <typename T>
void Tree<T>::inorderTreeTraversalPrint(TreeNode *node)
{
    if (!node)
    {
        return;
    }
    int midpoint = node->valueSize_ / 2;
    for (int i{}; i < midpoint; i++)
    {
        // "'LEFT' half recursion"
        inorderTreeTraversalPrint(node->values_[i]);
    }
    std::cout << node->data_ << "-";
    for (int i{midpoint}; i < node->valueSize_; i++)
    {
        // "'RIGHT' half recursion"
        inorderTreeTraversalPrint(node->values_[i]);
    }
}

template <typename T>
void Tree<T>::preorderTreeTraversalPrint(TreeNode *node)
{
    if (!node)
    {
        return;
    }
    std::cout << node->data_ << "-";
    int midpoint = node->valueSize_ / 2;
    for (int i{}; i < midpoint; i++)
    {
        // "'LEFT' half recursion"
        inorderTreeTraversalPrint(node->values_[i]);
    }
    for (int i{midpoint}; i < node->valueSize_; i++)
    {
        // "'RIGHT' half recursion"
        inorderTreeTraversalPrint(node->values_[i]);
    }
}

template <typename T>
void Tree<T>::postorderTreeTraversalPrint(TreeNode *node)
{
    if (!node)
    {
        return;
    }
    int midpoint = node->valueSize_ / 2;
    for (int i{}; i < midpoint; i++)
    {
        // "'LEFT' half recursion"
        inorderTreeTraversalPrint(node->values_[i]);
    }
    for (int i{midpoint}; i < node->valueSize_; i++)
    {
        // "'RIGHT' half recursion"
        inorderTreeTraversalPrint(node->values_[i]);
    }
    std::cout << node->data_ << "-";
}

template <typename T>
bool Tree<T>::insertionHelperDFS(const T &element, TreeNode *node)
{
    if (!node)
    {
        return false;
    }

    if (node->valueSize_ < node->order_)
    {
        TreeNode *newNode = new TreeNode(order_, element);
        node->values_.push_back(newNode);
        node->valueSize_++;
        return true;
    }

    for (int i{}; i < node->valueSize_; i++)
    {
        if (insertionHelperDFS(element, node->values_[i]))
        {
            return true;
        }
        else
        {
            break;
        }
    }
    return false;
}

template <typename T>
void Tree<T>::insertionHelperBFS(const T &element, TreeNode *root)
{
    std::queue<TreeNode *> queue;

    queue.push(root);

    while (!queue.empty())
    {
        TreeNode *curr = queue.front();
        queue.pop();
        if (curr->valueSize_ < curr->order_)
        {
            TreeNode *newNode = new TreeNode(order_, element);
            curr->values_.push_back(newNode);
            curr->valueSize_++;
            break;
        }

        for (int i{}; i < curr->valueSize_; i++)
        {
            queue.push(curr->values_[i]);
        }
    }
}

template <typename T>
bool Tree<T>::removalHelperDFS(const T &element, TreeNode *node)
{
    if (node->data_ == element)
    {
        return true;
    }

    for (int i{}; i < node->valueSize_; i++)
    {
        if (removalHelperDFS(element, node->values_[i]))
        {
            node->valueSize_--;
            delete node->values_[i];
            node->values_[i] = nullptr;
            break;
        }
    }
    return false;
}
template <typename T>
void Tree<T>::removalHelperBFS(const T &element, TreeNode *node)
{
    std::queue<TreeNode *> queue;

    queue.push(root);

    while (!queue.empty())
    {
        TreeNode *curr = queue.front();
        queue.pop();

        for (int i{}; i < curr->valueSize_; i++)
        {
            if (curr->values_[i]->data_ == element)
            {
                TreeNode *nodeToDelete = curr->values_[i];
                delete nodeToDelete;
                nodeToDelete = nullptr;
                curr->values_[i] = nullptr;
                break;
            }
            queue.push(curr->values_[i]);
        }
    }
}

template <typename T>
typename Tree<T>::TreeNode *Tree<T>::searchHelperDFS(TreeNode *node, const T &element)
{
    if (!node)
    {
        return nullptr;
    }

    if (node->data_ == element)
    {
        return node;
    }

    for (int i{}; i < node->valueSize_; i++)
    {
        TreeNode *value = searchHelperDFS(node->values_[i], element);
        if (value != nullptr)
        {
            return value;
        }
    }

    return nullptr;
}

template <typename T>
typename Tree<T>::TreeNode *Tree<T>::searchHelperBFS(TreeNode *node, const T &element)
{
    std::queue<TreeNode *> queue;
    queue.push(node);

    while (!queue.empty())
    {
        TreeNode *curr = queue.front();
        queue.pop();

        if (curr->data_ == element)
        {
            return curr;
        }

        for (int i{}; i < curr->valueSize_; i++)
        {
            queue.push(curr->values_[i]);
        }
    }

    return nullptr;
}

template <typename T>
void Tree<T>::clearTree(TreeNode *node)
{
    // We have reached a leaf and will delete node.
    if (node->valueSize_ <= 0)
    {
        delete node;
        node = nullptr;
        return;
    }

    for (int i{}; i < node->valueSize_; i++)
    {
        // iterate all the way until you reach the end of a list
        clearTree(node->values_[i]);
        // delete the node the base case has reached from the vector list.
        node->values_[i] = nullptr;
    }
}

// ==================================================================================================================
// Public Methods
// ==================================================================================================================

template <typename T>
void Tree<T>::insert(const T &element, const std::string &type)
{
    if (!root)
    {
        TreeNode *newRoot = new TreeNode(order_, element);
        root = newRoot;
        size_++;
    }
    else
    {
        if (type == "DFS" || type == "dfs")
        {
            insertionHelperDFS(element, getRoot());
            size_++;
        }
        else if (type == "BFS" || type == "BFS")
        {
            insertionHelperBFS(element, getRoot());
            size_++;
        }
        else
        {
            throw new std::runtime_error("[ERROR]: Please specficy between Depth-First (DFS|dfs) or Breadth-Firsth (BFS|bfs).");
        }
    }
}

template <typename T>
void Tree<T>::remove(const T &element, const std::string &type)
{
    if (!root)
    {
        return;
    }
    else
    {
        if (type == "DFS" || type == "dfs")
        {
            removalHelperDFS(element, getRoot());
            size_--;
        }
        else if (type == "BFS" || type == "BFS")
        {
            removalHelperBFS(element, getRoot());
            size_--;
        }
        else
        {
            throw new std::runtime_error("[ERROR]: Please specficy between Depth-First (DFS|dfs) or Breadth-Firsth (BFS|bfs).");
        }
    }
}

template <typename T>
bool Tree<T>::equals(const Tree<T> &other) const
{
    if (!root || size_ != other.size_)
    {
        return false;
    }

    std::queue<TreeNode *> queueThis;
    std::queue<TreeNode *> queueOther;
    queueThis.push(root);
    queueOther.push(other.root);

    while (!queueThis.empty())
    {
        if (queueOther.empty())
        {
            return false;
        }
        TreeNode *thisNode = queueThis.front();
        TreeNode *otherNode = queueOther.front();
        queueThis.pop();
        queueOther.pop();

        if (thisNode->data_ != otherNode->data_)
        {
            return false;
        }

        int sizeThis{};
        int sizeOther{};

        while (sizeThis < thisNode->valueSize_)
        {
            queueThis.push(thisNode->values_[sizeThis++]);
        }

        while (sizeOther < otherNode->valueSize_)
        {
            queueOther.push(otherNode->values_[sizeOther++]);
        }
    }

    return queueOther.empty() && queueThis.empty();
}

template <typename T>
typename Tree<T>::TreeNode &Tree<T>::retrieve(const T &element) const
{
    if (!root)
    {
        return nullptr;
    }

    return searchHelperDFS(element, root);
}

template <typename T>
bool Tree<T>::contains(const T &element, const std::string &type)
{
    if (type == "DFS" || type == "dfs")
    {
        return searchHelperDFS(getRoot(), element) != nullptr;
    }
    else if (type == "BFS" || type == "bfs")
    {
        return searchHelperBFS(getRoot(), element) != nullptr;
    }
    else
    {
        std::runtime_error("[ERROR]: Please specficy between Depth-First (DFS|dfs) or Breadth-Firsth (BFS|bfs).");
    }
    return false;
}

template <typename T>
std::ostream &Tree<T>::print(std::ostream &os, const std::string &type)
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