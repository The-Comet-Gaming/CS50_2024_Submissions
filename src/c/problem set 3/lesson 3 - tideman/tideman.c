#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    //looping through the candidates checking for a match
    for (int i = 0; i < candidate_count; ++i)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    // looping through the preference ranks giving
    // +1 to the candidates prefered above others
    for (int i = 0; i < candidate_count; ++i)
    {
        for (int j = 0; j < candidate_count; ++j)
        {
            if (j <= i)
            {
                continue;
            }
            ++preferences[ranks[i]][ranks[j]];
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    // Looping over the candidate matchups and assigning
    // a winner and loser to each pairing that is not a
    // tie and hasn't already been paired
    for (int j = 0; j < candidate_count; ++j)
    {
        for (int k = 0; k < candidate_count; ++k)
        {
            if (k <= j) // skips over same and already paired indexes
            {
                continue;
            }
            if(preferences[j][k] > preferences[k][j])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = k;
                ++pair_count;
            }
            if(preferences[j][k] < preferences[k][j])
            {
                pairs[pair_count].winner = k;
                pairs[pair_count].loser = j;
                ++pair_count;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // selection sort loop for reordering the pairs
    for (int i = 0; i < pair_count; ++i)
    {
        bool matchFound = false;
        int highestFound = preferences[pairs[i].winner][pairs[i].loser];
        int index = i;
        int pairWinnerFound = pairs[i].winner;
        int pairLoserFound = pairs[i].loser;

        for (int j = 0; j < pair_count; ++j)
        {
            if (j < i)
            {
                continue;
            }
            if (highestFound < preferences[pairs[j].winner][pairs[j].loser])
            {
                highestFound = preferences[pairs[j].winner][pairs[j].loser];
                index = j;
                pairWinnerFound = pairs[j].winner;
                pairLoserFound = pairs[j].loser;
            }
        }
        pairs[index].winner = pairs[i].winner;
        pairs[index].loser = pairs[i].loser;

        pairs[i].winner = pairWinnerFound;
        pairs[i].loser = pairLoserFound;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count + 1; ++i)
    {
        int winnerLockingIn = pairs[i].winner;
        int loserGettingLocked = pairs[i].loser;

        bool cycleFound = false;
        int j = 0;
        while (j < candidate_count)
        {
            if (j == i)
            {
                continue;
            }
            if (locked[loserGettingLocked][j] == true)
            {
                loserGettingLocked = pairs[j].loser;
                if (loserGettingLocked == winnerLockingIn)
                {
                    cycleFound = true;
                    break;
                }
                j = 0;
            }
            else
            {
                ++j;
            }
        }

        if (cycleFound == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = true; // lock in here
        }
    }


    // start at the top of the pairs list
    // check if the loser is locked in over anyone
    // no? then lock in the edge
    // yes? then check if they are locked in over anyone
    // keep looping until you have either complete a full set of no lock ins, or find the find the winner from the start of the loop as a loser who has been locked in
    // in which case skip the lock in


    // I need to be able to check if a lock in would create a cycle
    // do a loop de loop
    // have an array

    // pair_count is the max number of locks that there can be

    // 1. intialise a loop of i < pair_count (3)
    // 2. store a variable of Alice's (0) candidate index
    // 3. intialise another loop of j < pair_count (3)
    // 3.1. store a variable of bob (1) who is being locked
    //      in under alice (0) so we need to check who bob is locked in against
    // 4. bob isn't locked in against anyone, so we set locked[0][1] to true



    // 1. make a variable to remember the current winner being locked in

    // 2. check the candidate that the edge is pointing too,
    // 3. do a loop checking if that candidate is locked
    //    into to any other candidate

    // should be a while loop with 2 possibly conditions
    // either a loop counter needs to be less than pair_count
    // or a boolean which is edgeFound

    // if a locked in edge is found, check if it locks


    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}
