// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // initializing a pointer to an address of a size equal to the BYTEs in the .WAV header
    uint8_t copiedHeader[HEADER_SIZE];
    fread(copiedHeader, HEADER_SIZE, 1, input);
    fwrite(copiedHeader, HEADER_SIZE, 1, output);

    // a variable for samples to be copied to and manipulated
    int16_t buffer;

    // Read single sample from input into buffer while there are samples left to read
    while (fread(&buffer, sizeof(int16_t), 1, input) != 0)
    {
        // multiple the int value of the sample by the factor
        buffer *= factor;

        // Write updated sample to the new file
        fwrite(&buffer, sizeof(int16_t), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
