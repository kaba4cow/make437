#include <stdio.h>
#include <stdlib.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

typedef struct {
    const char* input_file;
    const char* output_file;
    const char* image_format;
    int pixel_height;
} args_t;

static int parse_args(int argc, char** argv, args_t* args) {
    if (argc != 5) {
        fprintf(stderr, "Usage: make437 <input_file> <output_file> <pixel_height> <image_format>\n");

        return 0;
    }

    args->input_file = argv[1];
    args->output_file = argv[2];
    args->image_format = argv[4];

    args->pixel_height = atoi(argv[3]);
    if (args->pixel_height <= 0) {
        fprintf(stderr, "Pixel height must be greater than zero: %s\n", argv[3]);

        return 0;
    }

    return 1;
}

int main(int argc, char** argv) {
    args_t args;
    if (!parse_args(argc, argv, &args)) {
        return 1;
    }

    FILE* file = fopen(args.input_file, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open font file: %s\n", args.input_file);

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
        fprintf(stderr, "Failed to parse font file: %s\n", args.input_file);

        return 1;
    }

    free(font_data);

    return 0;
}