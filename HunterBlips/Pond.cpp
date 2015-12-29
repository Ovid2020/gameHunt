#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

#include "Pond.h"

void Pond::setPondRadius(double a){
    pondRad = a;
};

double Pond::getPondRadius(){
    return pondRad;
};

Pond::Pond(){
    pondRad = 45;
};

Pond::Pond(double a){
    pondRad = a;
};

