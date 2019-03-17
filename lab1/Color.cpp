#include "Color.h"
#include "Globals.h"

std::ostream& operator<<(std::ostream& out, const Color& c){
    out << "Color(" << c.r << ", " << c.g << ", " << c.b << ")";
    return out;
}

Color Color::random(){
    return {Globals::random(), Globals::random(), Globals::random()};
}