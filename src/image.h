#ifndef IMAGE
#define IMAGE
#include "color.h"

class image
{
private:
    /* data */
public:
    color *pixels;
    int width,
        height,
        frames;
    short *delays;
    image(const char * file/* args */);
    ~image();
    void print();
    void resize(short w_new,short h_new);
};


#endif