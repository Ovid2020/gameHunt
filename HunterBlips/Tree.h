#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

class Tree: public sf::Drawable, public sf::Transformable {

    sf::Texture m_tileset;
    sf::Vector2f trunkPosition;
    int treeWid = 20, treeHei = 100;      //set up tree trunks

    public:

    void setWidth(int);
    void setHeight(int);

    int getWidth();
    int getHeight();
//    sf::Vector2f getPosition();

    Tree(double a, double b);
    Tree(sf::Vector2f);
    Tree();

    private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {

        states.transform *= getTransform();

        states.texture = &m_tileset;

        sf::RectangleShape trunk(sf::Vector2f(treeWid,treeHei));
        trunk.setFillColor(Color(102,51,0));
        trunk.setOrigin(treeWid/2,treeHei/2);
        trunk.setOrigin(treeWid/2,treeHei/2);
        trunk.setPosition(trunkPosition);

        target.draw(trunk, states);

    }

};

