#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool digit_only(char* argv);
int key_assignment(int keyOffSet, int asciiRangeStart, int AsciiRangeEnd);

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Invalid Number of Commandlines.\n");
        return 1;
    }

    if (digit_only(argv[1]) != true)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    int k = atoi(argv[1]);
    if (k < 0)
    {
        printf("Invalid Key Number.\n");
        return 1;
    }

    int maxAsciiRange = 26;
    if (k > maxAsciiRange)
    {
        k = k - (k / maxAsciiRange * maxAsciiRange);
    }

    string plaintext = get_string("plaintext:  ");
    int textLen = strlen(plaintext); // initialize int textLength = strlen(plaintext);
    string ciphertext = plaintext; // initialize the length of cipher: ciphertext[textLength];

    int lowerAsciiRangeStart = 97; // https://www.ascii-code.com/characters/ascii-alphabet-characters
    int lowerAsciiRangeEnd = 122;
    int upperAsciiRangeStart = 65;
    int upperAsciiRangeEnd = 90;

    for (int i = 0; i < textLen; ++i)
    {
        int keyOffset = k + plaintext[i];
        if (islower(plaintext[i]) != 0)
        {
            ciphertext[i] = key_assignment(keyOffset, lowerAsciiRangeStart, lowerAsciiRangeEnd);
        }
        if (isupper(plaintext[i]) != 0)
        {
            ciphertext[i] = key_assignment(keyOffset, upperAsciiRangeStart, upperAsciiRangeEnd);
        }
        else
        {
            ciphertext[i] = plaintext[i];
        }
    }

    printf("ciphertext: %s\n", ciphertext);
    return 0;
}

bool digit_only(char* argv)
{
    int loopLen = strlen(argv);
    bool condition = false;
    for (int i = 0; i < loopLen; ++i)
    {
        if (isdigit(argv[i]) != 0)
        {
            condition = true;
        }
        else
        {
            condition = false;
        }
    }
    return condition;
}

int key_assignment(int keyOffset, int asciiRangeStart, int AsciiRangeEnd)
{
    int binaryOffset = 1;
    if (keyOffset > AsciiRangeEnd)
    {
        return keyOffset - AsciiRangeEnd - binaryOffset + asciiRangeStart;
    }
    else
    {
        return keyOffset;
    }
}
