#ifndef UI_H
#define UI_H

#include "raylib.h"
#include <vector>
#include <string>
#include "DictionaryEntry.h"

enum Screen {
    HOME,
    SEARCH
};

// Initialize the UI with a specified screen size.
void InitUI(int width, int height);

// Unload resources and close the window.
void CloseUI();

// Home screen: draws the home view and returns true if the "Search" button is clicked.
bool DrawHomeScreen();

// Search screen: draws the search view, updates the search text, shows suggestions,
// detects word search via a "Go" button, and displays the meaning.
// Returns true if the "Back" button is pressed.
bool DrawSearchScreen(const std::vector<DictionaryEntry> &dictionary);

#endif // UI_H
