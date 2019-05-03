#ifndef PHOTO_SAVER_H
#define PHOTO_SAVER_H

#include "Color.h"

#include <OpenEXR/ImfRgba.h>

class PhotoSaver {
    Imf::Rgba *pixels;
    int width;
    int height;
public:
    PhotoSaver(int width, int height);
    ~PhotoSaver();

    void setPixel(int x, int y, Color c);
    void save(std::string path);
};

#endif