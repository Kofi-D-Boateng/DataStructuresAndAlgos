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
#include <queue>     // used for BFS algorithms.
#include <stack>     // used for clearing the tree.

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

        // Data held by the node.
        T data_;

        TreeNode() : order_(2) {}
        TreeNode(int &order) : order_(order) {}
        TreeNode(int &order, const T &value) : order_(order), data_(value) {}

        TreeNode(const TreeNode &other) : order_(other.order_), values_(other.values_), data_(other.data_) {}

        TreeNode &operator=(const TreeNode &other)
        {
            order_ = other.order_;
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

    // Finds a node that can replace the node being removed. This node will most likely be a leaf node
    // in the subtree of the node being removed so that we can put the contents of the removed node in
    // the leaf node.
    TreeNode *findReplacerDFS(TreeNode *node);

    // helper function that will create a node and insert it into
    // childrens area if possible.
    bool insertionHelperDFS(const T &element, TreeNode *node);

    // helper function that will remove a node from the tree if possible.
    bool removalHelperDFS(const T &element, TreeNode *node);

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
    bool contains(const T &element);
    // Inserts an element into the tree
    // Type references wheter you use DFS or BFS Helper function.
    void insert(const T &element);

    // Removes an element from the tree
    // Type references wheter you use DFS or BFS Helper function.
    void remove(const T &element);

    // Returns either a reference to the element or a nullptr.
    TreeNode &retrieve(const T &element) const;

    // Deletes all the elements from the tree
    void clear()
    {
        if (root)
            clearTree(root);

        std::cout << "CURRENT SIZE: " << size_ << std::endl;
        print(std::cout, "pre");
        if (size_ != 0)
        {
            std::cout << "FOUND IT" << std::endl;
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
            while (size < curr->values_.size())
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
    int midpoint = node->values_.size() / 2;
    for (int i{}; i < midpoint; i++)
    {
        // "'LEFT' half recursion"
        inorderTreeTraversalPrint(node->values_[i]);
    }
    std::cout << node->data_ << "-";
    for (int i{midpoint}; i < node->values_.size(); i++)
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
    int midpoint = node->values_.size() / 2;
    for (int i{}; i < midpoint; i++)
    {
        // "'LEFT' half recursion"
        inorderTreeTraversalPrint(node->values_[i]);
    }
    for (int i{midpoint}; i < node->values_.size(); i++)
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
    int midpoint = node->values_.size() / 2;
    for (int i{}; i < midpoint; i++)
    {
        // "'LEFT' half recursion"
        inorderTreeTraversalPrint(node->values_[i]);
    }
    for (int i{midpoint}; i < node->values_.size(); i++)
    {
        // "'RIGHT' half recursion"
        inorderTreeTraversalPrint(node->values_[i]);
    }
    std::cout << node->data_ << "-";
}

template <typename T>
typename Tree<T>::TreeNode *Tree<T>::findReplacerDFS(TreeNode *node)
{
    if (!node)
    {
        return nullptr;
    }

    if (node->values_.size() == 0)
    {
        return node;
    }

    TreeNode *value = nullptr;
    for (TreeNode *n : node->values_)
    {
        value = findReplacerDFS(n);
        if (value)
        {
            break;
        }
    }

    return value;
}

template <typename T>
bool Tree<T>::insertionHelperDFS(const T &element, TreeNode *node)
{
    if (!node)
    {
        return false;
    }

    if (node->values_.size() < node->order_)
    {
        TreeNode *newNode = new TreeNode(order_, element);
        node->values_.push_back(newNode);
        return true;
    }

    for (int i{}; i < node->values_.size(); i++)
    {
        if (insertionHelperDFS(element, node->values_[i]))
        {
            break;
        }
    }
    return false;
}

template <typename T>
bool Tree<T>::removalHelperDFS(const T &element, TreeNode *node)
{
    if (node->data_ == element)
    {
        // If null, then it is a leaf node and we can do a regular removal;
        if (node->values_.size() <= 0)
        {
            delete node;
            return true;
        }
        else
        {
            std::cout << "START OF REPLACEMENT FIND" << std::endl;
            int midpoint = node->values_.size() / 2;
            TreeNode *replacementNode = findReplacerDFS(node->values_[midpoint]);
            std::cout << "REPLACEMENT DFS: " << replacementNode->data_ << std::endl;
            // Swap element with the leaf node and call remove on element again.
            T temp = node->data_;
            node->data_ = replacementNode->data_;
            replacementNode->data_ = temp;
            return removalHelperDFS(element, root);
        }
    }

    for (int i{}; i < node->values_.size(); i++)
    {
        if (removalHelperDFS(element, node->values_[i]))
        {
            auto location = node->values_.begin() + i;
            node->values_.erase(location);
            break;
        }
    }
    return false;
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

        for (int i{}; i < curr->values_.size(); i++)
        {
            queue.push(curr->values_[i]);
        }
    }

    return nullptr;
}

template <typename T>
void Tree<T>::clearTree(TreeNode *node)
{
    std::stack<TreeNode *> stack;
    std::queue<TreeNode *> queue;
    queue.push(node);
    while (!queue.empty())
    {
        TreeNode *curr = queue.front();
        queue.pop();
        stack.push(curr);
        for (TreeNode *n : curr->values_)
        {
            queue.push(n);
        }
    }

    while (!stack.empty())
    {
        TreeNode *nodeToDelete = stack.top();
        stack.pop();

        // We can safely call delete on all nodes because we are going
        // bottom up on deletes, therefore all previous nodes will
        // already be deleted.
        delete nodeToDelete;
        size_--;
    }
}

// ==================================================================================================================
// Public Methods
// ==================================================================================================================

template <typename T>
void Tree<T>::insert(const T &element)
{
    if (!root)
    {
        TreeNode *newRoot = new TreeNode(order_, element);
        root = newRoot;
        size_++;
    }
    else
    {
        insertionHelperDFS(element, getRoot());
        size_++;
    }
}

template <typename T>
void Tree<T>::remove(const T &element)
{
    if (!root)
    {
        return;
    }
    else
    {
        removalHelperDFS(element, getRoot());
        size_--;
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

        while (sizeThis < thisNode->values_.size())
        {
            queueThis.push(thisNode->values_[sizeThis++]);
        }

        while (sizeOther < otherNode->values_.size())
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

    return searchHelperBFS(element, root);
}

template <typename T>
bool Tree<T>::contains(const T &element)
{
    return searchHelperBFS(getRoot(), element) != nullptr;
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