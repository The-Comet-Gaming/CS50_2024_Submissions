// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

int WordsLoaded = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    /*char *lowerCaseWord = NULL;
    for (int i = 0; i < strlen(word); i++)
    {
        lowerCaseWord[i] = tolower(word[i]);
    }*/

    int n = hash(word);

    for (node *ptr = table[n]; ptr != NULL; ptr = ptr->next)
    {
        int i = strcasecmp(word, ptr->word);
        if (i < 0)
        {
            return false;
        }
        if (i == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
    // return word - 'a';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // Open the dictionary file
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        return false;
    }

    char loadedWord[LENGTH + 1];

    // Read each word in the file
    while (fscanf(source, "%s", loadedWord) != EOF)
    {
        // Create space for a new hash table node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        WordsLoaded++;

        // Copy the word into the new node and set link pointer to NULL
        strcpy(n->word, loadedWord);
        n->next = NULL;

        // Hash the word to obtain its hash value
        int i = hash(n->word);

        // if hash table[i] is empty
        if (table[i] == NULL)
        {
            table[i] = n;
        }
        else
        {
            for (node *ptr = table[i]; ptr != NULL; ptr = ptr->next)
            {
                // If at end of list
                if (ptr->next == NULL)
                {
                    // Append node
                    ptr->next = n;
                    break;
                }
            }
        }
    }
    // Close the dictionary file
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return WordsLoaded;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        for (node *tmp = table[i]; tmp != NULL; tmp = cursor)
        {
                cursor = cursor->next;
                free(tmp);
                tmp = cursor;
        }
    }
    return true;
}
