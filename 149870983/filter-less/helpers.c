#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int a;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (!(image[i][j].rgbtBlue == image[i][j].rgbtGreen &&
                  image[i][j].rgbtBlue == image[i][j].rgbtRed))
            {
                a = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) /
                          3.0);
                image[i][j].rgbtBlue = a;
                image[i][j].rgbtGreen = a;
                image[i][j].rgbtRed = a;
            }
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sepiaRed = round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen +
                                 0.189 * image[i][j].rgbtBlue);
            int sepiaGreen = round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen +
                                   0.168 * image[i][j].rgbtBlue);
            int sepiaBlue = round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen +
                                  0.131 * image[i][j].rgbtBlue);

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
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
    int a;
    int b;
    int c;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            a = image[i][j].rgbtBlue;
            b = image[i][j].rgbtRed;
            c = image[i][j].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][width - j - 1].rgbtBlue = a;
            image[i][width - j - 1].rgbtRed = b;
            image[i][width - j - 1].rgbtGreen = c;
        }
    }
    return;
}
// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int b;
    int g;
    int r;
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0)
            {
                if (j == 0)
                {
                    b = round((temp[i][j].rgbtBlue + temp[i][j + 1].rgbtBlue +
                               temp[i + 1][j].rgbtBlue + temp[i + 1][j + 1].rgbtBlue) /
                              4.0);
                    g = round((temp[i][j].rgbtGreen + temp[i][j + 1].rgbtGreen +
                               temp[i + 1][j].rgbtGreen + temp[i + 1][j + 1].rgbtGreen) /
                              4.0);
                    r = round((temp[i][j].rgbtRed + temp[i][j + 1].rgbtRed +
                               temp[i + 1][j].rgbtRed + temp[i + 1][j + 1].rgbtRed) /
                              4.0);
                    image[i][j].rgbtBlue = b;
                    image[i][j].rgbtGreen = g;
                    image[i][j].rgbtRed = r;
                }
                else if (j == width - 1)
                {
                    b = round((temp[i][j].rgbtBlue + temp[i][j - 1].rgbtBlue +
                               temp[i + 1][j].rgbtBlue + temp[i + 1][j - 1].rgbtBlue) /
                              4.0);
                    g = round((temp[i][j].rgbtGreen + temp[i][j - 1].rgbtGreen +
                               temp[i + 1][j].rgbtGreen + temp[i + 1][j - 1].rgbtGreen) /
                              4.0);
                    r = round((temp[i][j].rgbtRed + temp[i][j - 1].rgbtRed +
                               temp[i + 1][j].rgbtRed + temp[i + 1][j - 1].rgbtRed) /
                              4.0);
                    image[i][j].rgbtBlue = b;
                    image[i][j].rgbtGreen = g;
                    image[i][j].rgbtRed = r;
                }
                else
                {
                    b = round((temp[i][j].rgbtBlue + temp[i][j - 1].rgbtBlue +
                               temp[i][j + 1].rgbtBlue + temp[i + 1][j].rgbtBlue +
                               temp[i + 1][j - 1].rgbtBlue + temp[i + 1][j + 1].rgbtBlue) /
                              6.0);
                    g = round((temp[i][j].rgbtGreen + temp[i][j - 1].rgbtGreen +
                               temp[i][j + 1].rgbtGreen + temp[i + 1][j].rgbtGreen +
                               temp[i + 1][j - 1].rgbtGreen + temp[i + 1][j + 1].rgbtGreen) /
                              6.0);
                    r = round((temp[i][j].rgbtRed + temp[i][j - 1].rgbtRed +
                               temp[i][j + 1].rgbtRed + temp[i + 1][j].rgbtRed +
                               temp[i + 1][j - 1].rgbtRed + temp[i + 1][j + 1].rgbtRed) /
                              6.0);
                    image[i][j].rgbtBlue = b;
                    image[i][j].rgbtGreen = g;
                    image[i][j].rgbtRed = r;
                }
            }
            else if (i == height - 1)
            {
                if (j == 0)
                {
                    b = round((temp[i][j].rgbtBlue + temp[i][j + 1].rgbtBlue +
                               temp[i - 1][j].rgbtBlue + temp[i - 1][j + 1].rgbtBlue) /
                              4.0);
                    g = round((temp[i][j].rgbtGreen + temp[i][j + 1].rgbtGreen +
                               temp[i - 1][j].rgbtGreen + temp[i - 1][j + 1].rgbtGreen) /
                              4.0);
                    r = round((temp[i][j].rgbtRed + temp[i][j + 1].rgbtRed +
                               temp[i - 1][j].rgbtRed + temp[i - 1][j + 1].rgbtRed) /
                              4.0);
                    image[i][j].rgbtBlue = b;
                    image[i][j].rgbtGreen = g;
                    image[i][j].rgbtRed = r;
                }
                else if (j == width - 1)
                {
                    b = round((temp[i][j].rgbtBlue + temp[i][j - 1].rgbtBlue +
                               temp[i - 1][j].rgbtBlue + temp[i - 1][j - 1].rgbtBlue) /
                              4.0);
                    g = round((temp[i][j].rgbtGreen + temp[i][j - 1].rgbtGreen +
                               temp[i - 1][j].rgbtGreen + temp[i - 1][j - 1].rgbtGreen) /
                              4.0);
                    r = round((temp[i][j].rgbtRed + temp[i][j - 1].rgbtRed +
                               temp[i - 1][j].rgbtRed + temp[i - 1][j - 1].rgbtRed) /
                              4.0);
                    image[i][j].rgbtBlue = b;
                    image[i][j].rgbtGreen = g;
                    image[i][j].rgbtRed = r;
                }
                else
                {
                    b = round((temp[i][j].rgbtBlue + temp[i][j - 1].rgbtBlue +
                               temp[i][j + 1].rgbtBlue + temp[i - 1][j].rgbtBlue +
                               temp[i - 1][j - 1].rgbtBlue + temp[i - 1][j + 1].rgbtBlue) /
                              6.0);
                    g = round((temp[i][j].rgbtGreen + temp[i][j - 1].rgbtGreen +
                               temp[i][j + 1].rgbtGreen + temp[i - 1][j].rgbtGreen +
                               temp[i - 1][j - 1].rgbtGreen + temp[i - 1][j + 1].rgbtGreen) /
                              6.0);
                    r = round((temp[i][j].rgbtRed + temp[i][j - 1].rgbtRed +
                               temp[i][j + 1].rgbtRed + temp[i - 1][j].rgbtRed +
                               temp[i - 1][j - 1].rgbtRed + temp[i - 1][j + 1].rgbtRed) /
                              6.0);
                    image[i][j].rgbtBlue = b;
                    image[i][j].rgbtGreen = g;
                    image[i][j].rgbtRed = r;
                }
            }
            else if (j == 0)
            {
                b = round((temp[i][j].rgbtBlue + temp[i - 1][j].rgbtBlue + temp[i + 1][j].rgbtBlue +
                           temp[i][j + 1].rgbtBlue + temp[i - 1][j + 1].rgbtBlue +
                           temp[i + 1][j + 1].rgbtBlue) /
                          6.0);
                r = round((temp[i][j].rgbtRed + temp[i - 1][j].rgbtRed + temp[i + 1][j].rgbtRed +
                           temp[i][j + 1].rgbtRed + temp[i - 1][j + 1].rgbtRed +
                           temp[i + 1][j + 1].rgbtRed) /
                          6.0);
                g = round((temp[i][j].rgbtGreen + temp[i - 1][j].rgbtGreen +
                           temp[i + 1][j].rgbtGreen + temp[i][j + 1].rgbtGreen +
                           temp[i - 1][j + 1].rgbtGreen + temp[i + 1][j + 1].rgbtGreen) /
                          6.0);
            }
            else if (j == width - 1)
            {
                b = round((temp[i][j].rgbtBlue + temp[i - 1][j].rgbtBlue + temp[i + 1][j].rgbtBlue +
                           temp[i][j - 1].rgbtBlue + temp[i - 1][j - 1].rgbtBlue +
                           temp[i + 1][j - 1].rgbtBlue) /
                          6.0);
                r = round((temp[i][j].rgbtRed + temp[i - 1][j].rgbtRed + temp[i + 1][j].rgbtRed +
                           temp[i][j - 1].rgbtRed + temp[i - 1][j - 1].rgbtRed +
                           temp[i + 1][j - 1].rgbtRed) /
                          6.0);
                g = round((temp[i][j].rgbtGreen + temp[i - 1][j].rgbtGreen +
                           temp[i + 1][j].rgbtGreen + temp[i][j - 1].rgbtGreen +
                           temp[i - 1][j - 1].rgbtGreen + temp[i + 1][j - 1].rgbtGreen) /
                          6.0);
            }
            else
            {
                b = round((temp[i][j].rgbtBlue + temp[i][j - 1].rgbtBlue + temp[i][j + 1].rgbtBlue +
                           temp[i - 1][j].rgbtBlue + temp[i - 1][j - 1].rgbtBlue +
                           temp[i - 1][j + 1].rgbtBlue + temp[i + 1][j].rgbtBlue +
                           temp[i + 1][j - 1].rgbtBlue + temp[i + 1][j + 1].rgbtBlue) /
                          9.0);
                g = round((temp[i][j].rgbtGreen + temp[i][j - 1].rgbtGreen +
                           temp[i][j + 1].rgbtGreen + temp[i - 1][j].rgbtGreen +
                           temp[i - 1][j - 1].rgbtGreen + temp[i - 1][j + 1].rgbtGreen +
                           temp[i + 1][j].rgbtGreen + temp[i + 1][j - 1].rgbtGreen +
                           temp[i + 1][j + 1].rgbtGreen) /
                          9.0);
                r = round((temp[i][j].rgbtRed + temp[i][j - 1].rgbtRed + temp[i][j + 1].rgbtRed +
                           temp[i - 1][j].rgbtRed + temp[i - 1][j - 1].rgbtRed +
                           temp[i - 1][j + 1].rgbtRed + temp[i + 1][j].rgbtRed +
                           temp[i + 1][j - 1].rgbtRed + temp[i + 1][j + 1].rgbtRed) /
                          9.0);
                image[i][j].rgbtBlue = b;
                image[i][j].rgbtGreen = g;
                image[i][j].rgbtRed = r;
            }
        }
    }
    return;
}
