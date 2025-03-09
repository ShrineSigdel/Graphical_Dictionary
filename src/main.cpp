#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "UI.h"
#include "Trie.h" // Contains getNode(), insert(), getMeaning(), getSuggestions(), etc.

// Assuming Screen is defined in UI.h like so:
// enum Screen { HOME, SEARCH, ADDWORD };

// Modified main() that loads the dictionary from file before initializing UI.
int main()
{
    // Create the root node for the trie.
    TrieNode *dictionary = getNode();

    // Open the files for oxford and meaning.
    std::ifstream inOxford("oxford.txt"), inMeaning("meaning.txt");
    if (!inOxford || !inMeaning)
    {
        std::cerr << "Error: Could not open dictionary files." << std::endl;
        return 1;
    }

    std::string word, meaning;
    // Read oxford and meaning until end-of-file.
    while (std::getline(inOxford, word) && std::getline(inMeaning, meaning))
    {
        // Optionally, trim newline characters or whitespace here.
        insert(dictionary, word, meaning);
    }
    inOxford.close();
    inMeaning.close();

    // Set up screen dimensions for the UI.
    const int screenWidth = 1600;
    const int screenHeight = 1000;

    // Initialize UI (raylib).
    InitUI(screenWidth, screenHeight);
    SetTargetFPS(60);

    // Screen state management.
    Screen currentScreen = HOME;

    // Main application loop.
    while (!WindowShouldClose())
    {
        if (currentScreen == HOME)
        {
            // Draw the home screen and check which button (if any) was pressed.
            Screen nextScreen = DrawHomeScreen();
            if (nextScreen != HOME)
            {
                currentScreen = nextScreen;
            }
        }
        else if (currentScreen == SEARCH)
        {
            // Draw the search screen, passing the loaded trie dictionary.
            bool goBack = DrawSearchScreen(dictionary);
            if (goBack)
            {
                currentScreen = HOME;
            }
        }
        else if (currentScreen == ADDWORD)
        {
            // Draw the add word screen.
            bool goBack = DrawAddWordScreen(dictionary, "oxford.txt", "meaning.txt");
            if (goBack)
            {
                currentScreen = HOME;
            }
        }
    }

    CloseUI(dictionary);
    return 0;
}
