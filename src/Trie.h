#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <algorithm>

// Global variables used by trie functions.
// These are referenced in the dictionary code.
static bool wordfound = false;
static bool searching = true;
static int count = 0;

const int ALPHABET_SIZE = 26;

// Trie node structure that holds a flag, a word's meaning, and pointers to child nodes.
struct TrieNode {
    bool isEndOfWord;
    std::string meaning;
    TrieNode* children[ALPHABET_SIZE];
};

// Creates a new Trie node and initializes its children to nullptr.
TrieNode* getNode() {
    TrieNode* node = new TrieNode;
    node->isEndOfWord = false;
    node->meaning = "";
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = nullptr;
    }
    return node;
}

// Inserts a word and its meaning into the trie.
void insert(TrieNode* root, const std::string &word, const std::string &meaning) {
    TrieNode* pCrawl = root;
    for (char ch : word) {
        int index = ch - 'a';
        if (pCrawl->children[index] == nullptr) {
            pCrawl->children[index] = getNode();
        }
        pCrawl = pCrawl->children[index];
    }
    pCrawl->isEndOfWord = true;
    pCrawl->meaning = meaning;
}

// Basic search function to check if a word exists in the trie.
bool search(TrieNode* root, const std::string &word) {
    TrieNode* pCrawl = root;
    for (char ch : word) {
        int index = ch - 'a';
        if (pCrawl->children[index] == nullptr)
            return false;
        pCrawl = pCrawl->children[index];
    }
    return (pCrawl != nullptr && pCrawl->isEndOfWord);
}

// Helper function: Performs a depth-first search in the subtrie,
// collecting complete oxford (with their meaning) that start with the given prefix.
void dfs(TrieNode* node, const std::string &prefix, std::vector<std::pair<std::string, std::string>> &suggestions) {
    if (node == nullptr)
        return;
    if (node->isEndOfWord) {
        suggestions.push_back({prefix, node->meaning});
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != nullptr) {
            char letter = 'a' + i;
            dfs(node->children[i], prefix + letter, suggestions);
        }
    }
}

// Searches for the word in the trie. If found, prints its meaning;
// otherwise, suggests oxford with the same prefix.
// The global variables 'wordfound' and 'count' are updated accordingly.
void search_and_suggest(TrieNode* root, const std::string &word) {
    TrieNode* pCrawl = root;
    for (char ch : word) {
        int index = ch - 'a';
        if (pCrawl->children[index] == nullptr) {
            wordfound = false;
            count = 0;
            std::cout << "Word not found. No suggestions available.\n";
            return;
        }
        pCrawl = pCrawl->children[index];
    }
    if (pCrawl->isEndOfWord) {
        std::cout << "Word found: " << word << "\nMeaning: " << pCrawl->meaning << "\n";
        wordfound = true;
    } else {
        wordfound = false;
        std::cout << "Word not found as a complete word. Suggestions based on prefix '" << word << "':\n";
    }
    std::vector<std::pair<std::string, std::string>> suggestions;
    dfs(pCrawl, word, suggestions);
    count = suggestions.size();
    if (count > 0) {
        for (const auto &s : suggestions) {
            std::cout << s.first << " : " << s.second << "\n";
        }
    }
}

// Recursively removes a word from the trie. Returns the updated trie root.
// If the word is found, sets the global flag 'wordfound' to true.
TrieNode* remove(TrieNode* root, const std::string &word) {
    if (root == nullptr)
        return nullptr;
    
    if (word.empty()) {
        if (root->isEndOfWord) {
            root->isEndOfWord = false;
            wordfound = true;
        }
        // Check if node is now unnecessary.
        bool isEmpty = true;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (root->children[i] != nullptr) {
                isEmpty = false;
                break;
            }
        }
        if (isEmpty) {
            delete root;
            root = nullptr;
        }
        return root;
    }
    
    int index = word[0] - 'a';
    root->children[index] = remove(root->children[index], word.substr(1));
    
    // If current node is not end of another word and has no children, delete it.
    if (!root->isEndOfWord) {
        bool hasChild = false;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (root->children[i]) {
                hasChild = true;
                break;
            }
        }
        if (!hasChild) {
            delete root;
            root = nullptr;
        }
    }
    return root;
}

#endif // TRIE_H
