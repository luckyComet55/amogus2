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
        bar.setSize(Vector2f(max/2, 10));
    }

    void update(int k) {
        if (k >= 0) if (k < max) bar.setSize(Vector2f(k/2, 10));
    }

    void draw(RenderWindow &window, float x, float y, float dir)
    {
        bar.setPosition(x + dir * 50, y - 12);
        window.draw(bar);
    }
};

#endif
