#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int size;
    do
    {
        size = get_int("Size of Pyramid: ");
    }
    while (size < 1);

    for (int row = 1; row <= size; row++)
    {
        for (int spaces = size - row; spaces > 0; spaces--)
        {
            printf(" ");
        }

        for (int column = 1; column <= row; column++)
        {
            printf("#");
        }
        printf("\n");
    }
}
