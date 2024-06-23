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
const unsigned int N = 5393;

int WordsLoaded = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    for (node *ptr = table[hash(word)]; ptr != NULL; ptr = ptr->next)
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
    int hashNumber = 0;
    int n = strlen(word);
    for (int i = 0; i < n; i++)
    {
        if (word[i] >= 'A' && word[i] <= 'Z')
        {
            hashNumber += tolower(word[i]);
        }
        else
        {
            hashNumber += word[i];
        }
    }
    return hashNumber - 'a';
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
            fclose(source);
            return false;
        }

        WordsLoaded++;

        // Copy the word into the new node and set link pointer to NULL
        strcpy(n->word, loadedWord);
        // Hash the word to obtain its hash value
        int i = hash(n->word);
        n->next = table[i];
        table[i] = n;
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
