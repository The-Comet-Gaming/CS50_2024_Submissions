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
void AssignPairs(int winner, int loser);
bool CycleFound(int winner, int loser);

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
    for (int i = 0; i < candidate_count; ++i)
    {
        for (int j = 0; j < candidate_count; ++j)
        {
            if (j <= i || preferences[i][j] == preferences[j][i])
            {
                continue;
            }
            if (preferences[i][j] > preferences[j][i])
            {
                AssignPairs(i, j);
            }
            else
            {
                AssignPairs(j, i);
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
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
    for (int i = 0; i < pair_count; ++i)
    {
        if (CycleFound(pairs[i].winner, pairs[i].loser) == true)
        {
            continue;
        }
        locked[pairs[i].winner][pairs[i].loser] = true;
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; ++i)
    {
        bool winnerFound = true;
        for (int j = 0; j < candidate_count; ++j)
        {
            if (locked[j][i] == true)
            {
                winnerFound = false;
                break;
            }
        }
        if (winnerFound == true)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

void AssignPairs(int winner, int loser)
{
    pairs[pair_count].winner = winner;
    pairs[pair_count].loser = loser;
    ++pair_count;
}

bool CycleFound(int winner, int loser)
{
    // checks a candidate against the field to find if they are
    // locked in under another candidate and checks if the candidate
    // they are locked in under matches the loser of the current pair
    for (int i = 0; i < candidate_count; ++i)
    {
        if (locked[i][winner] != true)
        {
            continue;
        }
        if (i == loser)
        {
            return true;
        }
        if (CycleFound(i, loser) == true)
        {
            return true;
        }
    }
    return false;
}
