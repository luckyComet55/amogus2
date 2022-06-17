#ifndef AMOGUS2_LOOT_H
#define AMOGUS2_LOOT_H

#include "entity.h"

class Loot: public Entity {
public:
    Loot(AnimationManager &a, std::string map[], int x, int y, int type) : Entity(a, x, y) {
        option("Loot", 0, type, "be");
    }

    void update(float time) {

        if (Health <= 0) {
            life = false;
        }
        anim.tick(time);
    }
};
#endif
