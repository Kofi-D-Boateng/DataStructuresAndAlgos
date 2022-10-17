/**
 * @file PrefixTree.h
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
#include <map>

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

    // A DFS algorithm that will build words from words given in the main function
    std::string wordBuilderHelperDFS(Node *node) const;

    // A helper function that will return whether the word is in the tree or not.
    bool wordSearchHelper(Node *node) const;
};