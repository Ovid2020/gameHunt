#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

#include "Star.h"

void Star::setStarRadius(double a){
    starRad = a;
};

double Star::getStarRadius(){
    return starRad;
};

Star::Star(){
    starRad = 5;
};

Star::Star(double a){
    starRad = a;
};


