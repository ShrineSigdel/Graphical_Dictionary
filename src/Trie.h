#include<iostream>
#include<string>


using namespace std;
const int num_of_alphabets=26;

class TrieNode{
    public:
    TrieNode *children[num_of_alphabets];
    bool isEndofWord;
    string word;
    string meaning;

    TrieNode() {
        isEndofWord = false;
        word = "";
        meaning = "";
        for (int i = 0; i < num_of_alphabets; i++) {
            children[i] = nullptr;  // Initialize all child pointers to NULL
        }
    }
};

//insertion

void insert(TrieNode* root, string key, string mean) {
    TrieNode* ptr = root;
    for (int i = 0; i < key.length(); i++) {
        int index = key[i] - 'a';
        if (ptr->children[index] == nullptr) {
            ptr->children[index] = new TrieNode();
        }
        ptr = ptr->children[index];
    }
    ptr->isEndofWord = true;
    ptr->word = key;
    ptr->meaning = mean;
}

bool search(TrieNode* root, const string& key) {
    TrieNode* ptr = root;
    
    // Traverse the Trie for each character of the key
    for (int i = 0; i < key.length(); i++) {
        int index = key[i] - 'a'; // Convert character to index
        
        // If the child node for this character does not exist
        if (!ptr->children[index]) {
            return false; // The word is not in the Trie
        }
        
        // Move to the next node
        ptr = ptr->children[index];
    }
    
    // After traversing all characters, check if it's the end of the word
    if (ptr != nullptr && ptr->isEndofWord) {
        // Word found, return the meaning
        cout << "Word found: " << ptr->word << endl;
        cout << "Meaning: " << ptr->meaning << endl;
        return true;
    }
    
    // If the word is not found
    return false;
}

bool deleteWord(TrieNode* &root, const string &key, int depth = 0) {
    // Base case: If Trie is empty
    if (!root) return false;

    // If we reached the end of the word
    if (depth == key.length()) {
        // If this node is an end of a word, unmark it
        if (root->isEndofWord) {
            root->isEndofWord = false;
            root->meaning = ""; // Clear the meaning

            // Check if the node has any children
            for (int i = 0; i < num_of_alphabets; i++) {
                if (root->children[i] != nullptr)
                    return false; // If children exist, don’t delete the node
            }

            // If no children, delete this node
            delete root;
            root = nullptr;
            return true;
        }
        return false;
    }

    // Get the index of the current character
    int index = key[depth] - 'a';
    if (!root->children[index]) return false; // Word not found

    // Recursive call for next character
    bool shouldDeleteCurrentNode = deleteWord(root->children[index], key, depth + 1);

    // After returning from recursion, check if child node should be deleted
    if (shouldDeleteCurrentNode) {
        delete root->children[index];
        root->children[index] = nullptr;

        // If current node is not an end of another word and has no children, delete it
        if (!root->isEndofWord) {
            for (int i = 0; i < num_of_alphabets; i++) {
                if (root->children[i] != nullptr)
                    return false; // If any child exists, don't delete
            }
            delete root;
            root = nullptr;
            return true;
        }
    }

    return false;
}

    
