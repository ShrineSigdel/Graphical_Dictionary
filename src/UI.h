#ifndef UI_H
#define UI_H

#include "raylib.h"
#include <vector>
#include <string>
#include "Trie.h"

// Enum for different screens in the UI.
enum Screen
{
    HOME,       // Home screen where users can navigate
    SEARCH,     // Search screen for looking up words
    ADDWORD     // Add word screen to insert a new word
};

// Initializes the UI with a specified screen size.
void InitUI(int width, int height);

// Unloads resources and closes the window, including freeing up memory used by the Trie.
void CloseUI(TrieNode *root);

// Draws the home screen UI. Returns the screen the user wishes to navigate to,
// based on button clicks (e.g., "Search" or "Add Word").
Screen DrawHomeScreen();

// Draws the search screen UI, where the user can search for a word. This function
// also updates the search text, shows suggestions based on the input, and displays
// the meaning of the word. Returns true if the "Back" button is pressed.
bool DrawSearchScreen(TrieNode *dictionary);

// Draws the "Add Word" screen, allowing users to add a new word along with its meaning
// to the dictionary. This function takes the filenames for words and meanings to
// load the data accordingly.
bool DrawAddWordScreen(TrieNode *dictionary, const std::string &wordFilename, const std::string &meaningFilename);

#endif // UI_H
