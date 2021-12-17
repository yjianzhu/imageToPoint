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
};


#endif