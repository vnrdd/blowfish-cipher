#pragma once

#include "bitmap_utils.h"
#include <iostream>
#include <cassert>
#include <cmath>

class Image
{
private:
    BITMAPINFOHEADER infoHeader; //Header that we need to store.
    RGBQUAD **rgbQuad; //Pointer for array of "pixels". Used for both palette and non-palette images.
    RGBQUAD *palette; //Pointer for storing palette of image(in case of palette images).

public:
    Image(); //Default constructor.
    Image(const char mode, const unsigned short bCount, const int width, const int height); //The constructor taking four arguments and returning an image.
    Image(const char *filename); //Constructor reads images from file.
    Image(const Image &other); //Copy constructor
    int loadImage(const char *filename); //Method that reads image from file.
    int width(); //Get wight
    int height(); //Get height
    RGBQUAD getPixel(int i, int j); //Get pixel
    void setPixel(int i, int j, RGBQUAD pixel); //Set new pixel
    void writeImage(const char *filename) const; //Method that writes image to file.
    Image &operator=(const Image &other); //An assignment operator.
    Image &operator/=(const Image &other); //Scaling operator.
    Image &operator/(const short depth); //Depth modifying operator.
    ~Image(); //Destructor
};