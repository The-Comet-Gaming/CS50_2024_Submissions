#include <cs50.h>
#include <stdio.h>

void math(int);

int change;
int total_coins;
int count = 0;

int main(void)
{
    do
    {
        change = get_int("Change Owed in Cents: ");
    }
    while (change < 0);

    math(25);
    math(10);
    math(5);
    math(1);

    printf("Coins Given: %i\n", total_coins);
}

void math(int coin)
{
    if (change == 0)
    {
        return;
    }
    count = change / coin;
    total_coins = total_coins + count;
    change = change - coin * count;
}
