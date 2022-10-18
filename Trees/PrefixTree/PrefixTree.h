/**
 * @file PrefixTree.h
 * @author Kofi Boateng
 * @version 0.1
 * @date 2022-10-18
 *
 *
 */

#pragma once
#include <iostream>  // for cout & cerr
#include <stdexcept> // for runtime_error
#include <ostream>   // for::ostream
#include <map>       // used to map characters to nodes
#include <vector>    // used to return multiple words built from prefixes

// This is an implementation of a Prefix Tree, or Trie Tree. A Prefix tree
// is a type of tree that is used to store alphabetical words into nodes.
// Common use cases for Prefix trees are autocorrect and problems that deal
// storing and looking up words.

template <typename T>
class PrefixTree
{
public:
    class Node
    {
    public:
        // A map of characters and their associated node. Each map can hold
        // at most 52 characters, upper and lower case.
        std::map<char, Node *> children;

        // A flag that designates whether it is the end of a word or not.
        bool endOfWord;

        // Default constructor;
        Node() : children(std::map<char, Node *>), endOfWord(false) {}

        // Copy constructor
        Node(const Node &other) : children(other.children), endOfWord(other.endOfWord) {}

        // Copy assignemnt operator
        Node &operator=(const Node &other)
        {
            children = other.children;
            endOfWord = other.endOfWord;

            return *this
        }

        // Deconstructor
        ~Node() {}
    };

private:
    // Root of the tree.
    Node *root;
    // A counter that keeps track of how many full words are currently in the tree.

    int wordCount;

    // ClearTree is used to recursively remove elements from the
    // the tree during deallocation.
    void clearTree(Node *node);


    // A DFS function that will find a word in the tree "delete" each of its node representations.
    void wordRemoverDFS(Node *node, const int &index, const std::string &word);

    // A DFS algorithm that will build words from words given in the main function.
    // TLDR: Backtracking functionality

    // We will traverse down each child of the node, appending characters to word until we hit a "True"
    // flag, in which case we will add the word to the collection. If the "True" flag is followed by
    // a non empty list, we will continue to loop through the children of that node as wel. Once the
    // children have all been exhausted we will then remove the character.
    void wordBuilderHelperDFS(Node *node, std::string word, std::vector<std::string> wordCollection);

    // A DFS helper function that will return whether the word is in the tree or not.
    bool wordSearchHelper(const int &index, Node *node, const std::string &word) const;

public:
    // Returns size of the tree;
    int size() const { return wordCount_; }

    // Inserts a word into the tree.
    void insert(const std::string &word);

    // Searches for the existence of a word in the tree.
    // This function will also account for wildcards as well.
    // Wildcards will only match any character per wildcard.
    // Wildcard characters will be only the following: "*".
    bool search(const std::string &word) const;

    // Removes the suggested word from the tree, if the word exists, or
    // will return without removing anything.
    void remove(const std::string &word);

    // Builds all the words that start with the suggested prefix.
    // A prefix can be a single character "A" or a regular prefix
    // such as "Re-", "En-", however a dash , "-", is not needed
    // to signify the prefix.
    std::vector<std::string> wordBuilder(const std::string &prefix) const;

    PrefixTree() : root(nullptr) wordCount_(0) {}

    PrefixTree(const PrefixTree<T> &other) : PrefixTree()
    {
        *this = other;
    }

    PrefixTree<T> &operator=(const PrefixTree<T> &other)
    {
        root = other.root;
        wordCount_ = other.wordCount_;

        return *this;
    }
};

// ===============================================================================================================================
// Implementation Section
// ===============================================================================================================================

// ===========================================================================================================
// Helper Functions
// ===========================================================================================================

template <typename T>
void PrefixTree<T>::wordRemoverDFS(Node *node, const int &index, const std::string &word)
{
    // Instead of deallocating the word from the tree, we will change the flag to false,
    // signifying the word no longer "exists in the tree".

    // Example A-N-T and A-N-T-E-N-N-A, since T is the end of the word (ant), its flag
    // is set to true. However, when we "delete" the word ant, we will set the T flag to
    // false and all that is left is A-N-T-E-N-N-A.
    if (node->endOfWord)
    {
        node->endOfWord = false;
        return;
    }

    if (!node->children[word[index]])
    {
        return;
    }

    wordRemoverDFS(node->children[word[index]], index + 1, word);
}

template <typename T>
bool PrefixTree<T>::wordSearchHelper(const int &index, Node *node, const std::string &word) const
{
    Node *currNode = node;

    for (int i = index; i < word.length(); i++)
    {
        // Wildcard will trigger an iteration over the current node's children in the map
        if (word[i] == '*')
        {
            for (auto const &entry : currNode->children)
            {
                // Increment one index passed the wildcard to match rest of characters
                // against every child.
                if (wordSearchHelper(i + 1, entry.second, word))
                {
                    return true;
                }
            }
            return false;
        }
        // Regular search for children
        else
        {
            if (!currNode->children[word[i]])
            {
                return false;
            }
            else
            {
                currNode = currNode->children[word[i]];
            }
        }
    }

    return currNode->endOfWord;
}

template <typename T>
void PrefixTree<T>::wordBuilderHelperDFS(Node *node, std::string word, std::vector<std::string> wordCollection)
{

    // Index of the last character in word. This character should have been the trigger of the
    // previous call, so we can assume that this character exist in the tree. We will also use
    // this index to remove the character once all of its children have been exhausted.

    for (auto const &entry : node->children)
    {
        // Push the character key onto the string (i.e. word = A-N, entry.fisrt = 'T', word = A-N-T).
        word.push_back(entry.first);

        // Check to see if we can add the word
        if (node->endOfWord == true)
        {
            wordCollection.push_back(word);
        }
        // This will loop through all the children of each node.
        // This is will act as our base case. When empty we will
        // commence backtracking. This loop will be passed if the
        // children's map is empty.
        int size = entry.second.size();
        while (size > 0)
        {
            wordBuilderHelperDFS(entry.second, word, wordCollection);
            size--;
        }

        // Backtracking part: Remove last character to form new word in
        // another stack frame.
        word.pop_back();
    }
}

// ===========================================================================================================
// Public Methods
// ============================================================================================================

template <typename T>
void PrefixTree<T>::insert(const std::string &word)
{
    Node *currNode = root;
    for (char const &ch : word)
    {
        if (root->children[ch])
        {
            currNode = root->children[ch];
        }
        else
        {
            Node *newNode = Node();
            root->children[ch] = newNode;
            currNode = newNode;
        }
    }
    currNode->endOfWord = true;
}

template <typename T>
bool PrefixTree<T>::search(const std::string &word) const { return wordSearchHelper(0, root); }

template <typename T>
void PrefixTree<T>::remove(const std::string &word)
{
    if (!root)
    {
        return;
    }
    wordRemoverDFS(root, 0, word);
}

template <typename T>
std::vector<std::string> PrefixTree<T>::wordBuilder(const std::string &prefix) const
{

    // The bumper is the index at which we will recursively go through each child of the last
    // character of the prefix to build the words
    int bumper = prefix.length() - 1;

    // Collection of words
    std::vector<std::string> wordCollection;

    Node *currNode = root;

    for (int runner = 0; runner < prefix.length(); runner++)
    {

        // Case 1:
        // If the prefix is not contained in the tree, we will return an
        // empty listing
        if (!currNode->children[prefix[runner]])
        {
            return wordCollection;
        }
        // Case 2:
        // End of prefix and hit the bumper.
        else if (currNode->children[prefix[runner]] && prefix == bumper && prefix[runner] == prefix[bumper])
        {
            Node *node = currNode->children[prefix[runner]];
            for (auto const &entry : node->children)
            {
                wordBuilderHelperDFS(entry.second, prefix, wordCollection);
            }
        }
        // Case 3:
        // Continue traversing
        else
        {
            currNode = currNode->children[prefix[runner]];
        }
    }

    return wordCollection;
}