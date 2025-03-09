#include "UI.h"
#include "Trie.h" // Include trie functions and structure.
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <deque>

// Global variables for screen dimensions and textures.
static int screenWidth;
static int screenHeight;
static Texture2D backgroundTexture;
static Texture2D logoTexture; // Dictionary logo texture

// Global fonts used for drawing text.
static Font sRegularFont;
static Font sItalicFont;

// These variables will store the search text and the displayed meaning.
static std::string displayedMeaning = "";
static std::string searchText = "";

/**
 * @brief Initializes the UI components including the window, fonts, and textures.
 *
 * @param width  The width of the window.
 * @param height The height of the window.
 */
void InitUI(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    InitWindow(screenWidth, screenHeight, "Dictionary App");

    // Disable ESC key from closing the window.
    SetExitKey(0);

    // Load custom fonts.
    sRegularFont = LoadFont("../assets/fonts/Regular.ttf");
    sItalicFont = LoadFont("../assets/fonts/Italic.ttf");

    // Load background and logo textures.
    backgroundTexture = LoadTexture("../assets/images/background.jpg");
    logoTexture = LoadTexture("../assets/images/logo.png");
}

/**
 * @brief Closes the UI by unloading textures, deleting the Trie, and closing the window.
 *
 * @param root Pointer to the root of the Trie.
 */
void CloseUI(TrieNode *root)
{
    UnloadTexture(backgroundTexture);
    UnloadTexture(logoTexture);
    deleteTrie(root);
    CloseWindow();
}

/**
 * @brief Renders the home screen with options for searching or adding words.
 *
 * @return A Screen enum indicating the next screen to navigate to.
 */
Screen DrawHomeScreen()
{
    BeginDrawing();

    if (backgroundTexture.id > 0)
        DrawTexture(backgroundTexture, 0, 0, WHITE);
    else
        ClearBackground(RAYWHITE);

    DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 90});

    float logoScale = 0.3f;
    if (logoTexture.id > 0)
    {
        DrawTextureEx(logoTexture, (Vector2){20, 20}, 0.0f, logoScale, WHITE);
    }

    const char *title = "Dictionary App";
    float titleFontSize = 70.0f;
    Vector2 titleSize = MeasureTextEx(sRegularFont, title, titleFontSize, 2);
    DrawTextEx(sRegularFont, title,
               (Vector2){screenWidth / 2 - titleSize.x / 2, 100},
               titleFontSize, 2, DARKBLUE);

    const char *intro = "Search for words and discover meanings";
    float introFontSize = 35.0f;
    Vector2 introSize = MeasureTextEx(sItalicFont, intro, introFontSize, 2);
    DrawTextEx(sItalicFont, intro,
               (Vector2){screenWidth / 2 - introSize.x / 2, 180},
               introFontSize, 2, BLUE);

    // ----- Search Button -----
    Rectangle searchButton = {screenWidth / 2 - 110, screenHeight / 2 - 50, 220, 70};
    Color searchButtonColor = BLUE; // Set the button background color to blue
    if (CheckCollisionPointRec(GetMousePosition(), searchButton))
    {
        searchButtonColor = DARKBLUE; // Darker blue when hovered
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            displayedMeaning = "";
            EndDrawing();
            return SEARCH;
        }
    }
    DrawRectangleRec(searchButton, searchButtonColor);
    DrawRectangleLinesEx(searchButton, 3, DARKBLUE);
    Vector2 searchTextSize = MeasureTextEx(sRegularFont, "Search", 32.0f, 2);
    DrawTextEx(sRegularFont, "Search",
               (Vector2){searchButton.x + searchButton.width / 2 - searchTextSize.x / 2,
                         searchButton.y + searchButton.height / 2 - searchTextSize.y / 2},
               32.0f, 2, WHITE); // Set text color to white

    // ----- Add Word Button -----
    Rectangle addWordButton = {screenWidth / 2 - 110, screenHeight / 2 + 30, 220, 70};
    Color addWordButtonColor = BLUE; // Set the button background color to blue
    if (CheckCollisionPointRec(GetMousePosition(), addWordButton))
    {
        addWordButtonColor = DARKBLUE; // Darker blue when hovered
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            EndDrawing();
            return ADDWORD;
        }
    }

    DrawRectangleRec(addWordButton, addWordButtonColor);
    DrawRectangleLinesEx(addWordButton, 3, DARKBLUE);
    Vector2 addWordTextSize = MeasureTextEx(sRegularFont, "Add Word", 32.0f, 2);
    DrawTextEx(sRegularFont, "Add Word",
               (Vector2){addWordButton.x + addWordButton.width / 2 - addWordTextSize.x / 2,
                         addWordButton.y + addWordButton.height / 2 - addWordTextSize.y / 2},
               32.0f, 2, WHITE); // Set text color to white

    EndDrawing();
    return HOME;
}

/**
 * @brief Renders the search screen where users can search for words, view meanings,
 * suggestions, and recent searches.
 *
 * @param dictionary Pointer to the Trie dictionary.
 * @return true if the back button is pressed, false otherwise.
 */
bool DrawSearchScreen(TrieNode *dictionary)
{
    static std::deque<std::string> recentWords; // Stores up to 5 recent words

    // Load recent searches from "recent.txt" (only once at startup)
    if (recentWords.empty())
    {
        std::ifstream recentFile("recent.txt");
        std::string word;
        while (std::getline(recentFile, word, ','))
        {
            if (!word.empty())
                recentWords.push_back(word);
        }
        recentFile.close();
    }

    BeginDrawing();

    if (backgroundTexture.id > 0)
        DrawTexture(backgroundTexture, 0, 0, WHITE);
    else
        ClearBackground(RAYWHITE);

    DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 90});

    // ----- Back Button -----
    Rectangle backButton = {20, 20, 120, 50};
    Color backButtonColor = BLUE; // Set the button background color to blue
    if (CheckCollisionPointRec(GetMousePosition(), backButton))
    {
        backButtonColor = DARKBLUE; // Darker blue when hovered
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            searchText = "";
            displayedMeaning = "";
            EndDrawing();
            return true;
        }
    }
    DrawRectangleRec(backButton, backButtonColor);
    DrawRectangleLinesEx(backButton, 3, DARKBLUE);
    Vector2 backTextSize = MeasureTextEx(sRegularFont, "<- Back", 28.0f, 2);
    DrawTextEx(sRegularFont, "<- Back",
               (Vector2){backButton.x + backButton.width / 2 - backTextSize.x / 2,
                         backButton.y + backButton.height / 2 - backTextSize.y / 2},
               28.0f, 2, WHITE); // Set text color to white

    // ----- Search Bar -----
    Rectangle searchBar = {100, 120, screenWidth - 320, 60};
    DrawRectangleRec(searchBar, (Color){245, 245, 245, 255});
    DrawRectangleLinesEx(searchBar, 3, DARKBLUE);
    DrawTextEx(sRegularFont, searchText.c_str(),
               (Vector2){searchBar.x + 20, searchBar.y + 15},
               28.0f, 2, BLACK);

    // ----- Process Text Input -----
    int key = GetCharPressed();
    while (key > 0)
    {
        if (key >= 32 && key <= 125)
        {
            searchText += (char)key;
        }
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !searchText.empty())
    {
        searchText.pop_back();
        displayedMeaning = ""; // Clear displayed meaning when backspace is pressed.
    }

    // ----- Perform Search on ENTER Key Press -----
    if (IsKeyPressed(KEY_ENTER) && !searchText.empty())
    {
        std::string meaningFound = getMeaning(dictionary, searchText);
        if (!meaningFound.empty())
        {
            displayedMeaning = "Meaning: " + meaningFound;
        }
        else
        {
            displayedMeaning = "Word not found.";
        }

        // ---- Store Recent Searches -----
        if (std::find(recentWords.begin(), recentWords.end(), searchText) == recentWords.end())
        {
            if (recentWords.size() >= 5)
            {
                recentWords.pop_front();
            }
            recentWords.push_back(searchText);

            std::ofstream recentFile("recent.txt", std::ios::trunc);
            for (size_t i = 0; i < recentWords.size(); i++)
            {
                recentFile << recentWords[i];
                if (i < recentWords.size() - 1)
                    recentFile << ",";
            }
            recentFile.close();
        }
    }

    // ----- Compute Suggestions Based on Search Text -----
    std::vector<std::pair<std::string, std::string>> suggestions;
    if (!searchText.empty())
    {
        suggestions = getSuggestions(dictionary, searchText);
    }

    // ----- Draw Suggestions (Limit to 6) -----
    int suggestionY = 200;
    DrawTextEx(sRegularFont, "Suggestions:", (Vector2){100, (float)suggestionY}, 30.0f, 2, DARKBLUE);
    suggestionY += 50;
    int count = 0;
    for (const auto &suggestion : suggestions)
    {
        if (count >= 6)
            break;
        DrawTextEx(sRegularFont, suggestion.first.c_str(),
                   (Vector2){100, (float)suggestionY},
                   28.0f, 2, BLACK);
        suggestionY += 50;
        count++;
    }

    // ----- Display Meaning Panel (if available) -----
    if (!displayedMeaning.empty())
    {
        Rectangle meaningPanel = {100, suggestionY, screenWidth - 200, 150};
        DrawRectangleRec(meaningPanel, (Color){255, 255, 255, 220});
        DrawRectangleLinesEx(meaningPanel, 2, DARKBLUE);
        DrawTextEx(sRegularFont, displayedMeaning.c_str(),
                   (Vector2){meaningPanel.x + 10, meaningPanel.y + 10},
                   26.0f, 2, BLACK);
    }

    // ----- Display Recent Searches at Bottom -----
    if (!recentWords.empty())
    {
        std::string recentDisplay = "Recent: ";
        for (size_t i = 0; i < recentWords.size(); i++)
        {
            recentDisplay += recentWords[i];
            if (i < recentWords.size() - 1)
                recentDisplay += ", ";
        }

        int recentY = screenHeight - 70;
        int boxHeight = 50;
        int boxWidth = screenWidth - 200;

        Rectangle recentBox = {100, (float)recentY - 10, (float)boxWidth, (float)boxHeight};
        DrawRectangleRec(recentBox, (Color){230, 230, 230, 255});
        DrawRectangleLinesEx(recentBox, 2, DARKGRAY);

        DrawTextEx(sRegularFont, recentDisplay.c_str(),
                   (Vector2){120, (float)recentY}, 32.0f, 2, BLACK);
    }

    EndDrawing();
    return false;
}

/**
 * @brief Renders the Add Word screen where users can insert new words and their meanings.
 *
 * @param dictionary     Pointer to the Trie dictionary.
 * @param wordFilename   Filename for storing words.
 * @param meaningFilename Filename for storing meanings.
 * @return true if the back button is pressed, false otherwise.
 */
bool DrawAddWordScreen(TrieNode *dictionary, const std::string &wordFilename, const std::string &meaningFilename)
{
    static std::string wordInput = "";
    static std::string meaningInput = "";
    static int activeField = 0;

    BeginDrawing();

    if (backgroundTexture.id > 0)
        DrawTexture(backgroundTexture, 0, 0, WHITE);
    else
        ClearBackground(RAYWHITE);

    DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 90});

    // ----- Back Button -----
    Rectangle backButton = {20, 20, 120, 50};
    Color backButtonColor = BLUE; // Set the button background color to blue
    if (CheckCollisionPointRec(GetMousePosition(), backButton))
    {
        backButtonColor = DARKBLUE; // Darker blue when hovered
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            searchText = "";
            displayedMeaning = "";
            EndDrawing();
            return true;
        }
    }
    DrawRectangleRec(backButton, backButtonColor);
    DrawRectangleLinesEx(backButton, 3, DARKBLUE);
    Vector2 backTextSize = MeasureTextEx(sRegularFont, "<- Back", 28.0f, 2);
    DrawTextEx(sRegularFont, "<- Back",
               (Vector2){backButton.x + backButton.width / 2 - backTextSize.x / 2,
                         backButton.y + backButton.height / 2 - backTextSize.y / 2},
               28.0f, 2, WHITE); // Set text color to white

    // ----- Word Input Box -----
    Rectangle wordBox = {200, 120, screenWidth - 420, 60};
    Color wordBoxColor = (activeField == 1) ? LIGHTGRAY : (Color){245, 245, 245, 255};
    DrawRectangleRec(wordBox, wordBoxColor);
    DrawRectangleLinesEx(wordBox, 3, DARKBLUE);
    DrawTextEx(sRegularFont, "Word:",
               (Vector2){wordBox.x - 80, wordBox.y + 15}, 28.0f, 2, DARKBLUE);
    DrawTextEx(sRegularFont, wordInput.c_str(),
               (Vector2){wordBox.x + 20, wordBox.y + 15}, 28.0f, 2, BLACK);

    // ----- Meaning Input Box -----
    Rectangle meaningBox = {200, 200, screenWidth - 420, 60};
    Color meaningBoxColor = (activeField == 2) ? LIGHTGRAY : (Color){245, 245, 245, 255};
    DrawRectangleRec(meaningBox, meaningBoxColor);
    DrawRectangleLinesEx(meaningBox, 3, DARKBLUE);
    DrawTextEx(sRegularFont, "Meaning:",
               (Vector2){meaningBox.x - 110, meaningBox.y + 15}, 28.0f, 2, DARKBLUE);
    DrawTextEx(sRegularFont, meaningInput.c_str(),
               (Vector2){meaningBox.x + 20, meaningBox.y + 15}, 28.0f, 2, BLACK);

    // ----- Handle Mouse Clicks for Switching Active Field -----
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, wordBox))
            activeField = 1;
        else if (CheckCollisionPointRec(mousePos, meaningBox))
            activeField = 2;
        else
            activeField = 0;
    }

    // ----- Process Text Input for the Active Field -----
    int key = GetCharPressed();
    while (key > 0)
    {
        if (key >= 32 && key <= 125)
        {
            if (activeField == 1)
                wordInput.push_back((char)key);
            else if (activeField == 2)
                meaningInput.push_back((char)key);
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        if (activeField == 1 && !wordInput.empty())
            wordInput.pop_back();
        else if (activeField == 2 && !meaningInput.empty())
            meaningInput.pop_back();
    }

    // ----- Insert Button -----
    Rectangle insertButton = {100, 300, 150, 50};
    Color insertButtonColor = BLUE; // Changed to blue
    Color insertTextColor = WHITE;  // Changed to white

    if (CheckCollisionPointRec(GetMousePosition(), insertButton))
    {
        insertButtonColor = DARKBLUE; // Darker blue when hovered
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            insert(dictionary, wordInput, meaningInput);

            std::ofstream wordFile(wordFilename, std::ios::app);
            std::ofstream meaningFile(meaningFilename, std::ios::app);
            if (wordFile.is_open() && meaningFile.is_open())
            {
                wordFile << wordInput << "\n";
                meaningFile << meaningInput << "\n";
            }
            wordFile.close();
            meaningFile.close();

            wordInput.clear();
            meaningInput.clear();
        }
    }

    DrawRectangleRec(insertButton, insertButtonColor);
    DrawRectangleLinesEx(insertButton, 3, WHITE); // White border
    Vector2 insertTextSize = MeasureTextEx(sRegularFont, "Insert", 28.0f, 2);
    DrawTextEx(sRegularFont, "Insert",
               (Vector2){insertButton.x + insertButton.width / 2 - insertTextSize.x / 2,
                         insertButton.y + insertButton.height / 2 - insertTextSize.y / 2},
               28.0f, 2, insertTextColor); // White text

    EndDrawing();
    return false;
}
