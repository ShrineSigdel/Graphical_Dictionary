#include <string>
#include "UI.h"
#include "Trie.h"

// Forward declaration for the dictionary loader.
// (You can also place this function in a separate module if desired.)
TrieNode *LoadDictionary(const std::string &wordsFileName, const std::string &meaningsFileName)
{
    TrieNode *root = getNode();
    FILE *fpWords = fopen(wordsFileName.c_str(), "r");
    FILE *fpMeanings = fopen(meaningsFileName.c_str(), "r");

    if (!fpWords || !fpMeanings)
    {
        // Handle error: you could show a message on the UI if necessary.
        return root;
    }

    char wordBuffer[256];
    char meaningBuffer[1024];

    while (fgets(wordBuffer, sizeof(wordBuffer), fpWords) &&
           fgets(meaningBuffer, sizeof(meaningBuffer), fpMeanings))
    {
        // Remove newline characters if present.
        std::string word(wordBuffer);
        std::string meaning(meaningBuffer);
        if (!word.empty() && word[word.size() - 1] == '\n')
        {
            word.pop_back();
        }
        if (!meaning.empty() && meaning[meaning.size() - 1] == '\n')
        {
            meaning.pop_back();
        }
        insert(root, word, meaning);
    }

    fclose(fpWords);
    fclose(fpMeanings);

    return root;
}

int main()
{
    const int screenWidth = 1600;
    const int screenHeight = 1000;

    // Load the trie-based dictionary from files.
    TrieNode *dictionary = LoadDictionary("oxford.txt", "meaning.txt");

    // Initialize UI (raylib).
    InitUI(screenWidth, screenHeight);
    SetTargetFPS(60);

    Screen currentScreen = HOME;
    std::string searchText = "";

    // Main application loop.
    while (!WindowShouldClose())
    {
        if (currentScreen == HOME)
        {
            bool goToSearch = DrawHomeScreen();
            if (goToSearch)
            {
                searchText = ""; // Reset search text.
                currentScreen = SEARCH;
            }
        }
        else if (currentScreen == SEARCH)
        {
            // Now DrawSearchScreen accepts a TrieNode* instead of a dummy dictionary.
            bool goBack = DrawSearchScreen(dictionary);
            if (goBack)
            {
                currentScreen = HOME;
            }
        }
    }

    CloseUI();
    return 0;
}
