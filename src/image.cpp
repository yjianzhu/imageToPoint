#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//read image from file

void setPixelGray(color *pixel, unsigned char* ptr) {
        pixel->r = pixel->g = pixel->b = ptr[0];
        pixel->a = 255;
}

void setPixelGrayAlpha(color *pixel, unsigned char* ptr) {
        pixel->r = pixel->g = pixel->b = ptr[0];
        pixel->a = ptr[1];
}
void setPixelRGB(color *pixel, unsigned char* ptr) {
        pixel->r = ptr[0];
        pixel->g = ptr[1];
        pixel->b = ptr[2];
        pixel->a = 255;
}

void setPixelRGBAlpha(color *pixel, unsigned char* ptr) {
        pixel->r = ptr[0];
        pixel->g = ptr[1];
        pixel->b = ptr[2];
        pixel->a = ptr[3];
}
unsigned char *stbi_xload(char const *filename, int *x, int *y, int *frames, int *channels)
{
        FILE *f;
        stbi__context s;
        unsigned char *result = 0;

        if (!(f = stbi__fopen(filename, "rb")))
                return stbi__errpuc("can't fopen", "Unable to open file");

        stbi__start_file(&s, f);

        result = stbi_load(filename, x, y, frames, *channels);

        fclose(f);

        return result;
}

void img_load_from_data(image *img, stbi_uc* ptr, int w, int h, int frames, int channels) {
        if (ptr && w && h) {
                img->width = w;
                /*h *= 3;*/
                img->height = h;
                img->delays = NULL;
                img->frames = frames;

                if (!(img->pixels = new color[w*h * frames])) {
                        perror("malloc error\n");
                        exit(1);
                }

                if (frames > 1 && !(img->delays = new short[frames])) { // avoid buffer overflow
                        perror("malloc error\n");
                        exit(1);
                }

                // fill the array
                void (*pixelSetter)(color *pixel, unsigned char* ptr) = &setPixelGray;
                switch (channels) {
                        case 1:
                                pixelSetter = &setPixelGray;
                                break;
                        case 2:
                                pixelSetter = &setPixelGrayAlpha;
                                break;
                        case 3:
                                pixelSetter = &setPixelRGB;
                                break;
                        case 4:
                                pixelSetter = &setPixelRGBAlpha;
                                break;
                }
                for (int frame = 0; frame < frames; frame++) {
                        int offset = frame * (sizeof(unsigned char) * channels * w*h + 2);
                        int i = 0;
                        for (int j = 0; j < w*h; i += channels, j++)
                                pixelSetter(&img->pixels[j + frame*w*h], ptr + i * sizeof(unsigned char) + offset);
                        if (frames > 1) {
                                uint16_t delay = ptr[offset + (1 + i) * sizeof(unsigned char)] << 8;
                                delay += ptr[offset + i * sizeof(unsigned char)];
                                img->delays[frame] = delay;
                        }
                }

                stbi_image_free(ptr);
        } else {
                perror("stb_image error");
                exit(1);
        }
}

image::image(const char * file)
{
    int channels=0, w, h, frames;
    unsigned char* ptr = stbi_xload(file, &w, &h, &frames, &channels);
    img_load_from_data(this, ptr, w, h, frames, channels);
}

image::~image()
{
}


