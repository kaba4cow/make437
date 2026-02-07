#include <stdio.h>

int main(int argc, char** argv) {
    if (argc != 5) {
        fprintf(stderr, "Usage: make437 <input_file> <output_file> <pixel_height> <image_format>\n");

        return 1;
    }

    const char* input_file = argv[1];
    (void) input_file;
    const char* output_file = argv[2];
    (void) output_file;
    const char* pixel_height = argv[3];
    (void) pixel_height;
    const char* image_format = argv[4];
    (void) image_format;

    return 0;
}