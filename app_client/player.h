#ifndef AMOGUS2_PLAYER_H
#define AMOGUS2_PLAYER_H
#include <SFML/Graphics.hpp>
#include "entity.h"
using namespace sf;
class PLAYER: public Entity{
public:
    enum {stay, run, jump, kick, die} STATE;
    bool shoot, hit;
    std::map<std::string, bool> key;

    PLAYER(AnimationManager &a, std::string map[], int x, int y): Entity(a, x ,y){
        option("Player", 0, 100, "stay");
        STATE = stay; hit = false;
    }

    void Keyboard() {
        if (key["ML"]) {
            shoot = true;
        }
        if (key["MR"]) {
            STATE = kick;
            hit = true;
            anim.play("kick");
        }
        if (key["Up"]) {
            dy = -0.5;
            STATE = jump;
            anim.play("jump");
        }
        if (key["L"]) {
            dir = 1;
            dx = -0.2;
            if (STATE == stay) STATE = run;
        }
        if (key["R"]) {
            dir = 0;
            dx = 0.2;
            if (STATE == stay) STATE = run;
        }

        if (!(key["R"] || key["L"]))
        {
            dx=0;
            if (STATE==run) STATE=stay;
        }
        if (!key["ML"])
        {
            shoot=false;
        }
        key["R"]=key["L"]=key["Up"]=key["MR"]=key["ML"]=false;
    }

    void Animation(float time)
    {
        if (STATE==stay) anim.set("stay");
        if (STATE==run) anim.set("run");
        if (STATE==jump) { anim.set("jump"); }
        if (STATE==kick) anim.set("kick");

        if (shoot) {anim.set("shoot");}

        if (hit) {timer+=time;
            if (timer>1000) {hit=false; timer=0;}
            anim.set("hit");}

        if (dir) anim.flip();

        anim.tick(time);
    }

    void update(float time)
    {
        Keyboard();
        Animation(time);
        dy+=0.0005*time;

        x+=dx*time;
        Collision(0);

        y+=dy*time;
        Collision(1);
    }

    void Collision(int dir) {
        for (int i = y/40; i < (y + h)/40; ++i)
            for (int j = x/40; j < (x + 140)/40; ++j) {
                if (TileMap[i][j]=='B' || TileMap[i][j]=='0') {
                    if ((dx<0) && (dir ==0)) x = j*40 + 40;
                    if ((dx>0) && (dir ==0)) x = j*40 - 140 ;
                    if ((dy>0) && (dir==1)) { y = i*40 - h; dy = 0; STATE = stay;}
                    if ((dy<0) && (dir==1)) { y = i*40 + 40; dy = 0;}
                }
            }
    }
};



#endif
