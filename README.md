# Graphical Dictionary App

This is a simple graphical dictionary application written in C++ using raylib. It demonstrates a basic trie data structure and loads oxford with their definitions from `oxford.txt` and `meaning.txt`.

## Features

- Loads dictionary oxford from external text files.
- Uses a trie data structure for quick word lookup.
- A minimal UI that accepts user input.
- Displays the definition of the entered word (if available).

## File Format

- **oxford.txt:** Each line contains a single word.
- **meaning.txt:** Each line contains the corresponding definition.

## Build Instructions

1. Ensure [raylib](https://github.com/raysan5/raylib) is installed.
2. Place `oxford.txt` and `meaning.txt` in the project root directory.
3. Create a build directory:
   ```bash
   mkdir build && cd build
   