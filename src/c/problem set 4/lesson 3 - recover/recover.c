#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

    const int blockSize = 512;
    uint8_t buffer[blockSize];
    int header[5] = {0xff, 0xd8, 0xff, 0xf0, 0xe0};

    char jpgName[8] = "000.jpg";
    int jpgsWritten = 0;
    FILE *jpg;

    bool writingToFile = false;

    // loop while there is still more read from the card
    while (fread(buffer, 1, blockSize, card) == blockSize)
    {
        // check if the block has the headerBytes
        if (buffer[0] == header[0] && buffer[1] == header[1] && buffer[2] == header[2] &&
            (buffer[3] & header[3]) == header[4])
        {
            // close a jpg before writing to a new one
            if (writingToFile == true)
            {
                fclose(jpg);
            }

            // initialize the header name
            // increase a variable for the .jpg name +1
            sprintf(jpgName, "%03i.jpg", jpgsWritten);
            jpgsWritten++;

            // create a new .jpg
            jpg = fopen(jpgName, "a");

            // set bool that says if we're writing to a file as true
            writingToFile = true;
        }

        if (writingToFile == true)
        {
            fwrite(buffer, sizeof(buffer), 1, jpg);
        }
    }
    fclose(jpg);
    fclose(card);
    return 0;
}
