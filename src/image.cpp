#include "image.h"
#include<iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
//read image from file

//******************************************function:get pixel color from char*.********************************
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

//****************************************************load data from jpg or other image format***********************************************
//*******       x is the length, y is width. channels is 颜色通道.      ****************************  

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
                else
                {
                        img->delays =new short[frames];
                        img->delays[0]=1;
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

void image::print()
{
        cout<<"image info:"<<endl;
        cout<<"delays:"<<*this->delays<<endl;
        cout<<"length and width:"<<this->height<<'\t'<<this->width<<endl;
        cout<<"frames:"<<this->frames<<endl;
        cout<<"first pixel:"<<this->pixels->r<<'\t'<<this->pixels->g<<'\t'<<this->pixels->b<<'\t'<<this->pixels->a<<endl;
}
image::image(const char * file)
{
    int channels=0, w, h, frames=1;
    unsigned char* ptr = stbi_load(file, &w, &h, &channels, 0);
    img_load_from_data(this, ptr, w, h, frames, channels);
}

image::~image()
{
}

void image::resize(short w_new,short h_new)
{       
        int wh=int(width/w_new)*int(height/h_new);
        //calculate the new pixels array.
        color * new_pixels;
        if (!(new_pixels =new color[(sizeof(color)*width*height*frames)])) {
                perror("malloc error\n");
                exit(1);
        }
        for(int frame=0;frame<frames;frame++)
        {
                //***************************平均周围的点********************
                int src_offset = height* width * frame, //原来的分隔大小
                        offset = w_new* h_new * frame;  //新的分隔

                for(int y=0;y<h_new;y++)
                {
                        for(int x=0;x<w_new;x++)
                        {
                                int r = 0, g = 0, b = 0, a=0,
                                        srcx=x*(1.0*width/w_new),
                                        srcy=y*(1.0*height/h_new);
                                
                                for (int yi = 0; yi < int(height/h_new); ++yi)
                                        for (int  xi = 0; xi < int(width/w_new); ++xi) {
                                                r += pixels[src_offset + srcx + xi +(srcy+yi)*width].r;
                                                g += pixels[src_offset + srcx + xi +(srcy+yi)*width].g;
                                                b += pixels[src_offset + srcx + xi +(srcy+yi)*width].b;
                                                a += pixels[src_offset + srcx + xi +(srcy+yi)*width].a;
                                        }
                                new_pixels[offset + x+y*w_new].r = r/wh;
                                new_pixels[offset + x+y*w_new].g = g/wh;
                                new_pixels[offset + x+y*w_new].b = b/wh;
                                new_pixels[offset + x+y*w_new].a = a/wh;


                        }
                }
        }


        delete[] pixels;
        pixels=new_pixels;
        //reset the value.
        width=w_new;
        height=h_new;
}

