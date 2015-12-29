#include <SFML/Graphics.hpp>

class Hunter: public sf::Drawable, public sf::Transformable {

    sf::Texture m_tileset;
    double accuracy, speed, radius, damage;
    sf::Color color;

    public:

    void setAccuracy(double);
    void setSpeed(double);
    void setRadius(double);
    void setDamage(double);
    void setFillColor(sf::Color);

    double getAccuracy();
    double getSpeed();
    double getRadius();
    double getDamage();
    sf::Color getFillColor();

    Hunter();
    Hunter(double,double,double,double,sf::Color);

    private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();

        states.texture = &m_tileset;

        sf::CircleShape shape(radius);
        shape.setFillColor(color);

        target.draw(shape, states);
    }
};
