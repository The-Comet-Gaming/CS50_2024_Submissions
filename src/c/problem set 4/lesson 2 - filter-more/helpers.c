#include "helpers.h"
#include <math.h>

int BinaryOffset = 1;

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
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width / 2; ++j)
        {
            int oppositeIndex = width - BinaryOffset - j;

            buffer = image[i][j];

            image[i][j] = image[i][oppositeIndex];

            image[i][oppositeIndex] = buffer;
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
            copyImage[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            float divider = 0.0;
            float averageRed = 0.0;
            float averageGreen = 0.0;
            float averageBlue = 0.0;
            for (int row = -1; row <= 1; row++)
            {
                int offsetRow = i + row;
                for (int column = -1; column <= 1; column++)
                {
                    int offsetColumn = j + column;
                    if (offsetRow >= 0 && offsetRow < height && offsetColumn >= 0 &&
                        offsetColumn < width)
                    {
                        divider++;
                        averageRed = averageRed + copyImage[offsetRow][offsetColumn].rgbtRed;
                        averageGreen = averageGreen + copyImage[offsetRow][offsetColumn].rgbtGreen;
                        averageBlue = averageBlue + copyImage[offsetRow][offsetColumn].rgbtBlue;
                    }
                }
            }

            image[i][j].rgbtRed = round(averageRed / divider);
            image[i][j].rgbtGreen = round(averageGreen / divider);
            image[i][j].rgbtBlue = round(averageBlue / divider);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copyImage[height][width];
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int gxSumRed = 0;
            int gxSumGreen = 0;
            int gxSumBlue = 0;

            int gySumRed = 0;
            int gySumGreen = 0;
            int gySumBlue = 0;

            for (int row = -1; row <= 1; row++)
            {
                for (int column = -1; column <= 1; column++)
                {
                    int neighbourRow = i + row;
                    int neighbourColumn = j + column;

                    if (neighbourRow >= 0 && neighbourRow < height && neighbourColumn >= 0 &&
                        neighbourColumn < width)
                    {
                        gxSumRed +=
                            image[neighbourRow][neighbourColumn].rgbtRed * gx[row + 1][column + 1];

                        gxSumGreen += image[neighbourRow][neighbourColumn].rgbtGreen *
                                      gx[row + 1][column + 1];

                        gxSumBlue +=
                            image[neighbourRow][neighbourColumn].rgbtBlue * gx[row + 1][column + 1];

                        gySumRed +=
                            image[neighbourRow][neighbourColumn].rgbtRed * gy[row + 1][column + 1];

                        gySumGreen += image[neighbourRow][neighbourColumn].rgbtGreen *
                                      gy[row + 1][column + 1];

                        gySumBlue +=
                            image[neighbourRow][neighbourColumn].rgbtBlue * gy[row + 1][column + 1];
                    }
                }
            }
            int sumRed = round(sqrt(gxSumRed * gxSumRed + gySumRed * gySumRed));
            int sumGreen = round(sqrt(gxSumGreen * gxSumGreen + gySumGreen * gySumGreen));
            int sumBlue = round(sqrt(gxSumBlue * gxSumBlue + gySumBlue * gySumBlue));

            copyImage[i][j].rgbtRed = sumRed > 255 ? 255 : sumRed;
            copyImage[i][j].rgbtGreen = sumGreen > 255 ? 255 : sumGreen;
            copyImage[i][j].rgbtBlue = sumBlue > 255 ? 255 : sumBlue;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copyImage[i][j];
        }
    }
    return;
}
