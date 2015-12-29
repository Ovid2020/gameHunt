#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

class Star: public sf::Drawable, public sf::Transformable {

    sf::Texture m_tileset;

    double starRad;

    public:

    void setStarRadius(double);

    double getStarRadius();

    Star();
    Star(double);

    private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {

        states.transform *= getTransform();

        states.texture = &m_tileset;
        sf::CircleShape star(starRad);
        star.setOrigin(5,5);
        star.setFillColor(Color(230,230,0));

        target.draw(star, states);
    }
};
