#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"
typedef int bool;
#define true 1
#define false 0

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    // TODO #2
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("could not open file\n");
        return 1;
    }

    // Read header
    // TODO #3
    WAVHEADER header;
    fread(&header, sizeof(WAVHEADER), 1, input);

    // Use check_format to ensure WAV format
    // TODO #4
    int test = check_format(header);
    if (test == false)
    {
        printf("input is not a WAV file");
    }

    // Open output file for writing
    // TODO #5
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("could not open file\n");
        return 1;
    }

    // Write header to file
    // TODO #6
    fwrite(&header, sizeof(WAVHEADER), 1, output);

    // Use get_block_size to calculate size of block
    // TODO #7
    int blocksize = get_block_size(header);

    // Write reversed audio to file
    // TODO #8
    fseek(input, 0, SEEK_END);
    long inputsize = ftell(input);
    long datasize = inputsize - sizeof(WAVHEADER);
    int num_blocks = datasize / blocksize;
    unsigned char *buffer = malloc(blocksize);
    for (int i = num_blocks - 1; i >= 0; i--)
    {
        fseek(input, sizeof(WAVHEADER) + blocksize * i, SEEK_SET);
        fread(buffer, blocksize, 1, input);
        fwrite(buffer, blocksize, 1, output);
    }
    free(buffer);
    fclose(input);
    fclose(output);
}

int check_format(WAVHEADER header)
{
    // TODO #4
    int test = 1;
    char array[] = "WAVE";
    for (int i = 0; i < 4; i++)
    {
        if (header.format[i] != array[i])
        {
            test = 0;
        }
    }
    if (test == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
    return 0;
}

int get_block_size(WAVHEADER header)
{
    // TODO #7
    int nch = header.numChannels;
    int bits = header.bitsPerSample;
    int result = nch * (bits / 8);

    return result;
}
