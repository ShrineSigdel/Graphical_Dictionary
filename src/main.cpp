// #include "raylib.h"

// int main() {
//     // Initialize the window
//     const int screenWidth = 800;
//     const int screenHeight = 600;
//     InitWindow(screenWidth, screenHeight, "Graphical Dictionary");

//     SetTargetFPS(60);  // Set the frame rate

//     while (!WindowShouldClose()) {  // Keep running until user closes the window
//         BeginDrawing();
//         ClearBackground(RAYWHITE);

//         DrawText("Graphical Dictionary App", 200, 200, 20, DARKGRAY);

//         EndDrawing();
//     }

//     CloseWindow();  // Properly close the window
//     return 0;
// }

#include <iostream>
#include <string>
#include "Trie.h"

using namespace std;

int main()
{
    // Create the root of the Trie
    TrieNode *root = new TrieNode();

    // Load data from the files into the Trie
    load_data(root);

    // Menu for interaction with the Trie
    int choice;
    string word, meaning;

    while (true)
    {
        cout << "\nMenu:\n";
        cout << "1. Insert a new word\n";
        cout << "2. Search for a word\n";
        cout << "3. Delete a word\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1: // Insert a new word
            cout << "Enter the word to insert: ";
            cin >> word;
            cin.ignore(); // Ignore the newline character left in the input buffer
            cout << "Enter the meaning of " << word << ": ";
            getline(cin, meaning);
            insert(root, word, meaning);
            save_word(word, meaning); // Save the word to the file
            cout << "Word '" << word << "' inserted successfully!\n";
            break;

        case 2: // Search for a word
            cout << "Enter the word to search: ";
            cin >> word;
            if (!search(root, word))
            {
                cout << "Word not found.\n";
            }
            break;

        case 3: // Delete a word
            cout << "Enter the word to delete: ";
            cin >> word;
            if (deleteWord(root, word))
            {
                cout << "Word '" << word << "' deleted successfully!\n";
            }
            else
            {
                cout << "Word not found or cannot be deleted.\n";
            }
            break;

        case 4: // Exit
            cout << "Exiting program...\n";
            return 0;

        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}