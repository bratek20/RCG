#include "Color.h"
#include "Globals.h"

Color Color::random(){
    return {Globals::random(), Globals::random(), Globals::random()};
}