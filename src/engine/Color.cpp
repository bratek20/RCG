#include "Color.h"
#include "Globals.h"

std::ostream& operator<<(std::ostream& out, const Color& c){
    out << "Color(" << c.r << ", " << c.g << ", " << c.b << ")";
    return out;
}

float Color::getAverage() const{
    return (r + g + b) / 3;
}
Color Color::random(){
    return {Globals::random(), Globals::random(), Globals::random()};
}