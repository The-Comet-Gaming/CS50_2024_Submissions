// Implements a dictionary's functionality

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;
unsigned int WordsLoaded = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // hash the word
    int hashNumber = hash(word);

    // search the hash node linked list on the table
    for (node *ptr = table[hashNumber]; ptr != NULL; ptr = ptr->next)
    {
        // check if string lengths are equal
        if (strlen(word) == strlen(ptr->word))
        {
            //
        }
        // convert to lower case before comparing
        // check if strings are equal
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // Open the dictionary file
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        //printf("Could not open %p.\n", source);
        return false;
    }

    char *word = NULL;

    // Read each word in the file
    while (fscanf(source, "%s", word) != EOF)
    {
        // Create space for a new hash table node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        WordsLoaded++;

        // Copy the word into the new node
        strcpy(n->word, word);
        n->next = NULL;

        // Hash the word to obtain its hash value
        int hashNumber = hash(word);

        // Insert the new node into the hash table (using the index specified by its hash value)
        // If the table is empty
        if (table[hashNumber] == NULL)
        {
            table[hashNumber] = n;
        }
        // If number belongs at beginning of list
        else if (strcmp(n->word, table[hashNumber]->word) < 0)
        {
            n->next = table[hashNumber];
            table[hashNumber] = n;
        }
        // If number belongs later in list
        else
        {
            for (node *ptr = table[hashNumber]; ptr != NULL; ptr = ptr->next)
            {
                // we need to check if the n word (oh dear) comes before table[N]->word
                // if it does then we need to insert the node into the chain
                // If at end of list
                if (ptr->next == NULL)
                {
                    // Append node
                    ptr->next = n;
                    break;
                }
                // If in middle of list
                if (strcmp(n->word, ptr->next->word) < 0)
                {
                    n->next = ptr->next;
                    ptr->next = n;
                    break;
                }
            }
        }
    }

    // Close the dictionary file
    fclose(source);
    return false;
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
    node *cursor = malloc(sizeof(node));
    if (cursor == NULL)
    {
        return false;
    }
    node *tmp = malloc(sizeof(node));
    if (tmp == NULL)
    {
        free(cursor);
        return false;
    }

    // iterate through the table
    for (int i = 0; i < N; i++)
    {
        // point cursor and tmp to the first node in the list of i
        cursor = table[i];
        tmp = table[i];

        // iterate through the linked list
        while (tmp != NULL)
        {
            cursor = table[i]->next;
            free(tmp);
            tmp = cursor;
        }
    }
    free(cursor);
    free(tmp);
    return true;
}
