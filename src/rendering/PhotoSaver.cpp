#include "PhotoSaver.h"

#include <OpenEXR/ImfRgbaFile.h>

using namespace Imf;

PhotoSaver::PhotoSaver(int width, int height) : width(width), height(height) {
    pixels = new Rgba[width * height];
}

PhotoSaver::~PhotoSaver() { delete[] pixels; }

void PhotoSaver::setPixel(int x, int y, glm::vec3 color) {
    Rgba pixel;
    pixel.a = 1;
    pixel.r = color.x;
    pixel.g = color.y;
    pixel.b = color.z;
    pixels[y * width + x] = pixel;
}

void PhotoSaver::save(std::string path) {
    RgbaOutputFile file(path.c_str(), width, height, WRITE_RGBA);
    file.setFrameBuffer(pixels, 1, width);
    file.writePixels(height);
}