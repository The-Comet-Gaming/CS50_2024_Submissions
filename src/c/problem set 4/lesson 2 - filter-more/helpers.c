#include "helpers.h"
#include <math.h>

int SetWithinRange(double value);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
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

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
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

    int gridSize = 9;
    int Gx[gridSize];
    Gx[0] = -1;
    Gx[1] = 0;
    Gx[2] = 1;
    Gx[3] = -2;
    Gx[4] = 0;
    Gx[5] = 2;
    Gx[6] = -1;
    Gx[7] = 0;
    Gx[8] = 1;

    int Gy[gridSize];
    Gy[0] = -1;
    Gy[1] = -2;
    Gy[2] = -1;
    Gy[3] = 0;
    Gy[4] = 0;
    Gy[5] = 0;
    Gy[6] = 1;
    Gy[7] = 2;
    Gy[8] = 1;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            int rowOffset = -1;
            int pixelOffset = -1;

            long GxSumRed = 0;
            long GxSumGreen = 0;
            long GxSumBlue = 0;

            long GySumRed = 0;
            long GySumGreen = 0;
            long GySumBlue = 0;
            for (int k = 0; k < gridSize; ++k)
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
                GxSumRed += copyImage[i + rowOffset][j + pixelOffset].rgbtRed * Gx[k];
                GxSumGreen += copyImage[i + rowOffset][j + pixelOffset].rgbtGreen * Gx[k];
                GxSumBlue += copyImage[i + rowOffset][j + pixelOffset].rgbtBlue * Gx[k];

                GySumRed += copyImage[i + rowOffset][j + pixelOffset].rgbtRed * Gy[k];
                GySumGreen += copyImage[i + rowOffset][j + pixelOffset].rgbtGreen * Gy[k];
                GySumBlue += copyImage[i + rowOffset][j + pixelOffset].rgbtBlue * Gy[k];
            }
            double sumRed = GxSumRed * GxSumRed + GySumRed * GySumRed;
            double sumGreen = GxSumGreen * GxSumGreen + GySumGreen * GySumGreen;
            double sumBlue = GxSumBlue * GxSumBlue + GySumBlue * GySumBlue;

            image[i][j].rgbtRed = SetWithinRange(sumRed);
            image[i][j].rgbtGreen = SetWithinRange(sumGreen);
            image[i][j].rgbtBlue = SetWithinRange(sumBlue);
        }
    }
    return;
}

int SetWithinRange(double value)
{
    value = sqrt(value);
    value = round(value);
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
