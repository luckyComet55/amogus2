#ifndef AMOGUS2_BULLET_H
#define AMOGUS2_BULLET_H

#include "entity.h"

class Bullet: public Entity {
public:
    Bullet(AnimationManager &a, std::string map[], int x, int y, bool dir): Entity(a, x, y) {
        option("Bullet", 0.6, 10, "move");
        if (dir) dx = -0.6;
    }

    void update(float time) {
        x += dx*time;
        for (int i = 0; i < (x + w)/40; ++i)
            if (TileMap[int(y/40)][i] == 'B') Health = 0;
        if (Health <= 0) {
            anim.set("explode");
            dx = 0;
            if (anim.isPlaying() == false) life = false;
        }
        anim.tick(time);
    }
};

#endif
