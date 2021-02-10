#include "bitmap_utils.h"

BITMAPINFOHEADER::BITMAPINFOHEADER() : size(40), width(0), height(0), planes(1), bitCount(0), compression(0), sizeImage(0), XPelsPerMeter(0), YPelsPerMeter(0), colorUsed(0), colorImportant(0) {}

BITMAPINFOHEADER::BITMAPINFOHEADER(const unsigned short bCount, const int _width, const int _height) : size(0), width(_width), height(_height), planes(1), bitCount(bCount), compression(0), XPelsPerMeter(0), YPelsPerMeter(0), colorUsed(0), colorImportant(0)
{
    sizeImage = bitCount * this->height * this->width;
}

BITMAPINFOHEADER::~BITMAPINFOHEADER() {}

BITMAPINFOHEADER BITMAPINFOHEADER::operator=(const BITMAPINFOHEADER &other)
{
    if (this == &other)
        return *this;
    size = other.size;
    width = other.width;
    height = other.height;
    planes = other.planes;
    bitCount = other.bitCount;
    compression = other.compression;
    sizeImage = other.sizeImage;
    XPelsPerMeter = other.XPelsPerMeter;
    YPelsPerMeter = other.YPelsPerMeter;
    colorUsed = other.colorUsed;
    colorImportant = other.colorImportant;
    return *this;
}

BITMAPFILEHEADER::BITMAPFILEHEADER() : type(0x4D42), size(0), reserved1(0), reserved2(0), offsetBits(0) {}

BITMAPFILEHEADER::BITMAPFILEHEADER(const unsigned short bCount, const int width, const int height) : type(0x4D42), reserved1(0), reserved2(0), offsetBits(0)
{
    size = bCount * width * height + offsetBits;
}

BITMAPFILEHEADER::~BITMAPFILEHEADER() {}