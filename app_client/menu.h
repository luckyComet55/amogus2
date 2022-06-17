#ifndef AMOGUS2_MENU_H
#define AMOGUS2_MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "map.h"
#include "game.h"

using namespace sf;

void GameOptions(RenderWindow &window, Texture &character_t, Music &music) {
    int currentKey = 0, skin = 0;
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
    Sprite heads(heads_t);
    heads.setTextureRect(IntRect(0,0,80,80));
    heads.setPosition(23*40,10*40);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S) {
                    currentKey += 1;
                    if (currentKey > 2) currentKey = 0;
                }
                if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W) {
                    currentKey -= 1;
                    if (currentKey < 0) currentKey = 2;
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
                if (currentKey == 1) {  // skin
                    if (event.key.code == Keyboard::Left || event.key.code == Keyboard::D) {
                        skin -= 1;
                        if (skin < 0) skin = 5;
                    }
                    if (event.key.code == Keyboard::Right || event.key.code == Keyboard::A) {
                        skin += 1;
                        if (skin > 5) skin = 0;
                    }
                    character_t.loadFromFile(skins[skin]);
                }
                if (currentKey == 2 && event.key.code == Keyboard::Enter || event.key.code == Keyboard::Escape) return;
            }
        }

        View view( FloatRect(0, 0, 1920, 1000) );
        background.setPosition(view.getCenter());
        window.draw(background);
        heads.setTextureRect(IntRect(0+80*skin,0,80,80));
        window.draw(heads);
        musicbar.setTextureRect(IntRect(0,0,4*music.getVolume(), 40));
        window.draw(musicbar);
        frame.setPosition(18*40, 5*40+4*40*currentKey);
        window.draw(frame);
        window.display();
    }
}

void Menu() {
    RenderWindow window(VideoMode(int(width), int(height)), "SheeshMenu");
    Texture bg, frame_t, character_t;
    bg.loadFromFile("mainmenu.png");

    character_t.loadFromFile("Biker.png");

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
                    if (currentKey == 0) RunGame(window, character_t);
                    //if (currentKey == 1) Registration();
                    if (currentKey == 2) GameOptions(window, character_t, music);
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
