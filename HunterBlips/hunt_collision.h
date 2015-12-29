#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>

#include "Hunter.h"

const int Hunt_Num = 30;

using namespace std;
using namespace sf;

void hunt_collision(vector<Hunter>&hunters, double h_move[Hunt_Num][2], vector<string>&run_tog, int tog_counter[]);
