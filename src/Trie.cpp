#include "Trie.h"
#include <cstdlib>

// Creates a new Trie node and initializes its children to nullptr.
TrieNode *getNode() {
    TrieNode *node = new TrieNode;
    node->isEndOfWord = false;
    node->meaning = "";
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = nullptr;
    }
    return node;
}

// Inserts a word and its meaning into the trie.
void insert(TrieNode *root, const std::string &word, const std::string &meaning) {
    TrieNode *pCrawl = root;
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

// Returns true if the word exists in the trie.
bool search(TrieNode *root, const std::string &word) {
    TrieNode *pCrawl = root;
    for (char ch : word) {
        int index = ch - 'a';
        if (pCrawl->children[index] == nullptr)
            return false;
        pCrawl = pCrawl->children[index];
    }
    return (pCrawl != nullptr && pCrawl->isEndOfWord);
}

// Returns the meaning of the word if found; otherwise, returns an empty string.
std::string getMeaning(TrieNode *root, const std::string &word) {
    TrieNode *pCrawl = root;
    for (char ch : word) {
        int index = ch - 'a';
        if (pCrawl->children[index] == nullptr)
            return "";
        pCrawl = pCrawl->children[index];
    }
    return (pCrawl != nullptr && pCrawl->isEndOfWord) ? pCrawl->meaning : "";
}

// Helper DFS function to collect words and their meanings.
void dfs(TrieNode *node, const std::string &prefix, std::vector<std::pair<std::string, std::string>> &suggestions) {
    if (node == nullptr)
        return;
    if (node->isEndOfWord)
        suggestions.push_back({prefix, node->meaning});
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != nullptr) {
            char letter = 'a' + i;
            dfs(node->children[i], prefix + letter, suggestions);
        }
    }
}

// Returns a vector of suggestions (word and meaning) based on the given prefix.
std::vector<std::pair<std::string, std::string>> getSuggestions(TrieNode *root, const std::string &prefix) {
    TrieNode *pCrawl = root;
    for (char ch : prefix) {
        int index = ch - 'a';
        if (pCrawl->children[index] == nullptr)
            return {};  // No suggestions available.
        pCrawl = pCrawl->children[index];
    }
    std::vector<std::pair<std::string, std::string>> suggestions;
    dfs(pCrawl, prefix, suggestions);
    return suggestions;
}

// Recursively removes a word from the trie.
TrieNode *remove(TrieNode *root, const std::string &word) {
    if (root == nullptr)
        return nullptr;

    if (word.empty()) {
        if (root->isEndOfWord) {
            root->isEndOfWord = false;
        }
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
