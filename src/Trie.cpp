#include <cstdio>
#include <cctype>
#include <algorithm>
#include <cstring>
#include <thread>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Trie.h"  // Ensure this header defines your trie functions and globals

#define max_recentoxford 5

// Function prototypes
void display_menu();
void search_word(TrieNode *);
void add_word(TrieNode *, std::string word = "0");
void print_recentoxford(TrieNode *, std::string word = "0");
TrieNode *delete_word(TrieNode *);

bool isValidWord(std::string);

int main()
{
    // Create the root node for the trie.
    TrieNode *root = getNode();

    // Open the files for oxford and meaning.
    std::ifstream in1("oxford.txt"), in2("meaning.txt");
    std::string s1, s2;
    // Read oxford and meaning until end-of-file.
    while(getline(in1, s1) && getline(in2, s2))
    {
        insert(root, s1, s2);
    }
    in1.close();
    in2.close();

    int choice = 0;
    // Main loop until user chooses to exit.
    while(choice != 5)
    {
        system("clear");
        std::cout << "\t\t\t\t\tWelcome to Dictionary!\n\n";
        display_menu();
        std::cin >> choice;
        std::cin.clear();
        std::cin.ignore(2147483647, '\n');

        switch(choice)
        {
            case 1:
                search_word(root);
                break;
            case 2:
                add_word(root);
                break;
            case 3:
                root = delete_word(root);
                break;
            case 4:
                print_recentoxford(root);
                break;
            case 5:
                std::cout << "Thank you for using this application!\nSee you next time!";
                break;
            default:
                std::cout << "Please enter a valid option!\n";
                std::cout << "Press Enter to continue";
                getchar();
        }
    }
    return 0;
}

void display_menu(){
    std::cout << "\t\t\t\t\t      Main Menu\n";
    std::cout << "1. Search a word.\n";
    std::cout << "2. Add a word.\n";
    std::cout << "3. Delete a word.\n";
    std::cout << "4. Search History.\n";
    std::cout << "5. Exit the application.\n";
    std::cout << "Enter your choice: ";
}

void search_word(TrieNode *root)
{
    system("clear");
    std::cout << "\t\t\t\t\t    Search a word!\n";
    std::cout << "Enter word to be searched: ";
    std::string key;
    std::cin >> key;
    std::cin.clear();
    std::cin.ignore(2147483647, '\n');

    // Convert the word to lowercase.
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);

    if(isValidWord(key))
    {
        static int numRecent = 1;
        char **recent = (char**)malloc(max_recentoxford * sizeof(char*));

        if (numRecent <= max_recentoxford)
        {
            FILE* fp = fopen("recent.txt", "a");
            const char* recent_str = key.c_str();
            recent[numRecent - 1] = (char*)malloc((strlen(recent_str) + 1) * sizeof(char));
            strcpy(recent[numRecent - 1], recent_str);
            fprintf(fp, "%s\n", recent[numRecent - 1]);
            fclose(fp);
        }
        
        // Call your trie search and suggestion routine.
        search_and_suggest(root, key);

        if(count == 0)
        {
            std::cout << "Sorry, no suggestions found!\n";
        }
        
        char c = 'n';
        if(wordfound == false)
        {
            std::cout << "\nThink we missed this word?\n";
            std::cout << "Would you like to add this word to the dictionary along with its meaning? (y/n) ";
            std::cin >> c;
            std::cin.clear();
            std::cin.ignore(2147483647, '\n');
            if(c == 'y' || c == 'Y')
                add_word(root, key);
        }
        
        if(c == 'n' || c == 'N' || wordfound == true){
            std::cout << "Would you like to search another word? (y/n) ";
            std::cin >> c;
            std::cin.clear();
            std::cin.ignore(2147483647, '\n');
            
            if(c == 'y'){
                searching = true;
                wordfound = true;
                count = 0;
                search_word(root);
            }
        }

        // Reset flags for next search.
        searching = true;
        wordfound = true;
        count = 0;
    }
    else
    {
        std::cout << "Error! Word should only contain letters from the English alphabet!\n";
        std::cout << "Press Enter to continue";
        getchar();
        search_word(root);
    }
}

void add_word(TrieNode *root, std::string word)
{
    if(word == "0")
        system("clear");

    std::ofstream out1;
    std::ofstream out2;

    out1.open("../oxford.txt", std::ios_base::app);
    out2.open("../meaning.txt", std::ios_base::app);
    
    std::cout << "\t\t\t\t    Add a new word to the dictionary!\n";
    
    // If no word was provided, prompt the user.
    if(word == "0")
    {
        std::cout << "Enter the word to be added: ";
        std::cin >> word;
        std::cin.clear();
        std::cin.ignore(2147483647, '\n');
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    }

    if(isValidWord(word))
    {
        out1 << word << "\n";
        std::cout << "Enter the meaning of " << word << ": ";
        std::string meaning;
        std::getline(std::cin, meaning);
        out2 << meaning << "\n";
        insert(root, word, meaning);
    
        out1.close();
        out2.close();

        std::cout << "\nWord added Successfully!\n";
        std::cout << "Press Enter to continue";
        getchar();
    }
    else
    {
        std::cout << "Error! Word should only contain letters from the English alphabet!\n";
        std::cout << "Press Enter to continue";
        getchar();
        add_word(root);
    }
}

TrieNode *delete_word(TrieNode *root)
{ 
    system("clear");
    std::cout << "\t\t\t\t    Delete a word from the dictionary\n";
    std::cout << "Enter the word to be deleted: ";
    std::string key;
    std::cin >> key;
    std::cin.clear();
    std::cin.ignore(2147483647, '\n');

    // Convert to lowercase.
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);

    if(isValidWord(key))
    {   
        wordfound = false;
        root = remove(root, key);
        
        if(wordfound)
        {
            // Update oxford.txt by copying all oxford except the one to delete.
            std::ofstream temp("tempoxford.txt");
            std::ifstream fin("oxford.txt");
            std::string s;
            int pos = 0;
            bool found = false;

            while(std::getline(fin, s))
            {
                if(s == key)
                    found = true;
                else
                {
                    temp << s << "\n";
                    if(!found)
                        pos++;
                }
            }
            fin.close();
            temp.close();

            remove("oxford.txt");
            rename("tempoxford.txt", "oxford.txt");

            // Update meaning.txt similarly.
            std::ofstream temp2("tempmeaning.txt");
            std::ifstream fin2("meaning.txt");

            while(std::getline(fin2, s))
            {
                if(pos == 0)
                {
                    pos--;
                    continue;
                }
                temp2 << s << "\n";
                pos--;
            }
            fin2.close();
            temp2.close();

            remove("meaning.txt");
            rename("tempmeaning.txt", "meaning.txt");
        }
        
        std::cout << "Press Enter to continue";
        getchar();
        wordfound = true;
    }
    else
    {
        std::cout << "Error! Word should only contain letters from the English alphabet!\n";
        std::cout << "Press Enter to continue";
        getchar();
        delete_word(root);
    }
    
    return root;
}

bool isValidWord(std::string key)
{
    for (size_t i = 0; i < key.size(); i++)
    {
        if (!isalpha(key[i]))
            return false;
    }
    return true;
}

void print_recentoxford(TrieNode *root, std::string word)
{
    if(word == "0")
         system("clear");
    
    const int MAX_WORD_LENGTH = 100;
    FILE *fp = fopen("recent.txt", "r");
    if (fp == NULL) 
    {
        std::cout << "Failed to open file\n";
        exit(1);
    }

    char str[MAX_WORD_LENGTH];
    char *oxford[max_recentoxford];
    int count = 0;
    while (fscanf(fp, "%s", str) == 1) 
    {
        if (count < max_recentoxford)
        {
            oxford[count] = strdup(str);
            count++;
        }
        else
        {
            free(oxford[0]);
            for (int i = 0; i < max_recentoxford - 1; i++) 
            {
                oxford[i] = oxford[i + 1];
            }
            oxford[max_recentoxford - 1] = strdup(str);
        }
    }
    fclose(fp);

    std::cout << "Recently Searched oxford:\n\n";
    for (int i = count - 1; i >= 0; i--) 
    {
        std::cout << oxford[i] << "\n";
    }

    for (int i = 0; i < count; i++) 
    {
        free(oxford[i]);
    }
    std::cout << "\n\nPress Enter key to continue...\n";
    getchar();
}
