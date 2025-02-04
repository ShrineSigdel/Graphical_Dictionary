#include "raylib.h"

int main() {
    // Initialize the window
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Graphical Dictionary");

    SetTargetFPS(60);  // Set the frame rate

    while (!WindowShouldClose()) {  // Keep running until user closes the window
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Graphical Dictionary App", 200, 200, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();  // Properly close the window
    return 0;
}
