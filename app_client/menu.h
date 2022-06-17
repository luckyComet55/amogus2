#ifndef AMOGUS2_MENU_H
#define AMOGUS2_MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "map.h"
#include "game.h"

using namespace sf;

void GameOptions(RenderWindow &window, Texture &character_t1, Texture &character_t2, Music &music) {
    int currentKey = 0, skin1 = 5, skin2 = 4;
    std::string skins[6] = {"punk.png", "alien.png", "ledy.png", "cyborg-1.png", "Biker2.png", "Biker.png"};

    Texture bg, frame_t, heads_t, musicbar_t;
    bg.loadFromFile("settings.png");
    Sprite background(bg);
    background.setOrigin(bg.getSize().x/2, bg.getSize().y/2);

    frame_t.loadFromFile("frame.png");
    Sprite frame(frame_t);
    frame.setPosition(18*40, 5*40+4*40*currentKey);

    musicbar_t.loadFromFile("musicbar.png");
    Sprite musicbar(musicbar_t);
    musicbar.setPosition(19*40, 6*40);
    musicbar.setTextureRect(IntRect(0,0,4*music.getVolume(), 40));

    heads_t.loadFromFile("heads.png");
    Sprite heads1(heads_t);
    heads1.setTextureRect(IntRect(0, 0, 80, 80));
    heads1.setPosition(23 * 40, 10 * 40);
    Sprite heads2(heads_t);
    heads2.setTextureRect(IntRect(0, 0, 80, 80));
    heads2.setPosition(23 * 40, 14 * 40);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S) {
                    currentKey += 1;
                    if (currentKey > 3) currentKey = 0;
                }
                if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W) {
                    currentKey -= 1;
                    if (currentKey < 0) currentKey = 3;
                }
                if (currentKey == 0) {  //sound
                    if (event.key.code == Keyboard::Left || event.key.code == Keyboard::D) {
                        if (music.getVolume() > 5) music.setVolume(music.getVolume() - 5);
                        else music.setVolume(0);
                    }
                    if (event.key.code == Keyboard::Right || event.key.code == Keyboard::A) {
                        if (music.getVolume() < 95) music.setVolume(music.getVolume() + 5);
                        else music.setVolume(100);
                    }
                    if (event.key.code == Keyboard::M) {
                        if (music.getStatus() == music.Playing) music.pause();
                        else if (music.getStatus() == music.Paused) music.play();
                    }
                }
                if (currentKey == 1) {  // skin1
                    if (event.key.code == Keyboard::Left || event.key.code == Keyboard::D) {
                        skin1 -= 1;
                        if (skin1 < 0) skin1 = 5;
                    }
                    if (event.key.code == Keyboard::Right || event.key.code == Keyboard::A) {
                        skin1 += 1;
                        if (skin1 > 5) skin1 = 0;
                    }
                    character_t1.loadFromFile(skins[skin1]);
                }
                if (currentKey == 2) {  // skin2
                    if (event.key.code == Keyboard::Left || event.key.code == Keyboard::D) {
                        skin2 -= 1;
                        if (skin2 < 0) skin2 = 5;
                    }
                    if (event.key.code == Keyboard::Right || event.key.code == Keyboard::A) {
                        skin2 += 1;
                        if (skin2 > 5) skin2 = 0;
                    }
                    character_t2.loadFromFile(skins[skin2]);
                }
                if (currentKey == 3 && event.key.code == Keyboard::Enter || event.key.code == Keyboard::Escape) return;
            }
        }

        View view( FloatRect(0, 0, 1920, 1000) );
        background.setPosition(view.getCenter());
        window.draw(background);
        heads1.setTextureRect(IntRect(0 + 80 * skin1, 0, 80, 80));
        window.draw(heads1);
        heads2.setTextureRect(IntRect(0 + 80 * skin2, 0, 80, 80));
        window.draw(heads2);
        musicbar.setTextureRect(IntRect(0,0,4*music.getVolume(), 40));
        window.draw(musicbar);
        frame.setPosition(18*40, 5*40+4*40*currentKey);
        window.draw(frame);
        window.display();
    }
}

void Menu() {
    RenderWindow window(VideoMode(int(width), int(height)), "SheeshMenu");
    Texture bg, frame_t, character_t1, character_t2;
    bg.loadFromFile("mainmenu.png");

    character_t1.loadFromFile("Biker.png");
    character_t2.loadFromFile("Biker2.png");

    Sprite background(bg);
    background.setOrigin(bg.getSize().x/2, bg.getSize().y/2);


    int currentKey = 0;

    frame_t.loadFromFile("frame.png");
    Sprite frame(frame_t);
    frame.setPosition(18*40, 5*40+4*40*currentKey);

    Music music;
    music.openFromFile("Music.ogg");
    music.setLoop(true);
    music.setVolume(50);
    music.play();

    Clock clock;
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time/800;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S) {
                    currentKey += 1;
                    if (currentKey > 3) currentKey = 0;
                }
                if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W) {
                    currentKey -= 1;
                    if (currentKey < 0) currentKey = 3;
                }
                if (event.key.code == Keyboard::Enter) {
                    if (currentKey == 0) RunGame(window, character_t1, character_t2);
                    //if (currentKey == 1) Registration();
                    if (currentKey == 2) GameOptions(window, character_t1, character_t2, music);
                    if (currentKey == 3) window.close();
                }
            }
        }

        View view( FloatRect(0, 0, 1920, 1000) );
        background.setPosition(view.getCenter());
        window.draw(background);

        frame.setPosition(18*40, 5*40+4*40*currentKey);
        window.draw(frame);

        window.display();
    }
}

#endif
