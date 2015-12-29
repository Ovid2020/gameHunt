#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

class TreeTop: public sf::Drawable, public sf::Transformable {

    sf::Texture m_tileset;
    sf::Vector2f bottomPosition;//set up tree bottoms
    double leafRad;

    public:

    void setLeafRadius(int);

    double getLeafRadius();
    sf::Vector2f getPosition();

    TreeTop(double, double, double);
    TreeTop(sf::Vector2f,double);
    TreeTop();

    private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {

        states.transform *= getTransform();

        states.texture = &m_tileset;

        sf::CircleShape leaf1(leafRad);
        leaf1.setFillColor(Color::Green);
        leaf1.setOrigin(leafRad, leafRad);
        leaf1.setPosition(bottomPosition.x-15,bottomPosition.y-20);

        sf::CircleShape leaf2(leafRad);
        leaf2.setFillColor(Color::Green);
        leaf2.setOrigin(leafRad, leafRad);
        leaf2.setPosition(bottomPosition.x+15,bottomPosition.y-20);

        sf::CircleShape leaf3(leafRad);
        leaf3.setFillColor(Color::Green);
        leaf3.setOrigin(leafRad, leafRad);
        leaf3.setPosition(bottomPosition.x,bottomPosition.y-40);

        target.draw(leaf1, states);
        target.draw(leaf2, states);
        target.draw(leaf3, states);
    }

};


