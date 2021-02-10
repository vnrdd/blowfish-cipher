struct BITMAPINFOHEADER {
    int size;
    int width;
    int height;
    short int planes;
    short int bitCount;
    int compression;
    int sizeImage;
    int XPelsPerMeter;
    int YPelsPerMeter;
    int colorUsed;
    int colorImportant;

    BITMAPINFOHEADER();
    BITMAPINFOHEADER(const unsigned short bCount, const int _width, const int _height);
    BITMAPINFOHEADER operator=(const BITMAPINFOHEADER &other);
    ~BITMAPINFOHEADER();
};

struct BITMAPFILEHEADER {
    short int type;
    int size;
    short int reserved1;
    short int reserved2;
    int offsetBits;

    BITMAPFILEHEADER();
    BITMAPFILEHEADER(const unsigned short bCount, const int width, const int height);
    ~BITMAPFILEHEADER();
};

struct RGBQUAD {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char reserved;
};

struct RGBTRIPLE {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};