#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
} candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int loopCount = 0; loopCount < candidate_count; ++loopCount) // Iterate over each candidate
    {
        if (strcmp(candidates[loopCount].name, name) == 0)
        {
            ++candidates[loopCount].votes;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    candidate toBeSwapped;
    for (int loopCount1 = 0; loopCount1 < candidate_count; ++loopCount1)
    {
        toBeSwapped.votes = candidates[loopCount1].votes;
        toBeSwapped.name = candidates[loopCount1].name;
        int indexSelected = loopCount1;
        bool higherFound = false;

        for (int loopCount2 = 0; loopCount2 < candidate_count; ++loopCount2)
        {
            if (loopCount2 <= indexSelected)
            {
                continue;
            }
            if (toBeSwapped.votes < candidates[loopCount2].votes)
            {
                toBeSwapped.votes = candidates[loopCount2].votes;
                toBeSwapped.name = candidates[loopCount2].name;
                indexSelected = loopCount2;
                higherFound = true;
            }
        }
        if (higherFound == true)
        {
            candidates[indexSelected].votes = candidates[loopCount1].votes;
            candidates[indexSelected].name = candidates[loopCount1].name;
            candidates[loopCount1].votes = toBeSwapped.votes;
            candidates[loopCount1].name = toBeSwapped.name;
        }
    }

    int highestVote = candidates[0].votes;

    for (int loopCount = 0; loopCount < candidate_count; ++loopCount)
    {
        if (candidates[loopCount].votes == highestVote)
        {
            printf("%s\n", candidates[loopCount].name);
        }
        else
        {
            break;
        }
    }

    return;
}
