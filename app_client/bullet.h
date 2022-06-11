#ifndef AMOGUS2_BULLET_H
#define AMOGUS2_BULLET_H

#include "entity.h"

class Bullet: public Entity {
public:
    Bullet(AnimationManager &a, std::string map[], int x, int y, bool dir): Entity(a, x, y) {
        option("Bullet", 0.6, 1, "move");
        if (dir) dx = -0.6;
    }

    void update(float time) {
        x += dx*time;
        if (x < -12 || x > width) Health = 0;
//        if (y >= pl_y && y <= pl_y+130) {
//            if (x > pl_x + 10 && x < pl_x + 70) {
//                Health = 0;
//            }
//        }
        else for (int i = 0; i < (x + w)/40; ++i)
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
