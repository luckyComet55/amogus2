#ifndef AMOGUS2_GAME_H
#define AMOGUS2_GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <ctime>
#include "map.h"
#include "animations.h"
#include "entity.h"
#include "bullet.h"
#include "player.h"
#include "HealthBar.h"
#include "loot.h"
using namespace sf;

void RunGame(RenderWindow &window, Texture &character_t) {
    srand(time(NULL));

    Texture bullet_t, bg, block1, block2, ammo_t, heal_t, patron_t;
    bg.loadFromFile("bg.png");
    bullet_t.loadFromFile("bullet.png");
    block1.loadFromFile("block.png");
    block2.loadFromFile("block2.png");
    heal_t.loadFromFile("heal.png");
    patron_t.loadFromFile("patron.png");

    AnimationManager anim;
    anim.create("jump", character_t, 0, 138 * 3 + 9 + 2, 135, 130, 4, 0.004, 135, false);
    anim.create("run", character_t, 0, 138 * 4 + 9 + 3, 140, 130, 6, 0.005, 140);
    anim.create("stay", character_t, 60, 138 * 2 + 9 + 1, 80, 130, 4, 0.002, 140);
    anim.create("die", character_t, 0, 138 * 1 + 9, 140, 130, 6, 0.003, 140, false);
    anim.create("kick", character_t, 0, 138 * 0 + 8, 136, 130, 6, 0.004, 136, false);
    anim.create("shoot", character_t, 0, 138 * 5 + 9 + 4, 135, 130, 1, 0.1, 0, false);

    AnimationManager anim2;
    anim2.create("move", bullet_t, 0, 1, 10, 10, 1, 0.1);
    anim2.create("explode", bullet_t, 12,0,12,12,1,0.01,0, false);

    AnimationManager anim3;
    anim3.create("be", heal_t, 0, 0, 40, 40, 1, 0);
    AnimationManager anim4;
    anim4.create("be", patron_t, 0, 0, 40, 40, 1, 0);

    Sprite background(bg);
    background.setOrigin(bg.getSize().x/2, bg.getSize().y/2);

    Sprite platform(block1);
    Sprite ground(block2);

    std::list<Entity*>  entities;
    std::list<Entity*>::iterator it;

    PLAYER player(anim, TileMap, 100, height - 290);
    HealthBar healthbar;
    ammo_t.loadFromFile("ammo.png");
    Sprite ammo(ammo_t);
    ammo.setTextureRect(IntRect(0,0,40*player.ammo,40));
    ammo.setPosition(0,height-40);

    Clock clock;

    float timer = 0, timer_end = 400, timer_heal = 100, timer_patron = 200;
    bool isPatron=false, isHeal=false;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time/800;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed or Keyboard::isKeyPressed(Keyboard::Escape))
                window.close();
            if (player.life && event.type == Event::MouseButtonPressed && player.dy == 0)
                if (event.key.code == Mouse::Left)
                    timer = 0;
            if (event.type == Event::KeyPressed)
                if (event.key.code == Keyboard::F)
                    entities.push_back(new Bullet(anim2, TileMap, 1000, 650, 1));
        }

        if (!isHeal) {
            if (timer_heal > timer_end*20) timer_heal = 0;
            if (timer_heal == 0) {
                int i = rand() % 24, j = rand() % 47;
                while (TileMap[i][j] == 'B' || TileMap[i][j] == '0') {
                    if (TileMap[i][j] == 'B' || TileMap[i][j] == '0') i = rand() % 24;
                    if (TileMap[i][j] == 'B' || TileMap[i][j] == '0') j = rand() % 47;
                }
                if (TileMap[i][j] != 'B' && TileMap[i][j] != '0') {
                    entities.push_back(new Loot(anim3, TileMap, j * 40, i * 40, 1));
                    isHeal = true;
                }
            }
            timer_heal += time;
        }

        if (!isPatron) {
            if (timer_patron > timer_end*20) timer_patron = 0;
            if (timer_patron == 0) {
                int i = rand() % 24, j = rand() % 47;
                while (TileMap[i][j] == 'B' || TileMap[i][j] == '0') {
                    if (TileMap[i][j] == 'B' || TileMap[i][j] == '0') i = rand() % 24;
                    if (TileMap[i][j] == 'B' || TileMap[i][j] == '0') j = rand() % 47;
                }
                if (TileMap[i][j] != 'B' && TileMap[i][j] != '0') {
                    entities.push_back(new Loot(anim4, TileMap, j * 40, i * 40, 2));
                    isPatron = true;
                }
            }
            timer_patron += time;
        }

        if ((Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) && !player.shoot) player.key["L"] = true;
        if ((Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) && !player.shoot) player.key["R"] = true;
        if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Space)) {
                if (player.STATE == PLAYER::stay) player.key["Up"] = true; }
        if (Mouse::isButtonPressed(Mouse::Right))  player.key["MR"] = true;
        if (Mouse::isButtonPressed(Mouse::Left) && player.dy == 0) {
            if (player.ammo > 0){
                player.key["ML"] = true;
                if (timer > timer_end) timer = 0;
                if (timer == 0) {
                    entities.push_back(
                            new Bullet(anim2, TileMap, player.x - (player.dir - 1) * 135, player.y + 35, player.dir));
                            player.ammo -= 1;
                }
                timer += time;
            }
        }

        for (it = entities.begin(); it != entities.end();) {
            Entity *b = *it;
            if (player.life && b->y >= player.y && b->y <= player.y+130) {
                if (b->x > player.x + 10 && b->x < player.x + 70) {
                    if (b->Health == 1 && player.Health < 100) {
                        player.Health += 40;
                        if (player.Health > 100) player.Health = 100;
                        isHeal = false;
                        b->Health = 0;
                    }
                    if (b->Health == 2 && player.ammo < 15) {
                        player.ammo += 3;
                        if (player.ammo > 15) player.ammo = 15;
                        isPatron = false;
                        b->Health = 0;
                    }
                    if (b->Health == 10) {
                        player.Health -= 10;
                        b->Health = 0;
                    }
                }
            }
            b->update(time);
            if (b->life == false) {it = entities.erase(it); delete b;}
            else it++;
        }

        player.update(time);
        healthbar.update(player.Health);

        View view( FloatRect(0, 0, 1920, 1000) );
        background.setPosition(view.getCenter());
        window.draw(background);

        for (int i =0; i < 25; ++i)
            for (int j=0; j < 48; ++j) {
                if (TileMap[i][j]=='B') { platform.setPosition(j*40, i*40); window.draw(platform); }
                if (TileMap[i][j]=='0') { ground.setPosition(j*40, i*40); window.draw(ground); }
                if (TileMap[i][j]==' ') continue;
            }

        player.draw(window);

        if (!player.dir && (player.dx > 0 || player.dy != 0))
            healthbar.draw(window, player.x, player.y, 1);
        else if (player.shoot || player.hit)
            healthbar.draw(window, player.x, player.y, 0.5);
        else
            healthbar.draw(window, player.x, player.y, 0);

        for(it=entities.begin();it!=entities.end();it++)
            (*it)->draw(window);

        ammo.setTextureRect(IntRect(0,0,40*player.ammo,40));
        window.draw(ammo);

        window.display();
    }
}

#endif
