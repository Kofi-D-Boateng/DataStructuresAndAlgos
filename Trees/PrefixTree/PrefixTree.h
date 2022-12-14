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
#include <queue>

// This is an implementation of a Prefix Tree, or Trie Tree. A Prefix tree
// is a type of tree that is used to store alphabetical words into nodes.
// Common use cases for Prefix trees are autocorrect and problems that deal
// storing and looking up words.

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
        Node() : children(), endOfWord(false) {}

        // Copy constructor
        Node(const Node &other) : children(other.children), endOfWord(other.endOfWord) {}

        // Copy assignemnt operator
        Node &operator=(const Node &other)
        {
            children = other.children;
            endOfWord = other.endOfWord;

            return *this;
        }

        // Deconstructor
        ~Node() {}
    };

private:
    // Root of the tree.
    Node *root;
    // A counter that keeps track of how many full words are currently in the tree.

    int wordCount_;

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
    void wordBuilderHelperDFS(Node *node, std::string &word, std::vector<std::string> &wordCollection);

    // A DFS helper function that will return whether the word is in the tree or not.
    bool wordSearchHelper(const int &index, Node *node, const std::string &word) const;

public:
    // Returns size of the tree;
    int size() const { return wordCount_; }

    // Checks if the tree is empty or not
    bool isEmpty() const { return wordCount_ == 0; }

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

    // Checks for equality between two list.
    // Two list are equal if they have the same
    // length and same data at each position. O(n).
    bool equals(const PrefixTree &obj) const;
    bool operator==(const PrefixTree &obj) const { return equals(obj); }
    bool operator!=(const PrefixTree &obj) const { return !equals(obj); }

    // Builds all the words that start with the suggested prefix.
    // A prefix can be a single character "A" or a regular prefix
    // such as "Re-", "En-", however a dash , "-", is not needed
    // to signify the prefix. The function will either fill the
    // passed vector with a collection of words or there will be
    // no words present.
    void wordBuilder(std::string &prefix, std::vector<std::string> &wordCollection);

    PrefixTree() : root(new Node()), wordCount_(0) {}

    PrefixTree(const PrefixTree &other) : PrefixTree()
    {
        *this = other;
    }

    PrefixTree &operator=(const PrefixTree &other)
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

void PrefixTree::wordRemoverDFS(Node *node, const int &index, const std::string &word)
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

bool PrefixTree::wordSearchHelper(const int &index, Node *node, const std::string &word) const
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

void PrefixTree::wordBuilderHelperDFS(Node *node, std::string &word, std::vector<std::string> &wordCollection)
{
    // Check to see if we can add the word
    // This is essentially our first base case, however
    // We do not need to return just in case there
    // Is a word that can still be built.
    if (node->endOfWord == true)
    {
        wordCollection.push_back(word);
    }
    // Index of the last character in word. This character should have been the trigger of the
    // previous call, so we can assume that this character exist in the tree. We will also use
    // this index to remove the character once all of its children have been exhausted.
    for (auto entry : node->children)
    {
        // Push the character key onto the string (i.e. word = A-N, entry.fisrt = 'T', word = A-N-T).
        word.push_back(entry.first);

        // This is our second base case. Since we are not returning on nullptrs,
        // or any other flag, we will control how many times we recurse here.
        // Essentially, If there are no more children to call on, we will not
        // recurse anymore. The above case handles are nodes marked as complete
        // words.
        wordBuilderHelperDFS(entry.second, word, wordCollection);

        // Backtracking part: Remove last character to form new word in
        // another stack frame.
        word.pop_back();
    }
}

// ===========================================================================================================
// Public Methods
// ============================================================================================================

void PrefixTree::insert(const std::string &word)
{
    Node *currNode = root;
    for (char const &ch : word)
    {
        if (currNode->children.find(ch) != currNode->children.end())
        {
            currNode = currNode->children[ch];
        }
        else
        {
            Node *newNode = new Node();
            currNode->children[ch] = newNode;
            currNode = newNode;
        }
    }
    currNode->endOfWord = true;
    wordCount_++;
}

bool PrefixTree::search(const std::string &word) const { return wordSearchHelper(0, root, word); }

void PrefixTree::remove(const std::string &word)
{
    if (!root)
    {
        return;
    }
    wordRemoverDFS(root, 0, word);
    wordCount_--;
}

void PrefixTree::wordBuilder(std::string &prefix, std::vector<std::string> &wordCollection)
{

    Node *currNode = root;

    if (prefix.length() <= 0)
    {
        return;
    }

    for (int runner = 0; runner < prefix.length(); runner++)
    {

        // Case 1:
        // If the prefix is not contained in the tree, we will return an
        // empty listing
        if (!currNode->children[prefix[runner]])
        {
            break;
        }
        // Case 2:
        // Runner is at the last character in the string.
        else if (currNode->children[prefix[runner]] && runner + 1 == prefix.length())
        {
            Node *node = currNode->children[prefix[runner]];
            for (auto entry : node->children)
            {
                prefix.append(1, entry.first);
                wordBuilderHelperDFS(entry.second, prefix, wordCollection);
                prefix.pop_back();
            }
        }
        // Case 3:
        // Continue traversing
        else
        {
            currNode = currNode->children[prefix[runner]];
        }
    }
}

bool PrefixTree::equals(const PrefixTree &other) const
{
    if (!root || size() != other.size())
    {
        return false;
    }

    std::queue<Node *> queueThis;
    std::queue<Node *> queueOther;
    queueThis.push(root);
    queueOther.push(other.root);

    while (!queueThis.empty())
    {
        if (queueOther.empty())
        {
            return false;
        }
        Node *thisNode = queueThis.front();
        Node *otherNode = queueOther.front();
        queueThis.pop();
        queueOther.pop();

        std::queue<char> charThis;
        std::queue<char> charOther;

        if (thisNode->endOfWord != otherNode->endOfWord)
        {
            return false;
        }

        for (auto const &[character, node] : thisNode->children)
        {
            queueThis.push(node);
            charThis.push(character);
        }

        for (auto const &[character, node] : otherNode->children)
        {
            queueOther.push(node);
            charOther.push(character);
        }

        while (!charThis.empty() && !charOther.empty())
        {
            if (charThis.front() != charOther.front())
            {
                return false;
            }
            else
            {
                charThis.pop();
                charOther.pop();
            }
        }

        if (charThis.size() != charOther.size())
        {
            return false;
        }
    }

    return queueOther.empty() && queueThis.empty();
}