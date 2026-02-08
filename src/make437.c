#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define GRID_RESOLUTION 16
#define CHAR_COUNT 256

static const uint16_t cp437_to_unicode[256] = {
    0x0000, 0x263A, 0x263B, 0x2665, 0x2666, 0x2663, 0x2660, 0x2022, 0x25D8, 0x25CB, 0x25D9, 0x2642, 0x2640, 0x266A, 0x266B, 0x263C,
    0x25BA, 0x25C4, 0x2195, 0x203C, 0x00B6, 0x00A7, 0x25AC, 0x21A8, 0x2191, 0x2193, 0x2192, 0x2190, 0x221F, 0x2194, 0x25B2, 0x25BC,
    0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
    0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
    0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
    0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
    0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
    0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x2302,
    0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7, 0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
    0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9, 0x00FF, 0x00D6, 0x00DC, 0x00A2, 0x00A3, 0x00A5, 0x20A7, 0x0192,
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA, 0x00BF, 0x2310, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, 0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
    0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4, 0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229,
    0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248, 0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0,
};

typedef struct {
    const char* input_file;
    const char* output_file;
    int pixel_height;
} args_t;

static int parse_args(int argc, char** argv, args_t* args) {
    if (argc != 4) {
        fprintf(stderr, "Usage: make437 <input_file> <output_file> <pixel_height>\n");

        return 0;
    }

    args->input_file = argv[1];
    args->output_file = argv[2];
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

    int image_width = GRID_RESOLUTION * args.pixel_height;
    uint8_t* image_data = calloc(image_width * image_width, sizeof(uint8_t));
    float scale_factor = stbtt_ScaleForPixelHeight(&font, args.pixel_height);
    int ascent;
    stbtt_GetFontVMetrics(&font, &ascent, NULL, NULL);
    int baseline = (int) (ascent * scale_factor);
    for (int i = 0; i < CHAR_COUNT; i++) {
        int grid_x = args.pixel_height * (i % GRID_RESOLUTION);
        int grid_y = args.pixel_height * (i / GRID_RESOLUTION);
        int codepoint = cp437_to_unicode[i];
        int width, height, off_x, off_y;
        unsigned char* glyph_data = stbtt_GetCodepointBitmap(
            &font,
            0, scale_factor,
            codepoint,
            &width, &height,
            &off_x, &off_y
        );
        if (glyph_data) {
            for (int glyph_y = 0; glyph_y < height; glyph_y++) {
                for (int glyph_x = 0; glyph_x < width; glyph_x++) {
                    int glyph_index = glyph_y * width + glyph_x;
                    int cell_x = glyph_x + off_x;
                    int cell_y = glyph_y + off_y + baseline;
                    if (cell_x >= 0 && cell_x < args.pixel_height && cell_y >= 0 && cell_y < args.pixel_height) {
                        int image_x = grid_x + cell_x;
                        int image_y = grid_y + cell_y;
                        if (image_x >= 0 && image_x < image_width && image_y >= 0 && image_y < image_width) {
                            int image_index = image_y * image_width + image_x;
                            image_data[image_index] = glyph_data[glyph_index];
                        }
                    }
                }
            }
            stbtt_FreeBitmap(glyph_data, font.userdata);
        }
    }
    if (!stbi_write_png(args.output_file, image_width, image_width, 1, image_data, image_width)) {
        printf("Failed to write image to %s\n", args.output_file);
        free(image_data);
        free(font_data);

        return 1;
    }

    free(image_data);
    free(font_data);

    return 0;
}