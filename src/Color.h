#ifndef COLOR_H
#define COLOR_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <ostream>

struct Color {
    float r;
    float g;
    float b;

    Color(float r, float g, float b) : r(fix(r)), g(fix(g)), b(fix(b)) {}
    Color(glm::vec3 c) : Color(c.x, c.y, c.z) {}
    Color() : Color(0.0f, 0.0f, 0.0f) {}
    Color(unsigned char r, unsigned char g, unsigned char b) : Color(r / 255.0f, g / 255.0f, b / 255.0f) {}

    bool operator==(const Color &other) const {
        return r == other.r && g == other.g && b == other.b;
    }

    Color operator+(const Color &c) const {
        return Color(r + c.r, g + c.g, b + c.b);
    }

    Color operator+=(const Color &c) {
        *this = *this + c;
        return *this;
    }

    Color operator*(const Color &c) const {
        return Color(r * c.r, g * c.g, b * c.b);
    }

    Color operator*(float a) const { return Color(r * a, g * a, b * a); }

    Color operator/(float a) const { return Color(r / a, g / a, b / a); }

    operator glm::vec3() const { return {r, g, b}; }

    static Color random();

    float getAverage() const;

    friend std::ostream &operator<<(std::ostream &out, const Color &c);

  private:
    float fix(float x) { return glm::clamp(x, 0.0f, 1.0f); }
};

namespace Colors {
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
} // namespace Colors

#endif
