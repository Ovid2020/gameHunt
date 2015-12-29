#include "Tree.h"

Tree::Tree(){
    trunkPosition.x = 0;
    trunkPosition.y = 0;
}

Tree::Tree(double a, double b){
    trunkPosition.x = a;
    trunkPosition.y = b;
}

Tree::Tree(sf::Vector2f posVec){
    trunkPosition = posVec;
}

void Tree::setWidth(int a){
    treeWid=a;
}

void Tree::setHeight(int a){
    treeHei=a;
}

int Tree::getWidth(){
    return treeWid;
}

int Tree::getHeight(){
    return treeHei;
}

