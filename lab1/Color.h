#ifndef COLOR_H
#define COLOR_H

#include <GL/glew.h>
#include <glm/glm.hpp>

struct Color
{
    float r;
    float g;
    float b;

    Color(float r, float g, float b):
        r(r), g(g), b(b) {}    

    Color() : 
        Color(0, 0, 0) {}
    
    Color(int r, int g, int b) : 
        Color(r / 255.0f, g / 255.0f, b / 255.0f) {}

    bool operator==(const Color& other)
    {
        return r == other.r && g == other.g && b == other.b;
    }
    
    operator glm::vec3() const {
        return {r, g, b};
    }

    static Color random();
};

namespace Colors{
    const Color WHITE = Color(1.0f, 1.0f, 1.0f);
    const Color BLACK = Color(0.0f, 0.0f, 0.0f);
    const Color GREY = Color(0.75f, 0.75f, 0.75f);
    const Color BROWN = Color(1.0f, 0.9f, 0.8f);

    const Color RED = Color(1.0f, 0.0f, 0.0f);
    const Color GREEN = Color(0.0f, 1.0f, 0.0f);
    const Color BLUE = Color(0.0f, 0.0f, 1.0f);
    const Color YELLOW = Color(1.0f, 1.0f, 0.0f);
    const Color MAGENTA = Color(1.0f, 0.0f, 1.0f);
    const Color CYAN = Color(0.0f, 1.0f, 1.0f);
    const Color SAND = Color(0.93f, 0.83f, 0.43f);
}


#endif
