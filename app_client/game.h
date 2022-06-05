#ifndef AMOGUS2_GAME_H
#define AMOGUS2_GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include "map.h"
#include "animations.h"
#include "entity.h"
#include "bullet.h"
#include "player.h"
#include "HealthBar.h"
using namespace sf;

void RunGame() {
    RenderWindow window(VideoMode(int(width), int(height)), "Sheesh");

    Texture character_t, bullet_t, bg, block1, block2;
    character_t.loadFromFile("Biker.png");
    bg.loadFromFile("bg.png");
    bullet_t.loadFromFile("bullet.png");
    block1.loadFromFile("block.png");
    block2.loadFromFile("block2.png");

    AnimationManager anim;
    anim.create("jump", character_t, 0, 138 * 3 + 9 + 2, 135, 130, 4, 0.003, 135);
    anim.animList["jump"].loop = 0;
    anim.create("run", character_t, 0, 138 * 4 + 9 + 3, 140, 130, 6, 0.005, 140);
    anim.create("stay", character_t, 60, 138 * 2 + 9 + 1, 80, 130, 4, 0.002, 140);
    anim.create("die", character_t, 0, 138 * 1 + 9, 140, 130, 6, 0.003, 140);
    anim.animList["die"].loop = 0;
    anim.create("kick", character_t, 0, 138 * 0 + 8, 136, 130, 6, 0.004, 136);
    anim.animList["kick"].loop = 0;

    AnimationManager anim2;
    anim2.create("move", bullet_t, 0, 0, 0, 0, 0, 0);
    anim2.create("explode", bullet_t, 0,0,0,0,0,0.01,0, false);

    Sprite background(bg);
    background.setOrigin(bg.getSize().x/2, bg.getSize().y/2);

    Sprite platform(block1);
    Sprite ground(block2);

    std::list<Entity*>  entities;
    std::list<Entity*>::iterator it;

    PLAYER player(anim, TileMap, 100, height - 290);
    HealthBar healthbar;

    Clock clock;
    //RectangleShape Map(Vector2f(40, 40));

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
//            if (event.type == Event::MouseButtonPressed)
//                if (event.key.code == Mouse::Left)
//                    entities.push_back(new Bullet(anim2, TileMap, player.x, player.y, player.dir));
        }

        if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) player.key["L"] = true;
        if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) player.key["R"] = true;
        if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) {
                if (player.STATE == PLAYER::stay) player.key["Up"] = true; }
        //if (Mouse::isButtonPressed(Mouse::Right)) player.key["MR"] = true;
        //if (Mouse::isButtonPressed(Mouse::Left)) player.key["ML"] = true;

        for (it = entities.begin(); it != entities.end();) {
            Entity *b = *it;
            b->update(time);
            if (b->life == false) {it = entities.erase(it); delete b;}
            else it++;
        }

        player.update(time);
        healthbar.update(player.Health);

        View view( FloatRect(0, 0, 1920, 1000) );
        background.setPosition(view.getCenter());
        window.draw(background);

        for(it=entities.begin();it!=entities.end();it++)
            (*it)->draw(window);

        for (int i =0; i < 25; ++i)
            for (int j=0; j < 48; ++j) {
                if (TileMap[i][j]=='B') { platform.setPosition(j*40, i*40); window.draw(platform); }
                if (TileMap[i][j]=='0') { ground.setPosition(j*40, i*40); window.draw(ground); }
                if (TileMap[i][j]==' ') continue;
            }
        player.draw(window);
        healthbar.draw(window);

        window.display();
    }
}

#endif
