#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_substitute_correct(char* substitutionAlphabet); // prototypes
string make_cipher(char* substitutionAlphabet, string plaintext);

const int CorrectStringLength = 26;

int main(int numberOfArguments, char* substitutionAlphabet[]) // main function
{
    if (numberOfArguments != 2)
    {
        printf("Invalid Number of Commandlines.\n");
        return 1;
    }

    if (is_substitute_correct(substitutionAlphabet[1]) == false)
    {
        printf("Invalid Commandline.\n");
        return 1;
    }

    string plaintext = get_string("plaintext:  ");
    string ciphertext = make_cipher(substitutionAlphabet[1], plaintext);

    printf("ciphertext: %s\n", ciphertext);
    return 0;
}

// other functions
bool is_substitute_correct(char* substitutionAlphabet)
{
    int stringLength = strlen(substitutionAlphabet);
    if (stringLength == CorrectStringLength)
    {
        int characterCount[CorrectStringLength];
        int lowerAsciiStartNum = 97; // https://www.ascii-code.com/characters/a-z
        int lowerAsciiCodes[CorrectStringLength];
        for (int loop = 0; loop < stringLength; ++loop)
        {
            lowerAsciiCodes[loop] = lowerAsciiStartNum + loop;
            characterCount[loop] = 0;
        }

        for (int loopA = 0; loopA < stringLength; ++loopA)
        {
            if (isalpha(substitutionAlphabet[loopA]) != 0)
            {
                for (int loopB = 0; loopB < stringLength; ++loopB)
                {
                    if (tolower(substitutionAlphabet[loopA]) == lowerAsciiCodes[loopB])
                    {
                        ++characterCount[loopB];
                    }
                }
            }
            else
            {
                return false;
            }
        }

        for (int loop = 0; loop < stringLength; ++loop)
        {
            if (characterCount[loop] != 1)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

string make_cipher(char* substitutionAlphabet, string plaintext)
{
    int lowerAsciiStartNumber = 97; // https://www.ascii-code.com/characters/a-z
    int lowerAsciiCodes[CorrectStringLength];
    int stringLength = strlen(plaintext);
    string ciphertext = plaintext;

    for (int loop = 0; loop < CorrectStringLength; ++loop)
    {
        lowerAsciiCodes[loop] = lowerAsciiStartNumber + loop;
    }

    for (int loop = 0; loop < stringLength; ++loop)
    {
        if (isalpha(plaintext[loop]) != 0)
        {
            bool conditionMet = false;
            int bottomOfSearchRange = 0;
            int topOfSearchRange = CorrectStringLength;
            int searchedIndex = 0;
            int indexBinaryOffset = 1;
            int divider = 2;
            int dividedNumber = 0;

            while (conditionMet != true)
            {
                if (topOfSearchRange / divider > 0)
                {
                    dividedNumber = topOfSearchRange / divider;
                    searchedIndex = dividedNumber + bottomOfSearchRange - indexBinaryOffset;
                }
                else
                {
                    searchedIndex = 1 + bottomOfSearchRange - indexBinaryOffset;
                }

                if (tolower(plaintext[loop]) > lowerAsciiCodes[searchedIndex])
                {
                    bottomOfSearchRange = bottomOfSearchRange + dividedNumber;
                }
                if (tolower(plaintext[loop]) < lowerAsciiCodes[searchedIndex])
                {
                    topOfSearchRange = topOfSearchRange - dividedNumber;
                }
                if (tolower(plaintext[loop]) == lowerAsciiCodes[searchedIndex])
                {
                    if (islower(plaintext[loop]) != 0)
                    {
                        ciphertext[loop] = tolower(substitutionAlphabet[searchedIndex]);
                    }
                    else
                    {
                        ciphertext[loop] = toupper(substitutionAlphabet[searchedIndex]);
                    }
                    conditionMet = true;
                }
            }
        }
        else
        {
            ciphertext[loop] = plaintext[loop];
        }
    }
    return ciphertext;
}
