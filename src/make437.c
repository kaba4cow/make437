#include <stdio.h>
#include <stdlib.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

int main(int argc, char** argv) {
    if (argc != 5) {
        fprintf(stderr, "Usage: make437 <input_file> <output_file> <pixel_height> <image_format>\n");

        return 1;
    }

    const char* input_file = argv[1];
    const char* output_file = argv[2];
    (void) output_file;
    const char* pixel_height = argv[3];
    (void) pixel_height;
    const char* image_format = argv[4];
    (void) image_format;

    FILE* file = fopen(input_file, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open font file: %s\n", input_file);

        return 1;
    }
    fseek(file, 0, SEEK_END);
    long font_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    unsigned char* font_data = malloc(font_size);
    fread(font_data, 1, font_size, file);
    fclose(file);

    stbtt_fontinfo font;
    if (!stbtt_InitFont(&font, font_data, 0)) {
        fprintf(stderr, "Failed to parse font file: %s\n", input_file);

        return 1;
    }

    free(font_data);

    return 0;
}