#include "helpers.h"
#include <math.h>

float SetWithinRange(double value);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through each row in i, iterate through each pixel in j
    // Find the average of r g and b to apply to grayscale
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            float rgbAverage =
                (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0;
            rgbAverage = round(rgbAverage);

            image[i][j].rgbtRed = rgbAverage;
            image[i][j].rgbtGreen = rgbAverage;
            image[i][j].rgbtBlue = rgbAverage;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            float sepiaRed = 0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen +
                             0.189 * image[i][j].rgbtBlue;
            sepiaRed = round(sepiaRed);
            sepiaRed = SetWithinRange(sepiaRed);

            float sepiaGreen = 0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen +
                               0.168 * image[i][j].rgbtBlue;
            sepiaGreen = round(sepiaGreen);
            sepiaGreen = SetWithinRange(sepiaGreen);

            float sepiaBlue = 0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen +
                              0.131 * image[i][j].rgbtBlue;
            sepiaBlue = round(sepiaBlue);
            sepiaBlue = SetWithinRange(sepiaBlue);

            // Update pixel with sepia values
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //
    RGBTRIPLE buffer;
    int binaryOffset = 1;
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width / 2; ++j)
        {
            int oppositeIndex = width - binaryOffset - j;

            buffer.rgbtRed = image[i][j].rgbtRed;
            buffer.rgbtGreen = image[i][j].rgbtGreen;
            buffer.rgbtBlue = image[i][j].rgbtBlue;

            image[i][j].rgbtRed = image[i][oppositeIndex].rgbtRed;
            image[i][j].rgbtGreen = image[i][oppositeIndex].rgbtGreen;
            image[i][j].rgbtBlue = image[i][oppositeIndex].rgbtBlue;

            image[i][oppositeIndex].rgbtRed = buffer.rgbtRed;
            image[i][oppositeIndex].rgbtGreen = buffer.rgbtGreen;
            image[i][oppositeIndex].rgbtBlue = buffer.rgbtBlue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copyImage[height][width];
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            copyImage[i][j].rgbtRed = image[i][j].rgbtRed;
            copyImage[i][j].rgbtGreen = image[i][j].rgbtGreen;
            copyImage[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            float divider = 0.0;
            double averageRed = 0.0;
            double averageGreen = 0.0;
            double averageBlue = 0.0;
            int rowOffset = -1;
            int pixelOffset = -1;
            for (int k = 0; k < 9; ++k)
            {
                if (k == 3 || k == 6)
                {
                    ++rowOffset;
                    pixelOffset = -1;
                }

                if (i + rowOffset < 0 || i + rowOffset > height - 1)
                {
                    continue;
                }
                if (j + pixelOffset < 0 || j + pixelOffset > width - 1)
                {
                    ++pixelOffset;
                    continue;
                }

                ++divider;
                averageRed = averageRed + copyImage[i + rowOffset][j + pixelOffset].rgbtRed;
                averageGreen = averageGreen + copyImage[i + rowOffset][j + pixelOffset].rgbtGreen;
                averageBlue = averageBlue + copyImage[i + rowOffset][j + pixelOffset].rgbtBlue;
                ++pixelOffset;
            }
            averageRed /= divider;
            averageGreen /= divider;
            averageBlue /= divider;

            image[i][j].rgbtRed = round(averageRed);
            image[i][j].rgbtGreen = round(averageGreen);
            image[i][j].rgbtBlue = round(averageBlue);
        }
    }
    return;
}

float SetWithinRange(double value)
{
    if (value < 0)
    {
        return 0;
    }
    if (value > 255)
    {
        return 255;
    }
    return value;
}
