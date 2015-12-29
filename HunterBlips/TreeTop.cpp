#include "TreeTop.h"
TreeTop::TreeTop(){
    bottomPosition.x = 0;
    bottomPosition.y = 0;
    leafRad = 20;
}

TreeTop::TreeTop(double a, double b, double c){
    bottomPosition.x = a;
    bottomPosition.y = b;
    leafRad = c;
}

TreeTop::TreeTop(sf::Vector2f posVec, double a){
    bottomPosition = posVec;
    leafRad = a;
}

void TreeTop::setLeafRadius(int a){
    leafRad = a;
}

double TreeTop::getLeafRadius(){
    return leafRad;
}

sf::Vector2f TreeTop::getPosition(){
    return bottomPosition;
}
