#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Enter one argument\n");
        return 1;
    }
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file\n");
        return 1;
    }
    BYTE buffer[512];
    int count = 0;
    FILE *output = NULL;
    char filename[8];
    while (fread(buffer, 1, 512, card) == 512)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (output != NULL)
            {
                fclose(output);
            }
            sprintf(filename, "%03i.jpg", count++);
            output = fopen(filename, "w");
            if (output == NULL)
            {
                printf("Error opening the file\n");
                fclose(card);
                return 1;
            }
            if (output == NULL)
            {
                printf("Error opening the file\n");
                fclose(card);
                return 1;
            }
        }
        if (output != NULL)
        {
            fwrite(buffer, 1, 512, output);
        }
    }
    if (output != NULL)
    {
        fclose(output);
    }
    fclose(card);

    return 0;
}
