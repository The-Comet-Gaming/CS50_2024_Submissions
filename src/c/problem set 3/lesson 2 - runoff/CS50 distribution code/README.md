# Runoff
## Distribution Code
Whenever you’ll extend the functionality of existing code, it’s best to be sure you first understand it in its present state.

Look at the top of `runoff.c` first. Two constants are defined: `MAX_CANDIDATES` for the maximum number of candidates in the election, and `MAX_VOTERS` for the maximum number of voters in the election.
```
c
// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9
```

Notice that `MAX_CANDIDATES` is used to size an array, `candidates`.
```
c
// Array of candidates
candidate candidates[MAX_CANDIDATES];
```

`candidates` is an array of `candidates`. In `runoff.c` a `candidate` is a `struct`. Every `candidate` has a `string` field for their `name`, an `int` representing the number of `votes` they currently have, and a `bool` value called `eliminated` that indicates whether the candidate has been eliminated from the election. The array `candidates` will keep track of all of the candidates in the election.
```
c
// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;
```

Now you can better understand `preferences`, the two-dimensional array. The array `preferences[i]` will represent all of the preferences for voter number `i`. The integer, `preferences[i][j]`, will store the index of the candidate, from the `candidates` array, who is the `j`th preference for voter `i`.
```
c
// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];
```

The program also has two global variables: `voter_count` and `candidate_count`.
```
c
// Numbers of voters and candidates
int voter_count;
int candidate_count;
```

Now onto `main`. Notice that after determining the number of candidates and the number of voters, the main voting loop begins, giving every voter a chance to vote. As the voter enters their preferences, the `vote` function is called to keep track of all of the preferences. If at any point, the ballot is deemed to be invalid, the program exits.

Once all of the votes are in, another loop begins: this one’s going to keep looping through the runoff process of checking for a winner and eliminating the last place candidate until there is a winner.

The first call here is to a function called `tabulate`, which should look at all of the voters’ preferences and compute the current vote totals, by looking at each voter’s top choice candidate who hasn’t yet been eliminated. Next, the `print_winner` function should print out the winner if there is one; if there is, the program is over. But otherwise, the program needs to determine the fewest number of votes anyone still in the election received (via a call to `find_min`). If it turns out that everyone in the election is tied with the same number of votes (as determined by the `is_tie` function), the election is declared a tie; otherwise, the last-place candidate (or candidates) is eliminated from the election via a call to the `eliminate` function.

If you look a bit further down in the file, you’ll see that the rest of the functions—`vote`, `tabulate`, `print_winner`, `find_min`, `is_tie`, and `eliminate`—are all left to up to you to complete! **You should not modify anything else in runoff.c (and the inclusion of additional header files, if you’d like).**