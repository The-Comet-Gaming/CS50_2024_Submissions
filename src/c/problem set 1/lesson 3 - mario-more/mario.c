#include <cs50.h>
#include <stdio.h>

void print_block(int row);

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
        print_block(row);
        printf("  ");
        print_block(row);
        printf("\n");
    }
}

void print_block(int row)
{
    for (int column = 1; column <= row; column++)
    {
        printf("#");
    }
}
