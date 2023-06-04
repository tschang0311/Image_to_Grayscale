#include <stdlib.h>
#include <stdio.h>
#include "hw6.h"

int main(int argc, char *argv[])
{
    FILE *f = NULL;
    if (argc < 2)
        f = stdin;
    else if (argc == 2)
        f = fopen(argv[1], "r");

    struct image *image = read_image(f);

    grayscale_seq(image);

    printf("P3\n%u %u\n255\n", image -> width, image -> height);

    int dimensions = image -> width * image -> height;

    for (int i = 0; i < dimensions; i++)
        printf("%d %d %d\n", image -> pixels[i].red, image -> pixels[i].green, image -> pixels[i].blue);

    free(image -> pixels);
    free(image);

    return 0;
}
