#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>

const int ALPHABET_SIZE = 26;

struct TrieNode {
    bool isEndOfWord;
    std::string meaning;
    TrieNode *children[ALPHABET_SIZE];
};

// Creates a new trie node.
TrieNode *getNode();

// Inserts a word and its meaning into the trie.
void insert(TrieNode *root, const std::string &word, const std::string &meaning);

// Checks if a word exists in the trie.
bool search(TrieNode *root, const std::string &word);

// Returns the meaning of the word if it exists; otherwise, returns an empty string.
std::string getMeaning(TrieNode *root, const std::string &word);

// Returns a vector of (word, meaning) suggestions that share the given prefix.
std::vector<std::pair<std::string, std::string>> getSuggestions(TrieNode *root, const std::string &prefix);

// Recursively removes a word from the trie.
TrieNode *remove(TrieNode *root, const std::string &word);

#endif // TRIE_H
