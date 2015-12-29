#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>
#include "Hunter.h"
#include "Tree.h"
#include "TreeTop.h"
#include "Pond.h"
#include "Star.h"

using namespace std;

//global variables
double userHealth;
const double hunterBoost = 1.75;
const int userSpeed = 10,
          screenWid = 4000,
          screenHei = 2400,
          randomPondNum = rand() % 10 + 8,
          randomTreeNum = rand() % 70 + 180,
          starNum = 35,
          numOfHunters = 25;
sf::CircleShape userBlip(20);
sf::RenderWindow window(VideoMode(screenWid, screenHei), "Let's Play!");

//prototypes of all functions used in the main function
int loadMedia(sf::Music&, sf::Music&, sf::Music&, sf::Music&, sf::Font&);
void introText(sf::Font, sf::Music&);
void winOrLossScreen(int starCount, sf::Font, sf::View);
void buildEnvironment(vector<Pond>&, vector<Tree>&, vector<TreeTop>&, vector<Star>&);
void buildHunters(vector<Hunter>&);
void buildUserBlip(sf::CircleShape&);
void drawStatics(sf::Font, vector<Tree>, vector<TreeTop>, vector<Pond>, vector<Star>, const int);
void drawDynamics(vector<Hunter>&, sf::CircleShape, const double);
void moveUser(CircleShape&, int&, int&, bool&, bool&, char&, char&);
void moveHunters(sf::CircleShape, vector<Hunter>&, double(*)[2], int&, vector<string>&, int*, double&);
void collisionCheckForUser(vector<Tree>&, vector<Pond>&, vector<Star>&, CircleShape&, bool&, bool&, Music&,
        Music&, Music&, int&, sf::Font, const int);
void collisionCheckForHunter(vector<Hunter>&, double, vector<string>&, int);
void userTakesDamage(Hunter, double&);
double calculateDistance(sf::Vector2f, sf::Vector2f);

int main() {

    Clock clock;
    srand(time(NULL));

    sf::Music ding, crash, splash, introMusic;
    sf::Font font;
    sf::View view;
    sf::RectangleShape field(sf::Vector2f(screenWid, screenHei));
    vector<Hunter> huntVec(numOfHunters);
    vector<string> actionString; actionString.resize(numOfHunters);
    vector<Pond> ponds (randomPondNum);
    vector<Tree> trees(randomTreeNum);
    vector<TreeTop> tops(randomTreeNum);
    vector<Star> stars(starNum);
    int userXSpeed, userYSpeed,
        actionTimer[numOfHunters],
        starCount = 0,
        counter = 0;
    double hunterMove[numOfHunters][2];
    char lastKeyPress = 'Z',
         twoKeysAgo = 'Z';
    bool pondCollision, treeCollision,
         pause = true;

    userHealth = 100;
    loadMedia(ding, crash, splash, introMusic, font);
    buildHunters(huntVec);
    buildEnvironment(ponds, trees, tops, stars);
    buildUserBlip(userBlip);
    field.setFillColor(Color(10, 150, 10));
    view.setSize(sf::Vector2f(1200, 750));
    introMusic.setLoop(true);
    introMusic.play();

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed){
                window.close();
            }
            else  if (event.type == Event::KeyPressed) {
                pause = false;
                introMusic.stop();
            }
        }
        if (pause) {
            introText(font, introMusic);
        }
        else if (!pause) {
            while (userHealth > 0 && starCount < 10) {
                window.clear();
                window.draw(field);
                moveUser(userBlip, userXSpeed, userYSpeed, pondCollision, treeCollision, lastKeyPress, twoKeysAgo);
                moveHunters(userBlip, huntVec, hunterMove, counter, actionString, actionTimer, userHealth);
                collisionCheckForUser(trees, ponds, stars, userBlip, pondCollision, treeCollision, ding, crash, splash, starCount, font, counter);
                drawStatics(font, trees, tops, ponds, stars, starCount);
                drawDynamics(huntVec, userBlip, userHealth);
                view.setCenter(sf::Vector2f(userBlip.getPosition()));
                window.setView(view);
                window.display();
            }
            winOrLossScreen(starCount, font, view);
        }
    }
}

double calculateDistance(sf::Vector2f xyPair1, sf::Vector2f xyPair2){
    double distance = sqrt(((xyPair1.x - xyPair2.x) * (xyPair1.x - xyPair2.x) + (xyPair1.y - xyPair2.y) * (xyPair1.y - xyPair2.y)) + 1);
    return distance;
}

void collisionCheckForHunter(vector<Hunter>& hunters, double huntMoveXY[numOfHunters][2], vector<string>& actionString, int actionTimer[]) {
//

    int i, j;
    bool hasCollisionOccurred[numOfHunters];
    double dist, rad;
    for (i = 0; i < hunters.size() - 1; i++) {
        for (j = i + 1; j < hunters.size(); j++) {
        // Notice that j is offset from i. If both loops iterate through the entire array, the code ends up with redundant evaluations.
            if (!hasCollisionOccurred[i]) {
                dist = calculateDistance(hunters[i].getPosition(),hunters[j].getPosition());
                rad = hunters[i].getRadius() + hunters[j].getRadius();
                if (dist <= rad) {
                    hasCollisionOccurred[i] = true;
                    if (hunters[i].getFillColor() == sf::Color::Blue) {
                    // Blue hunters "eat" other hunters, taking on their attributes and then erasing them.
                        hunters[i].setSpeed(hunters[i].getSpeed() + (hunters[j].getSpeed() / 2));
                        hunters[i].setDamage(hunters[i].getDamage() + (hunters[j].getDamage() / 2));
                        hunters[i].setAccuracy(hunters[i].getAccuracy() + (hunters[j].getAccuracy() * 10));
                        hunters[i].setRadius(hunters[i].getRadius() + (hunters[j].getRadius() / 5));
                        hunters.erase(hunters.begin() + j);
                    }
                    else {
                    // A "normal" collision, in which neither hunter eats the other, occurs here.
                        if (hunters[i].getRadius() <= hunters[j].getRadius()){
                        // If the "i" hunter is smaller or equal is size to the "j" hunter, it reverses its direction, i.e. "flee" mode.
                            huntMoveXY[i][0] = huntMoveXY[i][0] * -1;
                            huntMoveXY[i][1] = huntMoveXY[i][1] * -1;
                            actionString[i] = "flee";
                            actionTimer[i] = 0;
                        }
                        if (hunters[j].getRadius() <= hunters[i].getRadius()){
                        // If the "j" hunter is smaller or equal is size to the "i" hunter, it reverses its direction, i.e. "flee" mode.
                        // Notice that both if statements having a <= mean that, if the two are the same size, both flee.
                            huntMoveXY[j][0] = huntMoveXY[j][0] * -1;
                            huntMoveXY[j][1] = huntMoveXY[j][1] * -1;
                            actionString[j] = "flee";
                            actionTimer[j] = 0;
                            hasCollisionOccurred[j] = true;
                        }
                    }
                }
            }
        }
    }
}

void userTakesDamage(Hunter hitter, double& userHealth) {//tracks damage
    userHealth = userHealth - hitter.getDamage();
}

void moveHunters(CircleShape user, vector<Hunter>&hunters, double huntMoveXY[numOfHunters][2], int& counter, vector<string>&actionString, int actionTimer[], double& health) {

    double hunterToUserY, hunterToUserX;
    sf::Vector2f targetXYPosition = user.getPosition();
    int detectCheck, dist, i;

    if (counter == 100) {
        counter = 0;
    }
    for (i = 0; i < hunters.size(); i++) {
        if (counter == 0) {
        // Set the current hunter to "chase" mode and give it a random x and y movement direction (magnitude of movement based on hunter's speed).
            actionString[i] = "chase";
            huntMoveXY[i][0] = (rand() % 3 - 1) * hunters[i].getSpeed();
            huntMoveXY[i][1] = (rand() % 3 - 1) * hunters[i].getSpeed();
        }

        if (actionTimer[i] == rand() % 10 + 35) {
        // A second mechanism to reset actionString gives more diversity to hunter movements.
            actionString[i] = "chase";
            actionTimer[i] = 0;
        }

        dist = calculateDistance(targetXYPosition, hunters[i].getPosition());

        detectCheck = ((rand() % dist + 1) / (hunters[i].getAccuracy())) + 1;
        // If this equals 1, the hunter moves closer to the user. Notice that the quotient approaches zero as distance shrinks and accuracy of the current hunter increases.
        // The + 1 after the division means that, if this zero value occurs, then detectCheck will equal 1, causing the hunter to move closer to the user (below code).

        if (dist > 600) {
        // If the user moves farther than 600 pixels away, the current hunter will sit still.
            huntMoveXY[i][0] = 0;
            huntMoveXY[i][1] = 0;
        }

        if (detectCheck == 1 && actionString[i] == "chase") {
        // The hunter's x and y positions are compared to the user's position. The hunter will be set to move closer to the user.
            hunterToUserX = hunters[i].getPosition().x - targetXYPosition.x;
            hunterToUserY = hunters[i].getPosition().y - targetXYPosition.y;
            if (hunterToUserX > 0) {
            // The hunter is to the right of the user. Set it to move to the left by using the negative of its x speed value.
                huntMoveXY[i][0] = -hunters[i].getSpeed();
            }
            else if (hunterToUserX < 0) {
            // The hunter is to the left of the user. Set it to move to the right by using its x speed value.
                huntMoveXY[i][0] = hunters[i].getSpeed();
            }
            if (hunterToUserY > 0) {
            // The hunter is above the user. Set it to move down by using the negative of its y speed value.
                huntMoveXY[i][1] = - hunters[i].getSpeed();
            }
            else if (hunterToUserY < 0) {
            // The hunter is below the user. Set it to move up by using its y speed value.
                huntMoveXY[i][1] = hunters[i].getSpeed();
            }
        }
        collisionCheckForHunter(hunters, huntMoveXY, actionString, actionTimer);
        // Checks for collisions between hunters and then reassigns huntMoveXY if they do collide

        if (dist < (hunters[i].getRadius() + user.getRadius())) {
        // If the hunter hits the user, the user's health decrements.
            userTakesDamage(hunters[i], health);
        }

        hunters[i].move(huntMoveXY[i][0], huntMoveXY[i][1]);
        // After all the above parameters have been used to set the values of huntMoveXY, the current hunter moves.

        if (actionString[i] == "flee") {
            actionTimer[i]++;
        }
    }
    counter++;
}

void collisionCheckForUser(vector<Tree>&treeVec, vector<Pond>& pondVec, vector<Star>& starVec, CircleShape& userBlip, bool& pondCollision, bool& treeCollision,
                        Music& ding, Music& crash, Music& splash, int& starCount, sf::Font font, const int counter) {

    sf::Vector2f userPos = userBlip.getPosition(), obstacleXY;
    pondCollision = false;
    treeCollision = false;
    int soundCheck = counter % 5;
    double distance;
    for (int i = 0; i < pondVec.size(); i++) {
        obstacleXY = pondVec[i].getPosition();
        distance = calculateDistance(userPos, obstacleXY);
        if (distance <= 70) {
            pondCollision = true;
            if (soundCheck == 0){
                splash.play();
            }
        }
    }
    for (int i = 0; i < treeVec.size(); i++) {       //check for collision with tree
        if ((abs(userPos.x - treeVec[i].getPosition().x) <= 30) && (abs(userPos.y - treeVec[i].getPosition().y) <= 70)) {
            treeCollision = true;
            crash.play();
        }
    }
    for (int i = 0; i < starVec.size(); i++) {       //check for collision with star
        obstacleXY = starVec[i].getPosition();
        distance = calculateDistance(userPos, obstacleXY);
        if (distance <= 25) {
            starCount++;
            ding.play();
            starVec.erase(starVec.begin() + i);
            userHealth = userHealth + 10;
            if (userHealth > 100) {
                userHealth = 100;
            }
        }
    }
}

void moveUser(CircleShape& userBlip, int& userXSpeed, int& userYSpeed, bool& pondCollision, bool& treeCollision, char& lastKeyPress, char& twoKeysAgo) {

    sf::Vector2f userPosition = userBlip.getPosition();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        if (treeCollision && (lastKeyPress == 'L' | (twoKeysAgo == 'L' && lastKeyPress != 'R'))){
                userXSpeed = 0;
        }
        else {
            twoKeysAgo = lastKeyPress;
            lastKeyPress = 'L';
            userXSpeed = -userSpeed;
        }
        if (pondCollision) {
            userXSpeed = userSpeed * -0.5;
        }
        if (userPosition.x < userBlip.getRadius()) {
            userXSpeed = 0;
        }
        userBlip.move(userXSpeed, 0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        if (treeCollision && (lastKeyPress == 'R' | (twoKeysAgo == 'R' && lastKeyPress != 'L'))) {
            userXSpeed = 0;
        }
        else {
            twoKeysAgo = lastKeyPress;
            lastKeyPress = 'R';
            userXSpeed = userSpeed;
        }
        if (pondCollision) {
            userXSpeed = userSpeed * 0.5;
        }
        if (userPosition.x > screenWid - userBlip.getRadius()) {
            userXSpeed = 0;
        }
        userBlip.move(userXSpeed, 0);
        }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        if (treeCollision && (lastKeyPress == 'U' | (twoKeysAgo == 'U' && lastKeyPress != 'D'))) {
            userYSpeed = 0;
        }
        else {
            twoKeysAgo = lastKeyPress;
            lastKeyPress = 'U';
            userYSpeed = -userSpeed;
        }
        if (pondCollision){
            userYSpeed = userSpeed * -0.5;
        }
        if (userPosition.y < userBlip.getRadius()) {
            userYSpeed = 0;
        }
        userBlip.move(0, userYSpeed);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        if ((treeCollision) && (lastKeyPress == 'D' | (twoKeysAgo == 'D' && lastKeyPress != 'U'))) {
            userYSpeed = 0;
        }
        else {
            twoKeysAgo = lastKeyPress;
            lastKeyPress = 'D';
            userYSpeed = userSpeed;
        }
        if (pondCollision){
            userYSpeed = userSpeed * 0.5;
        }
        if (userPosition.y > screenHei - userBlip.getRadius()) {
            userYSpeed = 0;
        }
        userBlip.move(0, userYSpeed);
    }
}

void drawDynamics(vector<Hunter>& huntVec, sf::CircleShape userBlip, const double userHealth) {
    for (int i = 0; i < huntVec.size(); i++) {
        window.draw(huntVec[i]);
    }
    if (userHealth>= 0) {
        sf::Vector2f heatlhBarPosition = userBlip.getPosition();
        sf::RectangleShape healthBarWipe(sf::Vector2f(33, 100 - userHealth));
        healthBarWipe.setPosition(heatlhBarPosition.x + 532, heatlhBarPosition.y - 323);
        healthBarWipe.setFillColor(sf::Color::White);
        window.draw(healthBarWipe);
    }
    window.draw(userBlip);
}

void drawStatics(sf::Font font, vector<Tree> treeVec, vector<TreeTop> topVec, vector<Pond> pondVec, vector<Star> starVec, const int starCount) {

    sf::Vector2f heatlhBarPosition = userBlip.getPosition();
    sf::RectangleShape healthBar(sf::Vector2f(33, 100));
    healthBar.setPosition(heatlhBarPosition.x + 532, heatlhBarPosition.y - 323);
    healthBar.setFillColor(sf::Color::Red);

    sf::RectangleShape healthBarBackground(sf::Vector2f(37, 104));
    healthBarBackground.setFillColor(sf::Color::White);
    healthBarBackground.setPosition(heatlhBarPosition.x + 530, heatlhBarPosition.y - 325);

    sf::Text healthtext("Health", font, 16);
    healthtext.setColor(sf::Color::Red);
    healthtext.setPosition(heatlhBarPosition.x + 522, heatlhBarPosition.y - 220);

    sf::Vector2f starCountTextPosition = userBlip.getPosition();
    string a = to_string(starCount);
    Text starText("Star count: " + a, font, 16);
    starText.setPosition(starCountTextPosition.x + 500, starCountTextPosition.y - 195);
    starText.setColor(Color::Blue);

    int i;

    for (i = 0; i < pondVec.size(); i++){
        window.draw(pondVec[i]);
    }

    for (i = 0; i < treeVec.size(); i++){
        window.draw(treeVec[i]);
    }

    for (i = 0; i < topVec.size(); i++){
        window.draw(topVec[i]);
    }

    for (i = 0; i < starVec.size(); i++){
        window.draw(starVec[i]);
    }
    window.draw(healthtext);
    window.draw(healthBarBackground);
    window.draw(healthBar);
    window.draw(starText);
}

void buildUserBlip(sf::CircleShape& blip) {
    blip.setFillColor(sf::Color::Black);
    blip.setOrigin(20, 20);
    blip.setPosition(500, 500);
}

void buildHunters(vector<Hunter>& huntVec){
    double rad, hunterXPosition, hunterYPosition;
    int i, height, width;
    sf::Vector2f userXYPosition;

    for (i = 0; i < 9; i++) {
        huntVec[i] = Hunter(100, 1.5, 30 - i*.2, 1, sf::Color::Red);
        rad = huntVec[i].getRadius();
        huntVec[i].setOrigin(rad, rad);
    }

    for (i = 10; i < 12; i++) {
        huntVec[5] = Hunter(10, .4, 3, .01, sf::Color::Blue);
        rad = huntVec[5].getRadius();
        huntVec[5].setOrigin(rad, rad);
    }

    for (i = 12; i < numOfHunters; i++) {
        huntVec[i] = Hunter();
    }

    userXYPosition = userBlip.getPosition();
    width = screenWid;
    height = screenHei;

    for (i = 0; i < huntVec.size(); i++){
        hunterXPosition = rand() %  width;
        hunterYPosition = rand() %  height;

        while (abs(hunterXPosition - userXYPosition.x) < 100 | abs(hunterYPosition - userXYPosition.y) < 100) {
            hunterXPosition = rand() %  width;
            hunterYPosition = rand() %  height;
        }

        huntVec[i].setPosition(hunterXPosition, hunterYPosition);
    }

    for (i = 0; i < huntVec.size(); i++) {
        huntVec[i].setSpeed(huntVec[i].getSpeed() * hunterBoost);
        huntVec[i].setAccuracy(huntVec[i].getAccuracy() * hunterBoost);
        huntVec[i].setDamage(huntVec[i].getDamage() * hunterBoost);
    }

}

void buildEnvironment(vector<Pond>& pondVec, vector<Tree>& treeVec, vector<TreeTop>& topVec, vector<Star>& starVec) {

    const double treeWid = 20,
                 treeHei = 100,
                 pondRad = 45;
    double dist, xdist, ydist, ranX, ranY;
    sf::Vector2f obstacleXY;
    int ranPondWid = screenWid - pondRad * 2,
        ranPondHei = screenHei - pondRad * 2,
        ranTreeWid = screenWid - treeWid,
        ranTreeHei = screenHei - treeHei,
        ranStarWid = screenWid - 10,
        ranStarHei = screenHei - 10;
    bool intersect;

    int i = 0;
    while (i < randomPondNum) {
    //set random positions for ponds and make sure they don't overlap
        intersect = false;
        obstacleXY.x = rand() % ranPondWid + pondRad;
        obstacleXY.y = rand() % ranPondHei + pondRad;
        for (int j = 0; j < i; j++) {
            dist = calculateDistance(obstacleXY, pondVec[j].getPosition());
            if (dist <= (pondRad * 2)){
                intersect = true;
            }
        }
        if (abs(obstacleXY.x - 500) <= 74 && abs(obstacleXY.y - 500) <= 65) {
            intersect = true;
        }
        if (!intersect){
            pondVec[i].setPosition(obstacleXY.x, obstacleXY.y);
            i++;
        }
    }

    i = 0;
    while (i < randomTreeNum) {
    //treeGroup.push_back(tree); //set random position for tree tfleeks so they aren't overlapping
        intersect = false;
        ranX = rand() % ranTreeWid + treeWid/2;
        ranY = rand() % ranTreeHei + treeHei/2;

        for (int j = 0; j < i; j++) {
            xdist = abs(ranX - treeVec[j].getPosition().x);
            ydist = abs(ranY - treeVec[j].getPosition().y);
            if (xdist <= 20 && ydist <= 100){
                intersect = true;
            }
        }
        for (int k = 0; k < randomPondNum; k++) {
            if ((abs(ranX - pondVec[k].getPosition().x) <= (treeWid + pondRad * 1.2)) && (abs(ranY - pondVec[k].getPosition().y) <= (treeHei/2 + pondRad))){
                intersect = true;
            }
        }
        if (abs(ranX - 500) <= 30 && abs(ranY - 500) <= 70){
            intersect = true;
        }
        if (!intersect){
            treeVec[i].setPosition(ranX, ranY);
            topVec[i] = TreeTop(treeVec[i].getPosition(), 20);
            i++;
        }
    }

    i = 0;
    while (i < starNum) {
        //check that stars aren't overlapping
        intersect = false;
        obstacleXY.x = rand() % ranStarWid + 5;
        obstacleXY.y = rand() % ranStarHei + 5;

        for (int j = 0; j < pondVec.size(); j++) {
        //check that stars don't overlap with ponds
            dist = calculateDistance(obstacleXY, pondVec[j].getPosition());
            if (dist <= (54 + 5)){
                intersect = true;
            }
        }
        for (int j = 0; j < treeVec.size(); j++) {
        //check that stars don't overlap with trees
            if ((abs(obstacleXY.x - treeVec[j].getPosition().x) <= 15) && (abs(obstacleXY.y - treeVec[j].getPosition().y) <= 55)){
                intersect = true;
            }
        }
        if (!intersect){
            starVec[i].setPosition(obstacleXY.x, obstacleXY.y);
            i++;
        }
    }
}

void introText(Font font, Music& introMusic){
    window.clear(Color::White);
    Text text("Welcome to our game! \n \n Try to gobble 10 yellow 'stars' using the \n arrows before the 'hunters' catch you. \n Watch out for trees and ponds! \n \n Press any key to begin playing.", font, 40);
    text.setColor(sf::Color::Black);
    text.setPosition(100, 100);
    window.draw(text);
    window.display();
}

void winOrLossScreen(int starCount, sf::Font font, sf::View view){
    if (starCount >= 10) {
        window.clear(Color(144, 20, 144));
        window.setSize(sf::Vector2u(800, 800));
        view.setCenter(400, 400);
        sf::Text text; text.setFont(font); text.setString("YOU WIN!");text.setCharacterSize(100);text.setPosition(120, 325);
        window.draw(text);
        window.setView(view);
        window.display();
    }
    else {
        window.clear();
        window.setSize(sf::Vector2u(800, 800));
        view.setCenter(400, 400);
        sf::Text text; text.setFont(font); text.setString("GAME OVER.");text.setCharacterSize(100);text.setPosition(80, 325);
        window.draw(text);
        window.setView(view);
        window.display();
    }
}

int loadMedia(Music& a, Music& b, Music& c, Music& d, Font& e){
    if (!a.openFromFile("bell.wav")){
        return EXIT_FAILURE;
    }
    if (!b.openFromFile("crash.wav")){
        return EXIT_FAILURE;
    }
    if (!c.openFromFile("water.wav")){
        return EXIT_FAILURE;
    }
    if (!d.openFromFile("gameintro.wav")){
        return EXIT_FAILURE;
    }
    if (!e.loadFromFile("sansation.ttf")) {
        return EXIT_FAILURE;
    }
}
