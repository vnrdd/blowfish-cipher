#include "image.h"

Image::Image() : rgbQuad(NULL) {}

Image::Image(const char mode, const unsigned short bCount, const int width, const int height) {
    //assert((width > 0 && height > 0) && "Incorrect dimentions");
    assert((bCount == 1 || bCount == 8 || bCount == 24 || bCount == 32) && "Incorrect number of bits per one pixel");
    infoHeader.bitCount = bCount;
    infoHeader.width = width;
    infoHeader.height = height;
    infoHeader.sizeImage = bCount * height * width;

    rgbQuad = new RGBQUAD *[infoHeader.height];
    for (int i = 0; i < infoHeader.height; ++i)
        rgbQuad[i] = new RGBQUAD[infoHeader.width];

    if (infoHeader.bitCount == 1 || infoHeader.bitCount == 4 || infoHeader.bitCount == 8){
        int num = pow(2, infoHeader.bitCount);
        palette = new RGBQUAD[num];
        for (int i = 0; i < num; ++i)
            palette[i].red = palette[i].green = palette[i].blue = i;
        if (infoHeader.bitCount == 1)
            palette[1].red = palette[1].green = palette[1].blue = 255;
    }

    for (int i = 0; i < infoHeader.height; ++i){
        for (int j = 0; j < infoHeader.width; ++j){
            rgbQuad[i][j].red = mode;
            rgbQuad[i][j].green = mode;
            rgbQuad[i][j].blue = mode;
            rgbQuad[i][j].reserved = 0;
        }
    }
}

Image::Image(const Image &other){
    infoHeader = other.infoHeader;
    rgbQuad = new RGBQUAD *[infoHeader.height];
    for (int i = 0; i < infoHeader.height; ++i)
        rgbQuad[i] = new RGBQUAD[infoHeader.width];

    if (infoHeader.bitCount == 1 || infoHeader.bitCount == 4 || infoHeader.bitCount == 8){
        int num = pow(2, infoHeader.bitCount);
        palette = new RGBQUAD[num];
        for (int i = 0; i < num; ++i)
            palette[i].red = palette[i].green = palette[i].blue = other.palette[i].red;
    }

    for (int i = 0; i < infoHeader.height; ++i)
        for (int j = 0; j < infoHeader.width; ++j)
            rgbQuad[i][j] = other.rgbQuad[i][j];
}

Image::Image(const char *filename){
    loadImage(filename);
}

Image::~Image(){
    for (int i = 0; i < infoHeader.height; ++i)
        delete rgbQuad[i];
    delete[] rgbQuad;
    if (infoHeader.bitCount == 1 || infoHeader.bitCount == 4 || infoHeader.bitCount == 8)
        delete[] palette;
}

int Image::loadImage(const char *filename){
    BITMAPFILEHEADER bfh;
    FILE *file;
    file = fopen(filename, "rb");
    assert(file != NULL && "Error with opening a file");

    int tryToRead = 0;
    tryToRead = fread(&bfh, sizeof(BITMAPFILEHEADER) - 2, 1, file);

    assert(tryToRead != 0 && "Reading error. Your file is not .bmp");
    tryToRead = fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);

    assert(tryToRead != 0 && "Reading error");

    rgbQuad = new RGBQUAD *[infoHeader.height];
    for (int i = 0; i < infoHeader.height; ++i)
        rgbQuad[i] = new RGBQUAD[infoHeader.width];
    int alignment = 4 - (infoHeader.width * 3) % 4;

    if (infoHeader.bitCount == 1 || infoHeader.bitCount == 4 || infoHeader.bitCount == 8){
        int num = pow(2, infoHeader.bitCount);
        palette = new RGBQUAD[num];
        for (int i = 0; i < num; ++i)
            fread(&palette[i], sizeof(RGBQUAD), 1, file);
        for (int i = 0; i < infoHeader.height; ++i)
        {
            for (int j = 0; j < infoHeader.width;)
            {
                if (infoHeader.bitCount == 1)
                {
                    if (infoHeader.width % 8 == 0)
                        alignment = (4 - (infoHeader.width / 8) % 4) % 4;
                    else
                        alignment = (4 - ((int)trunc(infoHeader.width / 8) + 1) % 4) % 4;
                    unsigned char pix = 0;
                    unsigned char mask = 0x80;
                    fread(&pix, sizeof(unsigned char), 1, file);
                    int shift = 7;
                    for (int k = 0; k < 8; ++k, ++j)
                    {
                        rgbQuad[i][j].red = rgbQuad[i][j].blue = rgbQuad[i][j].green = ((pix & (mask >> k)) >> shift);
                        --shift;
                    }
                }
                else if (infoHeader.bitCount == 8)
                {
                    unsigned char pix = 0;
                    fread(&pix, sizeof(unsigned char), 1, file);
                    rgbQuad[i][j].red = rgbQuad[i][j].blue = rgbQuad[i][j].green = pix;
                    ++j;
                }
            }
            if (alignment != 4)
                fseek(file, alignment, SEEK_CUR);
        }
        fclose(file);
        return 1;
    }
    else
    {

        RGBTRIPLE rgbTriple;
        for (int i = 0; i < infoHeader.height; ++i)
        {
            if (infoHeader.bitCount == 24)
            {
                for (int j = 0; j < infoHeader.width; ++j)
                {
                    fread(&rgbTriple, sizeof(RGBTRIPLE), 1, file);
                    rgbQuad[i][j].red = rgbTriple.red;
                    rgbQuad[i][j].green = rgbTriple.green;
                    rgbQuad[i][j].blue = rgbTriple.blue;
                    rgbQuad[i][j].reserved = 0;
                }
            }
            else if (infoHeader.bitCount == 32)
                fread(rgbQuad[i], sizeof(RGBQUAD), infoHeader.width, file);

            if (alignment != 4)
                fseek(file, alignment, SEEK_CUR);
        }
    }
    fclose(file);
    return 1;
}

int Image::width() {
    return infoHeader.width;

}

int Image::height() {
    return infoHeader.height;
}

RGBQUAD Image::getPixel(int i, int j) {
    return rgbQuad[j][i];
}

void Image::setPixel(int i, int j, RGBQUAD pixel) {
    rgbQuad[j][i] = pixel;
}

void Image::writeImage(const char *filename) const
{
    //assert(infoHeader.sizeImage != 0 && "Invalid operation with empty image");
    FILE *file;
    file = fopen(filename, "wb");
    assert(file != NULL && "Error with opening a file");

    BITMAPFILEHEADER bfh(infoHeader.bitCount, infoHeader.width, infoHeader.height);
    if (infoHeader.bitCount == 1)
        bfh.reserved2 = 62;
    if (infoHeader.bitCount == 8)
        bfh.reserved2 = 1078;
    char buf = 0;
    int alignment = 4 - (infoHeader.width * 3) % 4;
    fwrite(&bfh, sizeof(BITMAPFILEHEADER) - 2, 1, file);
    fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);
    if (infoHeader.bitCount == 1 || infoHeader.bitCount == 4 || infoHeader.bitCount == 8)
    {
        if (infoHeader.bitCount == 1)
        {
            if (infoHeader.width % 8 == 0)
                alignment = (4 - (infoHeader.width / 8) % 4) % 4;
            else
                alignment = (4 - ((int)trunc(infoHeader.width / 8) + 1) % 4) % 4;
        }
        if (infoHeader.bitCount == 8)
            alignment = (infoHeader.width * 3) % 4;
        unsigned char pix = 0;
        for (int i = 0; i < pow(2, infoHeader.bitCount); ++i)
            fwrite(&palette[i], sizeof(RGBQUAD), 1, file);
        for (int i = 0; i < infoHeader.height; ++i)
        {
            for (int j = 0; j < infoHeader.width;)
            {
                pix = 0;
                if (infoHeader.bitCount == 1)
                {
                    for (int k = 0; k < 8; ++k, ++j)
                    {
                        if (k > 0)
                            pix = pix << 1;
                        if (j >= infoHeader.width)
                        {
                            continue;
                        }
                        else
                            pix += rgbQuad[i][j].red;
                    }
                }
                else
                {
                    pix = rgbQuad[i][j].red;
                    ++j;
                }
                fwrite(&pix, sizeof(unsigned char), 1, file);
            }
            if (alignment != 4)
                fwrite(&buf, 1, alignment, file);
        }
    }
    else
    {
        RGBTRIPLE rgbTriple;
        for (int i = 0; i < infoHeader.height; ++i)
        {
            if (infoHeader.bitCount == 24)
            {
                for (int j = 0; j < infoHeader.width; ++j)
                {
                    rgbTriple.red = rgbQuad[i][j].red;
                    rgbTriple.green = rgbQuad[i][j].green;
                    rgbTriple.blue = rgbQuad[i][j].blue;
                    fwrite(&rgbTriple, sizeof(RGBTRIPLE), 1, file);
                }
            }
            else if (infoHeader.bitCount == 32)
                fwrite(rgbQuad[i], sizeof(RGBQUAD), infoHeader.width, file);

            if (alignment != 4)
                fwrite(&buf, 1, alignment, file);
        }
    }
    fclose(file);
}