#ifndef AMOGUS2_PLAYER_H
#define AMOGUS2_PLAYER_H
#include <SFML/Graphics.hpp>
#include "entity.h"

using namespace sf;

class PLAYER: public Entity{
public:
    enum {stay, run, jump, kick} STATE;
    bool shoot, hit;
    std::map<std::string, bool> key;
    int ammo;

    PLAYER(AnimationManager &a, std::string map[], int x, int y): Entity(a, x ,y){
        option("Player", 0, 100, "stay");
        STATE = stay; shoot = false;
        ammo = 10;
    }

    void Keyboard() {
        if (key["ML"]) {
            dx = 0;
            shoot = true;
            anim.play("shoot");
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
        if (key["MR"]) {
            dx = 0;
            STATE = kick;
            hit = true;
            anim.play("kick");
        }

        if (!(key["R"] || key["L"]))
        {
            dx=0;
            if (STATE==run) STATE=stay;
        }
        key["R"]=key["L"]=key["Up"]=key["MR"]=key["ML"]=false;
    }

    void Animation(float time)
    {
        if (STATE==stay) anim.set("stay");
        if (STATE==run) anim.set("run");
        if (STATE==jump) { anim.set("jump"); }
        if (STATE==kick) anim.set("kick");
        if (!life) { anim.set("die"); }

        if (shoot) {timer+=time;
            if (timer>timer_end) {shoot=false; timer=0;}
            anim.set("shoot");
        }

        if (dir) anim.flip();

        anim.tick(time);
    }

    void update(float time)
    {
        if (Health <= 0){
            life = false;
        }
        if (Health > 100) Health = 100;
        if (ammo > 15) Health = 15;
        if (life) {
            Keyboard();
            if (STATE == kick &&
                anim.animList["kick"].currentFrame + 0.004 * time < anim.animList["kick"].frames.size())
                key["MR"] = true;
        }
        Animation(time);
        dy+=0.0005*time;

        x+=dx*time;
        Collision(0);

        y+=dy*time;
        Collision(1);
    }

    void Collision(int dir) {
        if (x < 0)  x = 0;
        if (x + 140 > width) x = width - 140;
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
