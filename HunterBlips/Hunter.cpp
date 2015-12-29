#include <SFML/Graphics.hpp>
#include "Hunter.h"

Hunter::Hunter(){
    accuracy = 6;
    speed = 2.7;
    radius = 5;
    damage = .2;
    color = sf::Color::White;
}

Hunter::Hunter(double a, double b, double c, double d, sf::Color e) {
    accuracy = a;
    speed = b;
    radius = c;
    damage = d;
    color = e;
}

void Hunter::setAccuracy(double a){
    accuracy=a;
}

void Hunter::setSpeed(double a){
    speed=a;
}

void Hunter::setRadius(double a){
    radius=a;
}

void Hunter::setDamage(double a){
    damage=a;
}

void Hunter::setFillColor(sf::Color a){
    color=a;
}

double Hunter::getAccuracy(){
    return accuracy;
}

double Hunter::getSpeed(){
    return speed;
}

double Hunter::getRadius(){
    return radius;
}

double Hunter::getDamage(){
    return damage;
}

sf::Color Hunter::getFillColor(){
    return color;
}

