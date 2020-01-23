#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

int main() {
    int width=0, height=0, channels=0;
    unsigned char *img = stbi_load("Lenna.png", &width, &height, &channels, 1); // 1 -> grayscale

    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

    return 0;
}