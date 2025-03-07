#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int num_of_alphabets = 26;

class TrieNode
{
public:
    TrieNode *children[num_of_alphabets];
    bool isEndofWord;
    string word;
    string meaning;

    TrieNode();
};

TrieNode::TrieNode()
{
    isEndofWord = false;
    word = "";
    meaning = "";
    for (int i = 0; i < num_of_alphabets; i++)
    {
        children[i] = nullptr;
    }
}

// Insertion Function: Adds a new word and its meaning to the Trie
void insert(TrieNode *root, const string &key, const string &mean)
{
    TrieNode *ptr = root;
    for (char c : key)
    {
        int index = c - 'a'; // Character index (assuming lowercase letters)

        if (index < 0 || index >= num_of_alphabets)
        {
            cout << "Invalid character in word: " << c << endl;
            return;
        }

        if (!ptr->children[index])
        {
            ptr->children[index] = new TrieNode();
        }

        ptr = ptr->children[index];
    }

    ptr->isEndofWord = true;
    ptr->word = key;                                      // Store the word
    ptr->meaning = mean;                                  // Store the meaning
    cout << "Inserted: " << key << " - " << mean << endl; // Debug print
}

// Search Function: Searches for a word and displays its meaning if found
bool search(TrieNode *root, const string &key)
{
    TrieNode *ptr = root;

    cout << "Searching for: " << key << endl; // Debug print

    for (char c : key)
    {
        int index = c - 'a'; // Character index (assuming lowercase letters)

        if (index < 0 || index >= num_of_alphabets)
        {
            cout << "Invalid character in word: " << c << endl;
            return false;
        }

        if (!ptr->children[index])
        {
            cout << "Failed at character: " << c << endl; // Debug print
            return false;
        }
        ptr = ptr->children[index];
    }

    if (ptr->isEndofWord)
    {
        cout << "Word found: " << ptr->word << endl;
        cout << "Meaning: " << ptr->meaning << endl;
        return true;
    }

    cout << "Reached end but not marked as word." << endl; // Debug print
    return false;
}

// Deletion Function: Deletes a word from the Trie while maintaining structure integrity
bool deleteWord(TrieNode *&root, const string &key, int depth = 0)
{
    // Base case: If Trie is empty
    if (!root)
        return false;

    // If we reached the end of the word
    if (depth == key.length())
    {
        // If this node is an end of a word, unmark it
        if (root->isEndofWord)
        {
            root->isEndofWord = false;
            root->word = "";    // Clear the word
            root->meaning = ""; // Clear the meaning

            // Check if the node has any children
            for (int i = 0; i < num_of_alphabets; i++)
            {
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
    if (!root->children[index])
        return false; // Word not found

    // Recursive call for next character
    bool shouldDeleteCurrentNode = deleteWord(root->children[index], key, depth + 1);

    // After returning from recursion, check if child node should be deleted
    if (shouldDeleteCurrentNode)
    {
        delete root->children[index];
        root->children[index] = nullptr;

        // If current node is not an end of another word and has no children, delete it
        if (!root->isEndofWord)
        {
            for (int i = 0; i < num_of_alphabets; i++)
            {
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

// Load data from words.txt and meanings.txt into the Trie


void load_data(TrieNode *root)
{
    ifstream wordsFile("words.txt");
    ifstream meaningsFile("meanings.txt");

    string word, meaning;

    if (!wordsFile.is_open())
    {
        cout << "Error opening words.txt." << endl;
    }
    if (!meaningsFile.is_open())
    {
        cout << "Error opening meanings.txt." << endl;
    }

    while (getline(wordsFile, word) && getline(meaningsFile, meaning))
    {
        // Trim leading/trailing whitespace
        word.erase(0, word.find_first_not_of(' '));
        word.erase(word.find_last_not_of(' ') + 1);
        meaning.erase(0, meaning.find_first_not_of(' '));
        meaning.erase(meaning.find_last_not_of(' ') + 1);

        insert(root, word, meaning);                            // Insert both word and meaning into Trie
        cout << "Loaded: " << word << " - " << meaning << endl; // Debug print
    }

    wordsFile.close();
    meaningsFile.close();

    cout << "Dictionary Loaded from files." << endl;
}

// Save new words to words.txt and meanings.txt
void save_word(const string &word, const string &meaning)
{
    ofstream wordsFile("words.txt", ios::app);
    ofstream meaningsFile("meanings.txt", ios::app);

    if (!wordsFile.is_open() || !meaningsFile.is_open())
    {
        cout << "Error opening files." << endl;
        return;
    }

    wordsFile << word << endl;
    meaningsFile << meaning << endl;

    wordsFile.close();
    meaningsFile.close();

    cout << "Word saved: " << word << " - " << meaning << endl; // Debug print
}
