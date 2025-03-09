#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "UI.h"
#include "Trie.h"  // Trie functions: getNode(), insert(), getMeaning(), getSuggestions(), etc.

/**
 * @brief Main function to initialize and run the Dictionary Application.
 * 
 * - Loads the dictionary from files into a Trie.
 * - Initializes the graphical user interface (GUI).
 * - Manages screen transitions (Home, Search, Add Word).
 * - Runs the main application loop until the window is closed.
 * 
 * @return int Returns 0 on successful execution, 1 if files fail to open.
 */
int main()
{
    // Create the root node for the Trie data structure.
    TrieNode *dictionary = getNode();

    // Open the dictionary files (words in "oxford.txt", meanings in "meaning.txt").
    std::ifstream inOxford("oxford.txt"), inMeaning("meaning.txt");
    if (!inOxford || !inMeaning)
    {
        std::cerr << "Error: Could not open dictionary files." << std::endl;
        return 1;
    }

    // Read words and their meanings from the files and insert them into the Trie.
    std::string word, meaning;
    while (std::getline(inOxford, word) && std::getline(inMeaning, meaning))
    {
        insert(dictionary, word, meaning);
    }
    inOxford.close();
    inMeaning.close();

    // Initialize UI with screen dimensions.
    const int screenWidth = 1600;
    const int screenHeight = 1000;
    InitUI(screenWidth, screenHeight);
    SetTargetFPS(60);

    // Screen state management variable.
    Screen currentScreen = HOME;

    // Main application loop: Runs until the user closes the window.
    while (!WindowShouldClose())
    {
        switch (currentScreen)
        {
            case HOME:
                // Render the home screen and determine the next screen.
                currentScreen = DrawHomeScreen();
                break;

            case SEARCH:
                // Render the search screen; return to home if back button is pressed.
                if (DrawSearchScreen(dictionary))
                    currentScreen = HOME;
                break;

            case ADDWORD:
                // Render the add word screen; return to home if back button is pressed.
                if (DrawAddWordScreen(dictionary, "oxford.txt", "meaning.txt"))
                    currentScreen = HOME;
                break;
        }
    }

    // Clean up resources and close the application.
    CloseUI(dictionary);
    return 0;
}
