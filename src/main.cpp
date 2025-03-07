#include <vector>
#include <string>
#include "UI.h"
#include "DictionaryEntry.h"

int main()
{
    const int screenWidth = 1600;
    const int screenHeight = 1000;

    // Create a dummy dictionary with both words and their meanings.
    std::vector<DictionaryEntry> dictionary = {
        {"apple", "A fruit that is sweet and crisp."},
        {"banana", "A long curved fruit that has a thick skin and soft sweet flesh."},
        {"cherry", "A small, round stone fruit that is typically bright or dark red."},
        {"date", "The fruit of the date palm, a staple in Middle Eastern diets."},
        {"smart", "Having or showing a quick-witted intelligence."},
        {"small", "Of a size that is less than normal or usual."},
        {"smack", "Hit (someone) sharply and violently."},
        {"smile", "Form one’s features into a pleased, kind, or amused expression."},
        {"smirk", "Smile in an irritatingly smug, conceited, or silly way."}
    };

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
