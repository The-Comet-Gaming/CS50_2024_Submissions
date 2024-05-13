#include "helpers.h"
#include <math.h>

float SetWithinRange(float value);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through each row in i, iterate through each pixel in j
    // Find the average of r g and b to apply to grayscale
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            float rgbAverage = (image[i][j].rgbtRed + image[i][j].rgbtGreen +
                                image[i][j].rgbtBlue) / 3.0;
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
            // Compute sepia values
            float sepiaRed = 0.393 * image[i][j].rgbtRed +
                                0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue;
            sepiaRed = round(sepiaRed);
            sepiaRed = SetWithinRange(sepiaRed);

            float sepiaGreen = 0.349 * image[i][j].rgbtRed +
                                0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue;
            sepiaGreen = round(sepiaGreen);
            SetWithinRange(sepiaGreen);

            float sepiaBlue = 0.272 * image[i][j].rgbtRed +
                                0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue;
            sepiaBlue = round(sepiaBlue);
            SetWithinRange(sepiaBlue);

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
    RGBTRIPLE buffer;
    int oppositeEndOfImage;
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j <= width / 2; ++j)
        {
            oppositeEndOfImage = width - j;

            buffer.rgbtRed = image[i][j].rgbtRed;
            buffer.rgbtGreen = image[i][j].rgbtGreen;
            buffer.rgbtBlue = image[i][j].rgbtBlue;

            image[i][j].rgbtRed = image[i][oppositeEndOfImage].rgbtRed;
            image[i][j].rgbtGreen = image[i][oppositeEndOfImage].rgbtGreen;
            image[i][j].rgbtBlue = image[i][oppositeEndOfImage].rgbtBlue;

            image[i][oppositeEndOfImage].rgbtRed = buffer.rgbtRed;
            image[i][oppositeEndOfImage].rgbtGreen = buffer.rgbtGreen;
            image[i][oppositeEndOfImage].rgbtBlue = buffer.rgbtBlue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through each row in i, iterate through each pixel in j
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

    RGBTRIPLE averageRGB;
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            int k = 0;
            int divider = 0;
            int rowOffset = -1;
            int pixelOffset = -1;

            averageRGB.rgbtRed = 0;
            averageRGB.rgbtGreen = 0;
            averageRGB.rgbtBlue = 0;

            while (k < 9)
            {
                if (i == 0 && rowOffset == -1)
                {
                    ++k;
                    continue;
                }
                if (i == height && rowOffset == +1)
                {
                    ++k;
                    continue;
                }

                if (j == 0 && pixelOffset == -1)
                {
                    ++k;
                    continue;
                }
                if (j == width && pixelOffset == +1)
                {
                    ++k;
                    continue;
                }

                averageRGB.rgbtRed += copyImage[i + rowOffset][j + pixelOffset].rgbtRed;
                averageRGB.rgbtGreen += copyImage[i + rowOffset][j + pixelOffset].rgbtGreen;
                averageRGB.rgbtBlue += copyImage[i + rowOffset][j + pixelOffset].rgbtBlue;

                if (k % 3 == 0)
                {
                    ++rowOffset;
                    pixelOffset = -1;
                }

                ++divider;
                ++pixelOffset;
                ++k;
            }
            image[i][j].rgbtRed = averageRGB.rgbtRed / divider;
            image[i][j].rgbtGreen = averageRGB.rgbtGreen / divider;
            image[i][j].rgbtBlue = averageRGB.rgbtBlue / divider;
        }
    }
    return;
}

float SetWithinRange(float value)
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
