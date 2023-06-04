#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include "hw6.h" 

int luminance(struct rgb c)
{
    int r = c.red;
    int g = c.green;
    int b = c.blue;

    int luminance = (0.2126 * r) + (0.7152 * g) + (0.0722 * b);
    int luminance_trunc = luminance / 1;
    return luminance_trunc;
}

struct rgb grayscale(struct rgb c)
{
    int lum = luminance(c);

    struct rgb gray;
    gray.red = lum;
    gray.green = lum;
    gray.blue = lum;

    return gray;
}

int get_dimensions(struct image *img)
{
    int width = img -> width;
    int height = img -> height;
    int dimensions = width * height;

    return dimensions;
}

int same_image(struct image *img1, struct image *img2)
{
    int img1_dim = get_dimensions(img1);
    int img2_dim = get_dimensions(img2);

    if (img1_dim != img2_dim)
        return 0;
    
    for (int i = 0; i < img1_dim; i++)
    {
        int r1 = img1 -> pixels[i].red;
        int g1 = img1 -> pixels[i].green;
        int b1 = img1 -> pixels[i].blue;

        int r2 = img2 -> pixels[i].red;
        int g2 = img2 -> pixels[i].green;
        int b2 = img2 -> pixels[i].blue;

        if (r1 == r2 && g1 == g2 && b1 == b2)
            continue;
        else
            return 0;
    }
    return 1;
}

int is_grayscale(struct image *img)
{
    int dimensions = get_dimensions(img);

    for (int i = 0; i < dimensions; i++)
    {
        int r = img -> pixels[i].red;
        int g = img -> pixels[i].green;
        int b = img -> pixels[i].blue;

        if (r == g && g == b)
            continue;
        else if (r != g || g != b)
            return 0;
    }
    return 1;
}

struct image *read_image(FILE *f)
{
    char p_type[3];
    fscanf(f, "%2s", p_type);

    int width;
    int height;
    fscanf(f, "%d %d", &width, &height);

    struct image *image = (struct image *)malloc(sizeof(struct image));
    image -> width = width;
    image -> height = height;

    unsigned int dimensions = width * height;
    image -> pixels = (struct rgb *)malloc(dimensions * (sizeof(struct rgb)));

    int depth;
    fscanf(f, "%d ", &depth);

    if (strcmp(p_type, "P3") == 0)
    {
        for (int i = 0; i < dimensions; i++)
        {
            struct rgb pixel;

            int r;
            int g;
            int b;
            fscanf(f, "%d %d %d", &r, &g, &b);

            pixel.red = r;
            pixel.green = g;
            pixel.blue = b;

            image -> pixels[i] = pixel;
        }
    }

    else if (strcmp(p_type, "P6") == 0)
    {
        for (int i = 0; i < dimensions; i++)
        {
            struct rgb pixel;
            pixel.red = getc(f);
            pixel.green = getc(f);
            pixel.blue = getc(f);

            image -> pixels[i] = pixel;
        }
    }
    return image;
}

void grayscale_seq(struct image *img)
{
    int dimensions = get_dimensions(img);

    for (int i = 0; i < dimensions; i++)
    {
        struct rgb gray = grayscale(img -> pixels[i]);
        img -> pixels[i] = gray;
    }
}

struct bounds {
    struct image *img;
    int lower;
    int upper;
};

void *grayscale_par_helper(void *arg)
{
    struct bounds *b = (struct bounds *)arg;
    int lo = b -> lower;
    int up = b -> upper;

    for (int i = lo; i <= up; i++)
    {
        b -> img -> pixels[i] = grayscale(b -> img -> pixels[i]);
    }

    free(b);
    return NULL;
}

void grayscale_par(struct image *img, unsigned int n_threads)
{
    int dimensions = get_dimensions(img);
    int divisions = (dimensions + (n_threads - 1)) / n_threads;
    pthread_t thread[n_threads];

    for (int i = 0; i < n_threads; i++)
    {
        struct bounds *b = (struct bounds *)malloc(sizeof(struct bounds));
        b -> img = img;
        b -> lower = divisions * i + 1;
        b -> upper = divisions * i + divisions;

        if (i == 0)
            b -> lower = 0;
        
        if (i == n_threads - 1)
            b -> upper = (dimensions);
        
        pthread_create(&thread[i], NULL, (void *)&grayscale_par_helper, (void *)b);
    }

    for (int i = 0; i < n_threads; i++)
    {
        pthread_join(thread[i], NULL);
        pthread_detach(thread[i]);
    }
}
