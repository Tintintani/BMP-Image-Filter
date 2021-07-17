#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "datatype.h" // Conatins User Defined Data Types

void swap(RGB *a, RGB *b);

void grayscale(int height, int width, RGB image[height][width]);
void reflection(int height, int width, RGB image[height][width]);
void sepia(int height, int width, RGB image[height][width]);
void blur(int height, int width, RGB image[height][width]);
void edge(int height, int width, RGB image[height][width]);

int main(void)
{
    printf("\n\t\tBMP Image Filter\n");
    printf("\n========================================================\n\n");

    char inputFileName[50], outputFileName[50];
    printf("Enter Input File Name with Location: \n");
    scanf("%s", inputFileName); // Image Name and Location

    if (inputFileName == NULL)
    {
        printf("\nError Loading Image... Invalid Name... Exiting...\n\n"); // Invalid Name
        printf("========================================================\n");
        return 1;
    }

    // Opening Input File in Read Mode in Binary
    FILE *input = fopen(inputFileName, "rb"); // Opening Image
    if (input == NULL)
    {
        printf("\nError Opening Image. Invalid Name or Not Enough Memory. Exiting...\n\n"); // Not Enough Memory
        printf("========================================================\n");
        fclose(input);
        return 2;
    }

    BMPFileHeader bfh;
    fread(&bfh, sizeof(BMPFileHeader), 1, input); // Reading BMP Image FileHeader - 14Bytes

    BMPInfoHeader bih;
    fread(&bih, sizeof(BMPInfoHeader), 1, input); // Reading BMP Image InfoHeader - 40Bytes

    if (bfh.BFHSIGN != 0x4D42 || bih.COLOURDEPTH != 24) // Condition for a 24 Bit BMP Iamge
    {
        printf("\nInvalid. Image is not a 24 Bit BMP. Exiting...\n\n"); // Image is not BMP or Invlaid
        printf("========================================================\n");
        fclose(input);
        return 3;
    }

    printf("\nEnter Output Image Name: \n"); // Output Image Name and Location
    scanf("%s", outputFileName);

    if (outputFileName == NULL)
    {
        printf("\nError creating Output Image... Invalid Name... Exiting...\n\n"); // Invalid Name
        printf("========================================================\n");
        fclose(input);
        return 4;
    }

    // Opening Output File in Write Mode in Binary
    FILE *output = fopen(outputFileName, "wb");
    if (output == NULL)
    {
        printf("\nError creating Output File. Exiting...\n\n"); // Error Creating Output Image
        printf("========================================================\n");
        fclose(output);
        fclose(input);
        remove(outputFileName);
        return 5;
    }

    // Getting Height and Width
    int width = bih.WIDTH;
    int height = abs(bih.HEIGHT);

    // Printing Image Details
    printf("\n------------------------------------------------------\n");
    printf("Image Details\n");
    printf("Name and Location : %s\n", inputFileName);
    printf("Height            : %d\n", height);
    printf("Width             : %d\n", width);
    printf("Size              : %.2f KB\n", (float)bfh.BFHSIZE / 1024);
    printf("Colour Depth      : %d\n", bih.COLOURDEPTH);
    printf("Compression       : %d\n", bih.BIHCOMP);
    printf("--------------------------------------------------------\n");

    RGB(*imagedata)
    [width] = calloc(height, width * sizeof(RGB)); // Allocating memory for imagedata

    if (imagedata == NULL)
    {
        printf("\nError Creating Image. Not Enough Memory. Exiting...\n"); // Error Creating Output Image
        printf("========================================================\n");
        fclose(output);
        fclose(input);
        remove(outputFileName);
        return 6;
    }

    int padding = (4 - (width * sizeof(RGB)) % 4) % 4; // Padding

    fseek(input, bfh.BFHIMGSTRT, SEEK_SET); // Seeking to Postion Where Image Data Starts

    for (int i = 0; i < height; i++)
    {
        fread(imagedata + i, sizeof(RGB), width, input); // Reading Image Data 24Bits at a time for Width times
        fseek(input, padding, SEEK_CUR);                 // Skipping Padding for each row
    }

    // Choosing Filter
    printf("\nChoose a Filter:\nG: Grayscale\nS: Sepia\nR: Reflection\nB: Blur\nE: Edge Detection\n");
    char filter;
    printf("Filter: ");
    scanf("%*c%c", &filter);

    switch (filter)
    {
    case 'g':
    case 'G':
        grayscale(height, width, imagedata);
        break;
    case 's':
    case 'S':
        sepia(height, width, imagedata);
        break;
    case 'r':
    case 'R':
        reflection(height, width, imagedata);
        break;
    case 'b':
    case 'B':
        blur(height, width, imagedata);
        break;
    case 'e':
    case 'E':
        edge(height, width, imagedata);
        break;

    default:
        // Invalid Filter
        fclose(input);
        fclose(output);
        free(imagedata);
        remove(outputFileName); // Deleting Output File
        printf("\nInvlaid Filter. Exiting...\n\n");
        printf("========================================================\n");
        return 7;
    }

    fwrite(&bfh, sizeof(BMPFileHeader), 1, output); // Writing BMPFileHeader to Output Image
    fwrite(&bih, sizeof(BMPInfoHeader), 1, output); // Writing BMPInfoHeader to Output Image

    for (int i = 0; i < height; i++)
    {
        fwrite(imagedata[i], sizeof(RGB), width, output); // Writing updated imagedata to output file

        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, output);
        }
    }

    printf("\nOutput Image Created as %s\n\n", outputFileName);

    // Closing Opened Files and Freeing Image Data
    fclose(input);
    fclose(output);
    free(imagedata);
}

// ============================================================================
// Grayscale
void grayscale(int height, int width, RGB image[height][width])
{
    for (int i = 0, n = height; i < n; i++)
    {
        for (int j = 0, m = width; j < m; j++)
        {
            // Shade of grey for individual pixel
            int average = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            //Converting each pixel to shade of grey
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = average;
        }
    }
    return;
}
// ============================================================================

// ============================================================================
// Sepia
void sepia(int height, int width, RGB image[height][width])
{
    // Choosing Strength
    int strength;
    do
    {
        printf("\nStrength of Sepia Filter between 1 and 3: ");
        scanf("%i%*c", &strength);
    } while (strength <= 0 || strength > 3);

    for (int i = 0; i < strength; i++)
    {
        for (int i = 0, n = height; i < n; i++)
        {
            for (int j = 0, m = width; j < m; j++)
            {
                RGB pixel = image[i][j];

                // RGB Values for the sepia pixel
                int sepiaRed = round((.393 * pixel.rgbtRed) + (.769 * pixel.rgbtGreen) + (.189 * pixel.rgbtBlue));
                int sepiaGreen = round((.349 * pixel.rgbtRed) + (.686 * pixel.rgbtGreen) + (.168 * pixel.rgbtBlue));
                int sepiaBlue = round((.272 * pixel.rgbtRed) + (.534 * pixel.rgbtGreen) + (.131 * pixel.rgbtBlue));

                // Converting each pixel to respecrive seipa colour
                image[i][j].rgbtRed = sepiaRed > 255 ? 255 : sepiaRed;
                image[i][j].rgbtGreen = sepiaGreen > 255 ? 255 : sepiaGreen;
                image[i][j].rgbtBlue = sepiaBlue > 255 ? 255 : sepiaBlue;
            }
        }
    }

    return;
}
// ============================================================================

// ============================================================================
// Reflection
void reflection(int height, int width, RGB image[height][width])
{
    RGB temp[height][width];

    // Copying each pixel to a temporary variable
    for (int i = 0; i < height; i++)
    {

        for (int j = 0; j < width / 2; j++)
        {
            swap(&image[i][j], &image[i][width - 1 - j]);
        }
    }
    return;
}
// ============================================================================

// ============================================================================
// Blur
void blur(int height, int width, RGB image[height][width])
{
    // Choosing Strength
    int strength;
    do
    {
        printf("\nStrength of Blur Filter between 1 and 10: ");
        scanf("%i%*c", &strength);
    } while (strength <= 0 || strength > 10);

    for (int i = 0; i < strength; i++)
    {
        RGB temp[height][width];

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                float count = 0;
                float sumRed = 0, sumBlue = 0, sumGreen = 0;
                for (int m = -1; m < 2; m++)
                {
                    for (int n = -1; n < 2; n++)
                    {
                        // Valid coordinates
                        if (i + m >= 0 && j + n >= 0 && i + m < height && j + n < width)
                        {
                            count++;
                            sumRed += image[i + m][j + n].rgbtRed;
                            sumGreen += image[i + m][j + n].rgbtGreen;
                            sumBlue += image[i + m][j + n].rgbtBlue;
                        }
                    }
                }

                // Converting each pixel in a temporary variable
                temp[i][j].rgbtRed = round(sumRed / count);
                temp[i][j].rgbtGreen = round(sumGreen / count);
                temp[i][j].rgbtBlue = round(sumBlue / count);
            }
        }

        // Updating imagedata with updated RGB values
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                image[i][j] = temp[i][j];
            }
        }
    }

    return;
}

void edge(int height, int width, RGB image[height][width])
{
    grayscale(height, width, image);
    RGB temp[height][width];

    float gx[3][3] = {
        {-0.5, 0, 0.5},
        {-0.1, 0, 0.1},
        {-0.5, 0, 0.5}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGB1 GX = {0, 0, 0};
            RGB1 GY = {0, 0, 0};

            for (int m = -1; m < 2; m++)
            {
                for (int n = -1; n < 2; n++)
                {
                    if (i + m >= 0 && j + n >= 0 && i + m < height && j + n < width)
                    {
                        GX.rgbtRed += image[i + m][j + n].rgbtRed * gx[m + 1][n + 1];
                        GX.rgbtGreen += image[i + m][j + n].rgbtGreen * gx[m + 1][n + 1];
                        GX.rgbtBlue += image[i + m][j + n].rgbtBlue * gx[m + 1][n + 1];

                        GY.rgbtRed += image[i + m][j + n].rgbtRed * gx[n + 1][m + 1];
                        GY.rgbtGreen += image[i + m][j + n].rgbtGreen * gx[n + 1][m + 1];
                        GY.rgbtBlue += image[i + m][j + n].rgbtBlue * gx[n + 1][m + 1];
                    }
                }
            }

            temp[i][j].rgbtRed = round(sqrt(GX.rgbtRed * GX.rgbtRed + GY.rgbtRed * GY.rgbtRed));
            temp[i][j].rgbtGreen = round(sqrt(GX.rgbtGreen * GX.rgbtGreen + GY.rgbtGreen * GY.rgbtGreen));
            temp[i][j].rgbtBlue = round(sqrt(GX.rgbtBlue * GX.rgbtBlue + GY.rgbtBlue * GY.rgbtBlue));
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed > 255 ? 255 : temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen > 255 ? 255 : temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue > 255 ? 255 : temp[i][j].rgbtBlue;
        }
    }
}

// ============================================================================

// Swap Function
void swap(RGB *a, RGB *b)
{
    RGB temp = *a;
    *a = *b;
    *b = temp;
}
