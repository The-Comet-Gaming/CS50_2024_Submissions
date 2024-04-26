#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

double division(double input, double words, float multi);

int main(void)
{
    string input = get_string("Input passage of text for assessment: ");

    int asciiExclamationMark = 33;
    int asciiFullstop = 46;
    int asciiQuestionMark = 63;
    double letterCount = 0;
    double sentenceCount = 0;
    double wordCount = 1;

    for (int i = 0; i < strlen(input); ++i)
    {
        if (isalpha(input[i]) != 0)
        {
            ++letterCount;
            continue;
        }
        if (isspace(input[i]) != 0)
        {
            ++wordCount;
            continue;
        }
        if (input[i] == asciiFullstop || input[i] == asciiQuestionMark || input[i] == asciiExclamationMark)
        {
            ++sentenceCount;
        }
    }

    double letterDivision = division(letterCount, wordCount, 0.0588);
    double sentenceDivision = division(sentenceCount, wordCount, 0.296);
    double grade = round(letterDivision - sentenceDivision - 15.8); // double grade = func - func - 15.8);

    if (grade < 1) // if grade is < 1 than it needs to print before grade 1.
    {
        printf("Before Grade 1\n");
    }
    if (grade > 16) // if grade is > than 16 it needs to print as Grade 16+
    {
        printf("Grade 16+\n");
    }
    if (grade >= 1 && grade <= 16)
    {
        printf("Grade %i\n", (int) grade);
    }
}

double division(double input, double words, float multi)
{
    double div = input / words;
    double i = div * 100;
    return i = multi * i;
}
