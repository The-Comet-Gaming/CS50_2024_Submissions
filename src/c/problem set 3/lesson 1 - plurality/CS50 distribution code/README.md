# Plurality
## Distribution Code
For this problem, you’ll extend the functionality of “distribution code” provided to you by CS50’s staff.

Whenever you’ll extend the functionality of existing code, it’s best to be sure you first understand it in its present state.

Look first at the top of the file. The line `#define MAX 9` is some syntax used here to mean that `MAX` is a constant (equal to `9`) that can be used throughout the program. Here, it represents the maximum number of candidates an election can have.
```
c
// Max number of candidates
#define MAX 9
```

Notice that `plurality.c` then uses this constant to define a global array—that is, an array that any function can access.
```
c
// Array of candidates
candidate candidates[MAX];
```

But what, in this case, is a `candidate`? In `plurality.c`, a `candidate` is a `struct`. Each `candidate` has two fields: a `string` called `name` representing the candidate’s name, and an `int` called `votes` representing the number of votes the candidate has.
```
c
// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;
```

Now, take a look at the `main` function itself. See if you can find where the program sets a global variable `candidate_count` representing the number of candidates in the election.
```
c
// Number of candidates
int candidate_count;
What about where it copies command-line arguments into the array candidates?

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
```

And where it asks the user to type in the number of voters?
```
c
int voter_count = get_int("Number of voters: ");
```

Then, the program lets every voter type in a vote, calling the `vote` function on each candidate voted for. Finally, `main` makes a call to the `print_winner` function to print out the winner (or winners) of the election. We’ll leave it to you to identify the code that implements this functionality.

If you look further down in the file, though, you’ll notice that the `vote` and `print_winner` functions have been left blank.
```
c
// Update vote totals given a new vote
bool vote(string name)
{
    // TODO
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // TODO
    return;
}
```

This part is up to you to complete! **You should not modify anything else in plurality.c other than the implementations of the vote and print_winner functions (and the inclusion of additional header files, if you’d like).**