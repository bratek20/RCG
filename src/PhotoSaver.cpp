#include "PhotoSaver.h"

#include <OpenEXR/ImfRgbaFile.h>

using namespace Imf;

PhotoSaver::PhotoSaver(int width, int height) : width(width), height(height) {
    pixels = new Rgba[width * height];
}

PhotoSaver::~PhotoSaver() { delete[] pixels; }

void PhotoSaver::setPixel(int x, int y, Color c) {
    Rgba pixel;
    pixel.a = 1;
    pixel.r = c.r;
    pixel.g = c.g;
    pixel.b = c.b;
    pixels[y * width + x] = pixel;
}

void PhotoSaver::save(std::string path) {
    RgbaOutputFile file(path.c_str(), width, height, WRITE_RGBA);
    file.setFrameBuffer(pixels, 1, width);
    file.writePixels(height);
}