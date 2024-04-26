#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    printf("Welcome to Scrabble! When prompted input your word.\n"); // welcome users to program and explain what they need to do

    string player_input[2]; //instantiate - string player_input[2];
    for (int i = 0; i < 2; i++) //Prompt for the user for two words
    {
        player_input[i] = get_string("Player %i: ", i+1); // get the user inputs and store them in the strings
    }

    int points[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
    int score[2] = {0, 0};
    int up_case_ascii[26];
    for (int i = 0; i < 26; ++i)
    {
        up_case_ascii[i] = 65 + i;
    }

    for (int a = 0; a < 2; ++a) // for loop for each player input
    {
        for (int b = 0; b < strlen(player_input[a]); ++b) // for loop for each char in the string
        {
            if (toupper(player_input[a][b]) >= up_case_ascii[0]
                && toupper(player_input[a][b]) <= up_case_ascii[25]) // if player_input[[a][b]] >= points[0] &&
            {                                                        // player_input[[0][0]] <= points[25]
                int ind_chng = up_case_ascii[25] - toupper(player_input[a][b]); // take var = point index[25] - player_input[a][b]
                score[a] = score[a] + points[25 - ind_chng];
            }
        }
    }

    if (score[0] > score[1]) // if score[0] > score[1] ; print Player 1 wins!
    {
        printf("Player 1 wins!\n");
    }
    if (score[0] < score[1]) // if score[0] < score[1] ; print Player 2 wins!
    {
        printf("Player 2 wins!\n");
    }
    else // if score[0] == score[1] ; print Tie!
    {
        printf("Tie!\n");
    }
}
