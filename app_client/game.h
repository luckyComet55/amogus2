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

void RunGame(RenderWindow &window, Texture &character_t1, Texture &character_t2) {
    srand(time(NULL));

    Texture bullet_t, bg, block1, block2, ammo_t, heal_t, patron_t,gameover_t;
    bg.loadFromFile("bg.png");
    bullet_t.loadFromFile("bullet.png");
    block1.loadFromFile("block.png");
    block2.loadFromFile("block2.png");
    heal_t.loadFromFile("heal.png");
    patron_t.loadFromFile("patron.png");
    gameover_t.loadFromFile("gameover.png");

    AnimationManager anim_p1;
    anim_p1.create("jump", character_t1, 0, 138 * 3 + 9 + 2, 135, 130, 4, 0.004, 135, false);
    anim_p1.create("run", character_t1, 0, 138 * 4 + 9 + 3, 140, 130, 6, 0.005, 140);
    anim_p1.create("stay", character_t1, 60, 138 * 2 + 9 + 1, 80, 130, 4, 0.002, 140);
    anim_p1.create("die", character_t1, 0, 138 * 1 + 9, 140, 130, 6, 0.003, 140, false);
    anim_p1.create("kick", character_t1, 0, 138 * 0 + 8, 136, 130, 6, 0.004, 136, false);
    anim_p1.create("shoot", character_t1, 0, 138 * 5 + 9 + 4, 135, 130, 1, 0.1, 0, false);

    AnimationManager anim_p2;
    anim_p2.create("jump", character_t2, 0, 138 * 3 + 9 + 2, 135, 130, 4, 0.004, 135, false);
    anim_p2.create("run", character_t2, 0, 138 * 4 + 9 + 3, 140, 130, 6, 0.005, 140);
    anim_p2.create("stay", character_t2, 60, 138 * 2 + 9 + 1, 80, 130, 4, 0.002, 140);
    anim_p2.create("die", character_t2, 0, 138 * 1 + 9, 140, 130, 6, 0.003, 140, false);
    anim_p2.create("kick", character_t2, 0, 138 * 0 + 8, 136, 130, 6, 0.004, 136, false);
    anim_p2.create("shoot", character_t2, 0, 138 * 5 + 9 + 4, 135, 130, 1, 0.1, 0, false);

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

    PLAYER player1(anim_p1, TileMap, 100, height - 290);
    HealthBar healthbar1;
    ammo_t.loadFromFile("ammo.png");
    Sprite ammo1(ammo_t);
    ammo1.setTextureRect(IntRect(0, 0, 40 * player1.ammo, 40));
    ammo1.setPosition(0, height - 40);

    PLAYER player2(anim_p2, TileMap, 500, height - 290);
    HealthBar healthbar2;
    Sprite ammo2(ammo_t);
    ammo2.setTextureRect(IntRect(0, 0, 40 * player2.ammo, 40));
    ammo2.setPosition(width - 15*40, height - 40);

    Clock clock;

    float timer1 = 0, timer2 = 0, timer_end = 400, timer_heal = 100, timer_patron = 200;
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
            if (player1.life && event.type == Event::KeyPressed && player1.dy == 0)
                if (event.key.code == Keyboard::E)
                    timer1 = 0;
            if (player2.life && event.type == Event::MouseButtonPressed && player2.dy == 0)
                if (event.key.code == Mouse::Left)
                    timer2 = 0;
//            if (event.type == Event::KeyPressed)
//                if (event.key.code == Keyboard::F)
//                    entities.push_back(new Bullet(anim2, TileMap, 1000, 650, 1));
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

        if ((Keyboard::isKeyPressed(Keyboard::A)) && !player1.shoot) player1.key["L"] = true;
        if ((Keyboard::isKeyPressed(Keyboard::D)) && !player1.shoot) player1.key["R"] = true;
        if (Keyboard::isKeyPressed(Keyboard::W)) {
                if (player1.STATE == PLAYER::stay) player1.key["Up"] = true; }
//        if (Keyboard::isKeyPressed(Keyboard::Q)) {
//            player1.key["MR"] = true;
//            if (player1.hit && player2.y == player1.y &&
//                (player2.x < player1.x + 2 * 100) && (player2.x > player1.x + 100)) {
//                player2.Health -= 10;
//                player1.hit = false;
//            }
//        }
        if (Keyboard::isKeyPressed(Keyboard::E) && player1.dy == 0) {
            if (player1.ammo > 0){
                player1.key["ML"] = true;
                if (timer1 > timer_end) timer1 = 0;
                if (timer1 == 0) {
                    entities.push_back(
                            new Bullet(anim2, TileMap, player1.x - (player1.dir - 1) * 135, player1.y + 35, player1.dir));
                    player1.ammo -= 1;
                }
                timer1 += time;
            }
        }

        if ((Keyboard::isKeyPressed(Keyboard::Left)) && !player2.shoot) player2.key["L"] = true;
        if ((Keyboard::isKeyPressed(Keyboard::Right)) && !player2.shoot) player2.key["R"] = true;
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (player2.STATE == PLAYER::stay) player2.key["Up"] = true; }
//        if (Mouse::isButtonPressed(Mouse::Right)) player2.key["MR"] = true;
        if (Mouse::isButtonPressed(Mouse::Left) && player2.dy == 0) {
            if (player2.ammo > 0){
                player2.key["ML"] = true;
                if (timer2 > timer_end) timer2 = 0;
                if (timer2 == 0) {
                    entities.push_back(
                            new Bullet(anim2, TileMap, player2.x - (player2.dir - 1) * 135, player2.y + 35, player2.dir));
                    player2.ammo -= 1;
                }
                timer2 += time;
            }
        }

        for (it = entities.begin(); it != entities.end();) {
            Entity *b = *it;
            if (player1.life && b->y >= player1.y && b->y <= player1.y + 130) {
                if (b->x > player1.x + 10 && b->x < player1.x + 70) {
                    if (b->Health == 1 && player1.Health < 100) {
                        player1.Health += 40;
                        if (player1.Health > 100) player1.Health = 100;
                        isHeal = false;
                        b->Health = 0;
                    }
                    if (b->Health == 2 && player1.ammo < 15) {
                        player1.ammo += 3;
                        if (player1.ammo > 15) player1.ammo = 15;
                        isPatron = false;
                        b->Health = 0;
                    }
                    if (b->Health == 10) {
                        player1.Health -= 10;
                        b->Health = 0;
                    }
                }
            }
            if (player2.life && b->y >= player2.y && b->y <= player2.y + 130) {
                if (b->x > player2.x + 10 && b->x < player2.x + 70) {
                    if (b->Health == 1 && player2.Health < 100) {
                        player2.Health += 40;
                        if (player2.Health > 100) player2.Health = 100;
                        isHeal = false;
                        b->Health = 0;
                    }
                    if (b->Health == 2 && player2.ammo < 15) {
                        player2.ammo += 3;
                        if (player2.ammo > 15) player2.ammo = 15;
                        isPatron = false;
                        b->Health = 0;
                    }
                    if (b->Health == 10) {
                        player2.Health -= 10;
                        b->Health = 0;
                    }
                }
            }
            b->update(time);
            if (b->life == false) {it = entities.erase(it); delete b;}
            else it++;
        }

        player1.update(time);
        healthbar1.update(player1.Health);

        player2.update(time);
        healthbar2.update(player2.Health);

        View view( FloatRect(0, 0, 1920, 1000) );
        background.setPosition(view.getCenter());
        window.draw(background);

        for (int i =0; i < 25; ++i)
            for (int j=0; j < 48; ++j) {
                if (TileMap[i][j]=='B') { platform.setPosition(j*40, i*40); window.draw(platform); }
                if (TileMap[i][j]=='0') { ground.setPosition(j*40, i*40); window.draw(ground); }
                if (TileMap[i][j]==' ') continue;
            }

        player1.draw(window);
        player2.draw(window);

        if (!player1.dir && (player1.dx > 0 || player1.dy != 0))
            healthbar1.draw(window, player1.x, player1.y, 1);
        else if (player1.shoot || player1.hit)
            healthbar1.draw(window, player1.x, player1.y, 0.5);
        else
            healthbar1.draw(window, player1.x, player1.y, 0);

        if (!player2.dir && (player2.dx > 0 || player2.dy != 0))
            healthbar2.draw(window, player2.x, player2.y, 1);
        else if (player2.shoot || player2.hit)
            healthbar2.draw(window, player2.x, player2.y, 0.5);
        else
            healthbar2.draw(window, player2.x, player2.y, 0);

        for(it=entities.begin();it!=entities.end();it++)
            (*it)->draw(window);

        ammo1.setTextureRect(IntRect(0, 0, 40 * player1.ammo, 40));
        window.draw(ammo1);
        ammo2.setTextureRect(IntRect(0, 0, 40 * player2.ammo, 40));
        window.draw(ammo2);
        if (!player1.life || !player2.life) {
            Texture gameover_t;
            gameover_t.loadFromFile("gameover.png");
            Sprite gameover(gameover_t);
            gameover.setPosition(view.getCenter());
            window.draw(gameover);
        }

        window.display();
    }
}

#endif
