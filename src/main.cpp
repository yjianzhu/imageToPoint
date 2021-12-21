#include<vector>
#include<string>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<cmath>
#include<fstream>
#include<map>
#include"image.h"
#include <unistd.h>
#include "terminal.h"


#define TRANSP_ALPHA 64
using namespace std;

uint32_t pixelToInt(const color *pixel) {
    if (pixel->a == 0)
        return 0xffff;
    else if (pixel->r == pixel->g && pixel->g == pixel->b)
        return 232 + (pixel->r * 23)/255;
    else
        return (16 + ((pixel->r*5)/255)*36
                + ((pixel->g*5)/255)*6
                + (pixel->b*5)/255);
}

char supportsUTF8() {
    const char* my_LC_ALL = getenv("LC_ALL");
    const char* LANG = getenv("LANG");
    const char* my_LC_CTYPE = getenv("LC_CTYPE");
    const char* UTF = "UTF-8";
    return (my_LC_ALL && strstr(my_LC_ALL, UTF))
        || (LANG && strstr(LANG, UTF))
        || (my_LC_CTYPE && strstr(my_LC_CTYPE, UTF));
}

int main(int argc,char *argv[])
{
    //image read(argv[1]);

    //test
    //string filename="../image/yipinhong.jpg";
    // fstream read;
    // read.open(filename,ios::in);
    // read.close();

    string filename="../image/xi.jpg";
    image I_my(filename.c_str());

    //define some parameter
    int loop=0,loops=1,precision=0;
    uint8_t true_color = 1;
    short width_terminal,height_terminal;


    //resize the picture
    width_terminal=terminal_columns();
    height_terminal=terminal_rows();
    I_my.resize(width_terminal,height_terminal);


    if (precision == 0 || precision > 2) {
        if (supportsUTF8())
            precision = 2;
        else
            precision = 1;
    }
    //precision=1;
    I_my.print();

    printf("\e[s\e[?25l");
    while (loop++ < loops || loops < 0) {
        uint32_t offset = 0;
        for (uint32_t frame = 0; frame < I_my.frames; frame++) 
        {
            if (frame > 0 || loop > 0) {
                if (frame > 0)
                    usleep(I_my.delays[frame - 1] * 10000);
                else
                    usleep(I_my.delays[I_my.frames - 1] * 10000);
                printf("\e[u");
            }
            uint32_t index, x, y;
            for (y = 0; y < I_my.height; y += 1) {
                for (x = 0; x < I_my.width; x++) {
                    index = y * I_my.width + x + offset;
                    const color* upperPixel = &I_my.pixels[index];
                    uint32_t fgCol = pixelToInt(upperPixel);


                    if (precision == 2) 
                    {

                        if (y < I_my.height - 1) 
                        {
                            const color* lowerPixel = &I_my.pixels[index + I_my.width];
                            uint32_t bgCol = pixelToInt(&I_my.pixels[index + I_my.width]);
                            if (upperPixel->a < TRANSP_ALPHA) 
                            { // first pixel is transparent
                                if (lowerPixel->a < TRANSP_ALPHA)
                                    printf("\e[m ");
                                else
                                    if (true_color)
                                        printf("\x1b[0;38;2;%d;%d;%dm\u2584",
                                               lowerPixel->r, lowerPixel->g, lowerPixel->b);
                                    else
                                        printf("\e[0;38;5;%um\u2584", bgCol);
                            } else {
                                if (lowerPixel->a < TRANSP_ALPHA)
                                    if (true_color)
                                        printf("\x1b[0;38;2;%d;%d;%dm\u2580",
                                               upperPixel->r, upperPixel->g, upperPixel->b);
                                    else
                                        printf("\e[0;38;5;%um\u2580", fgCol);
                                else
                                    if (true_color)
                                        printf("\x1b[48;2;%d;%d;%dm\x1b[38;2;%d;%d;%dm\u2584",
                                               upperPixel->r, upperPixel->g, upperPixel->b,
                                               lowerPixel->r, lowerPixel->g, lowerPixel->b);
                                    else
                                         printf("\e[38;5;%u;48;5;%um\u2580", fgCol, bgCol);
                            }

                        } 
                        
                        
                        else { // this is the last line
                            if (upperPixel->a < TRANSP_ALPHA)
                                printf("\e[m ");
                            else
                                if (true_color)
                                    printf("\x1b[0;38;2;%d;%d;%dm\u2580",
                                           upperPixel->r, upperPixel->g, upperPixel->b);
                                else
                                    printf("\e[38;5;%um\u2580", fgCol);
                        }


                    } 
                    
                    else 
                    {
                        if (I_my.pixels[index].a < TRANSP_ALPHA)
                            printf("\e[0m  ");
                        else
                            if (true_color)
                                printf("\x1b[0;48;2;%d;%d;%dm  ",
                                       I_my.pixels[index].r, I_my.pixels[index].g, I_my.pixels[index].b);
                            else
                                printf("\e[48;5;%um  ", fgCol);
                    }
                }
                // printf("\e[m\n");
            }
            offset += I_my.width * I_my.height;
            //frame = I_my.frames;
        }
    }
    // Display the cursor again
    printf("\e[?25h");


    // fstream read;
    // read.open(argv[1],ios::in);

    // read.seekg(0,read.end);
    // int file_length=read.tellg();
    // read.seekg(0,read.beg);

    // char* buffer=new char[file_length];
    // read.read(buffer,file_length);

    // read.close();

    // cout<<buffer<<endl;

    // delete[] buffer;  
    

    // return 0;
}