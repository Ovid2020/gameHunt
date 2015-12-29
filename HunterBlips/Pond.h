#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

class Pond: public sf::Drawable, public sf::Transformable {


    sf::Texture m_tileset;
    double pondRad = 45;

    public:

    void setPondRadius(double);

    double getPondRadius();

    Pond();
    Pond(double);

    private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {

        states.transform *= getTransform();

        states.texture = &m_tileset;

        CircleShape pond(pondRad);
        pond.setOrigin(pondRad,pondRad);
        pond.setFillColor(Color(0,102,224));
        pond.setScale(1.2f, 1.0f);

        target.draw(pond, states);
    }
};
