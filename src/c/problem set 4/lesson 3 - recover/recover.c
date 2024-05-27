#include <stdio.h>
#include <stdlib.h>

const int BlockSize = 512;

int main(int numberOfArguments, char *cardName[])
{
    if (numberOfArguments != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    FILE *card = fopen(cardName[1], "r");
    if (card == NULL)
    {
        printf("Could not open %p.\n", card);
        return 1;
    }

    uint8_t buffer[BlockSize];

    // While there's still data left to read from the memory card
    while (fread(buffer, 1, BlockSize, card) == 512)
    {
        // Create JPEGs from the data

        // check if signiture bytes are correct / present
    }

    fclose(card);
    return 0;
}
