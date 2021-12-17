#ifndef IMAGE
#define IMAGE
#include "color.h"

class image
{
private:
    int width,
        height,
        frames;
    short *delays;
    /* data */
public:
    image(const char * file/* args */);
    ~image();
};


#endif