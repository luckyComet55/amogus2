#ifndef AMOGUS2_HEALTHBAR_H
#define AMOGUS2_HEALTHBAR_H

#include <SFML/Graphics.hpp>
using namespace sf;

class HealthBar {
public:
    int max;
    RectangleShape bar;

    HealthBar() {
        bar.setFillColor(Color(100,0,0));
        max = 100;
    }

    void update(int k) {
        if (k > 0) if (k < max) bar.setSize(Vector2f(10, (max - k)*70/max));
    }

    void draw(RenderWindow &window)
    {
        Vector2f center = window.getView().getCenter();
        Vector2f size = window.getView().getSize();

        bar.setPosition( center.x - size.x/2 + 11, center.y - size.y/2 + 13);
        window.draw(bar);
    }
};

#endif
