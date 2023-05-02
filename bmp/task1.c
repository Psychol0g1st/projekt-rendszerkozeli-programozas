#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    char filename[] = "proc2.bmp";

    int f = open(filename, O_RDWR);
    if (f < 0)
    {
        printf("Error opening file %s\n", filename);
        return 1;
    }
    int size;
    lseek(f, 2, SEEK_CUR);
    read(f, &size, 4);
    printf("Size: %d bytes\n", size);
    lseek(f, 12, SEEK_CUR);
    int width, height;
    read(f, &width, 4);
    read(f, &height, 4);
    printf("Width: %d\nHeight: %d\n", width, height);
    int image_size;
    lseek(f, 8, SEEK_CUR);
    read(f, &image_size, 4);
    printf("Image size: %d bytes\n", image_size);

    int row_size = image_size / height;
    printf("Row size: %d bytes\n", row_size);
    int padding = row_size - width;
    printf("Padding: %d bytes\n", padding);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            unsigned char pixel;
            read(f, &pixel, 1);
            if (pixel == 0)
                printf(" ");
            else
                printf("*");
        }
        lseek(f, padding, SEEK_CUR);
        printf("\n");
    }
    return 0;
}