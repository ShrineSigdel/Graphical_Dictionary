#include "UI.h"
#include "Trie.h" // Include trie functions and structure.
#include <string>
#include <vector>

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

void InitUI(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    InitWindow(screenWidth, screenHeight, "Dictionary App");

    // Disable ESC key from closing the window.
    SetExitKey(0);

    // Load custom fonts (using Google Fonts like OpenSans if available).
    sRegularFont = LoadFont("../assets/fonts/Regular.ttf");
    sItalicFont = LoadFont("../assets/fonts/Italic.ttf");

    // Load background and logo textures.
    backgroundTexture = LoadTexture("../assets/images/background.jpg");
    logoTexture = LoadTexture("../assets/images/logo.png");
}

void CloseUI()
{
    UnloadTexture(backgroundTexture);
    UnloadTexture(logoTexture);
    CloseWindow();
}

bool DrawHomeScreen()
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
               introFontSize, 2, GRAY);

    Rectangle searchButton = {screenWidth / 2 - 110, screenHeight / 2 - 50, 220, 70};
    Color buttonColor = GRAY;
    if (CheckCollisionPointRec(GetMousePosition(), searchButton))
    {
        buttonColor = LIGHTGRAY;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            displayedMeaning = "";
            EndDrawing();
            return true;
        }
    }
    DrawRectangleRec(searchButton, buttonColor);
    DrawRectangleLinesEx(searchButton, 3, DARKBLUE);
    Vector2 buttonTextSize = MeasureTextEx(sRegularFont, "Search", 32.0f, 2);
    DrawTextEx(sRegularFont, "Search",
               (Vector2){searchButton.x + searchButton.width / 2 - buttonTextSize.x / 2,
                         searchButton.y + searchButton.height / 2 - buttonTextSize.y / 2},
               32.0f, 2, BLACK);

    EndDrawing();
    return false;
}

// Modified: DrawSearchScreen now accepts a trie pointer instead of a vector of DictionaryEntry.
bool DrawSearchScreen(TrieNode *dictionary)
{
    BeginDrawing();

    if (backgroundTexture.id > 0)
        DrawTexture(backgroundTexture, 0, 0, WHITE);
    else
        ClearBackground(RAYWHITE);

    DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 90});

    // Back button.
    Rectangle backButton = {20, 20, 120, 50};
    Color backButtonColor = GRAY;
    if (CheckCollisionPointRec(GetMousePosition(), backButton))
    {
        backButtonColor = LIGHTGRAY;
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
    Vector2 backTextSize = MeasureTextEx(sRegularFont, "Back", 28.0f, 2);
    DrawTextEx(sRegularFont, "Back",
               (Vector2){backButton.x + backButton.width / 2 - backTextSize.x / 2,
                         backButton.y + backButton.height / 2 - backTextSize.y / 2},
               28.0f, 2, BLACK);

    // Search bar.
    Rectangle searchBar = {100, 120, screenWidth - 320, 60};
    DrawRectangleRec(searchBar, (Color){245, 245, 245, 255});
    DrawRectangleLinesEx(searchBar, 3, DARKBLUE);
    DrawTextEx(sRegularFont, searchText.c_str(),
               (Vector2){searchBar.x + 20, searchBar.y + 15},
               28.0f, 2, BLACK);

    // Process text input.
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

    // On ENTER, look up the word using the trie.
    if (IsKeyPressed(KEY_ENTER))
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
    }

    // Compute suggestions based on current search text.
    std::vector<std::pair<std::string, std::string>> suggestions;
    if (!searchText.empty())
    {
        suggestions = getSuggestions(dictionary, searchText);
    }

    // Draw suggestions.
    int suggestionY = 200;
    DrawTextEx(sRegularFont, "Suggestions:", (Vector2){100, (float)suggestionY}, 30.0f, 2, DARKBLUE);
    suggestionY += 50;
    for (const auto &suggestion : suggestions)
    {
        std::string suggestionLine = suggestion.first + " : " + suggestion.second;
        DrawTextEx(sRegularFont, suggestionLine.c_str(),
                   (Vector2){100, (float)suggestionY},
                   28.0f, 2, BLACK);
        suggestionY += 50;
    }

    // Draw the panel showing the displayed meaning (if available).
    if (!displayedMeaning.empty())
    {
        Rectangle meaningPanel = {100, suggestionY, screenWidth - 200, 150};
        DrawRectangleRec(meaningPanel, (Color){255, 255, 255, 220});
        DrawRectangleLinesEx(meaningPanel, 2, DARKBLUE);
        DrawTextEx(sRegularFont, displayedMeaning.c_str(),
                   (Vector2){meaningPanel.x + 10, meaningPanel.y + 10},
                   26.0f, 2, BLACK);
    }

    EndDrawing();
    return false;
}
