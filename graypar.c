#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "hw6.h"

int main(int argc, char *argv[])
{
    FILE *f = NULL;
    if (argc < 3)
        f = stdin;
    else if (argc == 3)
        f = fopen(argv[2], "r");
    else
        return 1;

    if (f == NULL)
        return 1;
    
    struct image *image = read_image(f);

    grayscale_par(image, atoi(argv[1]));

    struct timeval start;
    struct timeval stop;
    gettimeofday(&start, NULL);
    grayscale_par(image, atoi(argv[1]));
    gettimeofday(&stop, NULL);
    // compute t0 and t1 in microseconds
    unsigned long int t0 = start.tv_sec * 1000000 + start.tv_usec;
    unsigned long int t1 = stop.tv_sec * 1000000 + stop.tv_usec;
    fprintf(stderr, "image processing time (usec): %d\n", t1-t0);

    printf("P3\n%u %u\n255\n", image -> width, image -> height);

    int dimensions = image -> width * image -> height;

    for (int i = 0; i < dimensions; i++)
        printf("%d %d %d\n", image -> pixels[i].red, image -> pixels[i].green, image -> pixels[i].blue);

    free(image -> pixels);
    free(image);

    return 0;
}
