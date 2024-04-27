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
void print_winner(int voterCount);

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
    print_winner(voter_count);
}

// Update vote totals given a new vote
bool vote(string name)
{
    //printf("Input Being Checked: %s\n", name);
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
void print_winner(int voterCount)
{
    //int maxVotesNeeded = voterCount / candidate_count;
    //if (maxVotesNeeded < 1)
    //{
        //maxVotesNeeded = 1;
    //}
    //for (int loopCount = 0; loopCount < candidate_count; ++loopCount)
    //{
        //if (candidates[loopCount].votes == maxVotesNeeded)
        //{
            //printf("%s\n", candidates[loopCount].name);
        //}
    //}
    // TODO
    // take the number of voters and divide by the number of candidates
    // initialize an int of number of votes needed to win, take the number of voters and divide it by the number of candidates,
    // unless it is less than 1 that number is the max number of votes needed to win
    // loop through each candidate and print them off if they have the correct number of votes

    // have an int that is set to 1, being the minimum number of votes


    // function takes int argument (which is the number of voters)



    // algorithim  that goes through the candidates[].votes

        // for loop equal to candidate count, selects the candidate[1stLoop].vote that is being reassigned

            // intialize an int called currentHighestVote to equal 0;
            // intialize an int called lowerValueIndex to equal 0;
            // intialize an int called higherValueIndex to equal 0;

            // for loop again equal to candidate count, goes through the candidare[2ndLoop].vote indexes

                // if the index of the 1st loop is equal to the index of the 2nd loop then skip over the check

                // if the candidates[loop2nd].votes is > candidates[loop1st].votes

                    // set higherValueIndex = loop2nd;

        // it takes the value of candidates[].votes and compares it too currentHighestVote;

    // intialize an int that is the number of voters / 2, call it voteMajority

    return;
}
