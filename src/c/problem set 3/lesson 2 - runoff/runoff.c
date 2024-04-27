#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
} candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // Checking if the inputted name matches with any of the candidates
    for (int loopCount = 0; loopCount < candidate_count; ++loopCount)
    {
        if (strcmp(candidates[loopCount].name, name) == 0)
        {
            preferences[voter][rank] = loopCount;
            return true;
        }
    }

    // loop that runs a number of times equal to the candidates
    // if statement checking if any of the candidates.names matches the name input
    // if a match is found, then set preferences[voter][rank] to equal loopCount.
    // (The loop is to find a match by going through the candidates[loop].name)
    // (if a match is found then the preferences[voter][rank] is assigned the loop value
    // as it corresponds to the relevant candidate.)

    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // intialize all of the vote values in case this is a recount
    for (int loopCount = 0; loopCount < candidate_count; ++loopCount)
    {
        candidates[loopCount].votes = 0;
    }

    // incrementing votes +1 for each non-eliminated candidate
    for (int loopCount1 = 0; loopCount1 < voter_count; ++loopCount1)
    {
        for (int loopCount2 = 0; loopCount2 < candidate_count; ++loopCount2)
        {
            if (candidates[preferences[loopCount1][loopCount2]].eliminated == true)
            {
                continue;
            }
            ++candidates[preferences[loopCount1][loopCount2]].votes;
            break;
        }
    }

    // go through the voters preference in a for loop
    // for (int loopCount1 = 0; loopCount1 < voter_count; ++loopCount1)
        // for (int loopCount2 = 0; loopCount2 < candidate_count; ++loopCount2)
            // if (candidates[preferences[loopCount1][loopCount2]].eliminated == true)
                // continue;
            // ++candidates[preferences[loopCount1][loopCount2]].votes;
            // break;

    // need to set all of the .votes to 0 as the count may be repeating
    // and we do not want votes being stockpiled.

    // need to get the number value of a majority needed to win
    //need to take into account if the candidate has been eliminated
    // if candidate[loopCount].eliminated == false

    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // Calculating the value needed for a majority
    int majorityNeeded = voter_count / 2 + 1;
    if (majorityNeeded < 1)
    {
        majorityNeeded = 1;
    }

    // Checking and printing if a candidate has a majority
    for (int loopCount = 0; loopCount < candidate_count; ++loopCount)
    {
        if (candidates[loopCount].votes >= majorityNeeded)
        {
            printf("%s\n", candidates[loopCount].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // intializing the minimum votes found in case this is a recount
    int minimumVotes = voter_count;
    for (int loopCount = 0; loopCount < candidate_count; ++loopCount)
    {
        if (candidates[loopCount].eliminated == false)
        {
            if (candidates[loopCount].votes < minimumVotes)
            {
                minimumVotes = candidates[loopCount].votes;
            }
        }
    }
    return minimumVotes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // loop to check if all of the noneliminated candidates tie
    for (int loopCount = 0; loopCount < candidate_count; ++loopCount)
    {
        if (candidates[loopCount].eliminated == false)
        {
            if (candidates[loopCount].votes != min)
            {
                return false;
            }
        }
    }
    // loop to go through the candidates[].votes
    // if the candidate isn't eliminated
    // and if the candidates[].votes == min

    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // eliminates any uneliminated candidates with the lowest vote/s
    for (int loopCount = 0; loopCount < candidate_count; ++loopCount)
    {
        if (candidates[loopCount].eliminated == false && candidates[loopCount].votes == min)
        {
            candidates[loopCount].eliminated = true;
        }
    }
    return;
}
