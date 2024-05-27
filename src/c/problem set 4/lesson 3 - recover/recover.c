#include <stdbool.h>
#include <stdint.h>
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

    char *jpgName = "###.jpg";
    int jpgsWritten = 0;
    bool writingToFile = false;
    FILE *jpg;

    // While there's still data left to read from the memory card
    while (fread(buffer, 1, BlockSize, card) == BlockSize)
    {
        // check if the first 4 bytes equal the correct value/s
        if (buffer [0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
                // check if we have already opened jpgs
                if (jpgsWritten >= 1)
                {
                    fclose(jpg);
                }

                // increase a variable for the .jpg name +1
                sprintf(jpgName, "%03i.jpg", jpgsWritten);
                jpgsWritten++;
                // create a new .jpg with the name
                jpg = fopen(jpgName, "w");
                // set bool that says if we're writing to a file as true
                writingToFile = true;
        }

        //check if we're already writing to a .jpg
        if (writingToFile == false)
        {
            continue;
        }

        //now that we've done the checks we can write to the current .jpg
        fwrite(buffer, sizeof(uint8_t), 1, jpg);
    }

    fclose(jpg);
    fclose(card);
    return 0;
}
