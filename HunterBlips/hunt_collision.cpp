
#include "hunt_collision.h"


void hunt_collision(vector<Hunter>&hunters,double h_move[Hunt_Num][2],vector<string>&run_tog, int tog_counter[])
{
    int i,j;
    bool collide_once;
    double dist, rad;
    sf::Vector2f hunt_xy;
    sf::Vector2f hunt_xy_2;
    for (i = 0; i < hunters.size(); i++) {
        collide_once = false;
        hunt_xy = hunters[i].getPosition();
        for (j = 0; j < hunters.size(); j++) {//finds the distance between two hunters
            if (i == j) {
                //skip action, if both loops refer to same hunter
            }
            else if (collide_once == false) {//only check the current hunter if no collision has occurred
                hunt_xy_2 = hunters[j].getPosition();
                dist = sqrt(((hunt_xy_2.x - hunt_xy.x) * (hunt_xy_2.x - hunt_xy.x) + (hunt_xy_2.y - hunt_xy.y) * (hunt_xy_2.y-hunt_xy.y)) + 1);
                rad = hunters[i].getRadius() + hunters[j].getRadius();
                if (dist <= rad)
                {
                    collide_once = true;//ensures position is reversed only for one collision (don't want it to collide with one, reverse,
                                    //then collide simultaneously with another and reverse back, negating initial reversal).
                    if ((hunters[i].getRadius() <= hunters[j].getRadius()) && (hunters[i].getFillColor() != sf::Color::Blue))//if the distance between two hunters is too close, reverse the "i" hunter's direction
                    {                                                    //Also, only the smaller hunter reports a collision and will reverse directions (in hunters_move)
                        h_move[i][0] = h_move[i][0] * -1;
                        h_move[i][1] = h_move[i][1] * -1;
                        run_tog[i] = "run";
                        tog_counter[i] = 0;
                    }
                    if (hunters[i].getFillColor()==sf::Color::Blue)//Blue hunters consumer other colored hunters.
                    {
                        hunters[i].setSpeed(hunters[i].getSpeed() + (hunters[j].getSpeed() / 2 ));
                        hunters[i].setDamage(hunters[i].getDamage() + (hunters[j].getDamage() / 2));
                        hunters[i].setAccuracy(hunters[i].getAccuracy() + (hunters[j].getAccuracy() * 10));
                        hunters[i].setRadius(hunters[i].getRadius() + (hunters[j].getRadius() / 5));
                        hunters.erase(hunters.begin() + j);
                    }
                    j = hunters.size() - 1; //force j to its last iteration to skip all subsequent collision checks.
                }
            }
        }
    }
}
