#include "Trie.h"
#include <cstdlib>

// Creates a new Trie node with initialized properties (children set to nullptr).
TrieNode *getNode()
{
    TrieNode *node = new TrieNode;
    node->isEndOfWord = false; // Word termination flag
    node->meaning = "";        // Meaning of the word (empty by default)
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        node->children[i] = nullptr; // Initialize all child pointers to nullptr
    }
    return node;
}

// Inserts a word with its meaning into the Trie.
void insert(TrieNode *root, const std::string &word, const std::string &meaning)
{
    TrieNode *pCrawl = root;
    for (char ch : word)
    {
        ch = tolower(ch);     // Ensure the character is lowercase
        int index = ch - 'a'; // Get the index for the current character
        if (index < 0 || index >= ALPHABET_SIZE)
            continue; // Ignore non-alphabet characters
        if (pCrawl->children[index] == nullptr)
        {
            pCrawl->children[index] = getNode(); // Create a new node if the child doesn't exist
        }
        pCrawl = pCrawl->children[index]; // Move to the next node in the path
    }
    pCrawl->isEndOfWord = true; // Mark the end of the word
    pCrawl->meaning = meaning;  // Store the word's meaning
}

// Searches for a word in the Trie and returns true if found.
bool search(TrieNode *root, const std::string &word)
{
    TrieNode *pCrawl = root;
    for (char ch : word)
    {
        int index = ch - 'a'; // Calculate index for the character
        if (pCrawl->children[index] == nullptr)
            return false;                 // If the child doesn't exist, return false
        pCrawl = pCrawl->children[index]; // Move to the next node
    }
    return (pCrawl != nullptr && pCrawl->isEndOfWord); // Check if it's the end of the word
}

// Retrieves the meaning of a word if found in the Trie, otherwise returns an empty string.
std::string getMeaning(TrieNode *root, const std::string &word)
{
    TrieNode *pCrawl = root;
    for (char ch : word)
    {
        int index = ch - 'a'; // Calculate the index for the character
        if (pCrawl->children[index] == nullptr)
            return "";                    // Return an empty string if the word is not found
        pCrawl = pCrawl->children[index]; // Move to the next node
    }
    return (pCrawl != nullptr && pCrawl->isEndOfWord) ? pCrawl->meaning : ""; // Return meaning if word is valid
}

// Depth-first search helper to collect all words and their meanings from the Trie.
void dfs(TrieNode *node, const std::string &prefix, std::vector<std::pair<std::string, std::string>> &suggestions)
{
    if (node == nullptr)
        return;
    if (node->isEndOfWord)
        suggestions.push_back({prefix, node->meaning}); // Add word and meaning if it's an end of a word
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (node->children[i] != nullptr)
        {
            char letter = 'a' + i;                                // Get character corresponding to index
            dfs(node->children[i], prefix + letter, suggestions); // Recurse into the child node
        }
    }
}

// Returns a list of words with their meanings based on the provided prefix.
std::vector<std::pair<std::string, std::string>> getSuggestions(TrieNode *root, const std::string &prefix)
{
    TrieNode *pCrawl = root;
    for (char ch : prefix)
    {
        int index = ch - 'a'; // Calculate the index for the character
        if (pCrawl->children[index] == nullptr)
            return {};                    // Return empty vector if no words match the prefix
        pCrawl = pCrawl->children[index]; // Move to the next node
    }
    std::vector<std::pair<std::string, std::string>> suggestions;
    dfs(pCrawl, prefix, suggestions); // Collect all matching words and meanings
    return suggestions;
}

// Recursively removes a word from the Trie.
TrieNode *remove(TrieNode *root, const std::string &word)
{
    if (root == nullptr)
        return nullptr;

    if (word.empty())
    {
        if (root->isEndOfWord)
        {
            root->isEndOfWord = false; // Unmark the word as the end of the word
        }
        bool isEmpty = true;
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            if (root->children[i] != nullptr)
            {
                isEmpty = false; // Check if any child exists
                break;
            }
        }
        if (isEmpty)
        {
            delete root; // Delete the node if it's empty
            root = nullptr;
        }
        return root;
    }

    int index = word[0] - 'a';                                             // Get the index for the current character
    root->children[index] = remove(root->children[index], word.substr(1)); // Recurse and remove the word

    // If current node is not the end of a word, and it has no children, delete the node
    if (!root->isEndOfWord)
    {
        bool hasChild = false;
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            if (root->children[i])
            {
                hasChild = true;
                break;
            }
        }
        if (!hasChild)
        {
            delete root;
            root = nullptr;
        }
    }
    return root;
}

// Deletes the entire Trie by recursively removing all nodes.
void deleteTrie(TrieNode *&root)
{
    if (!root)
        return;
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        deleteTrie(root->children[i]); // Recurse to delete all child nodes
    }
    delete root;    // Delete the current node
    root = nullptr; // Nullify the root pointer
}
