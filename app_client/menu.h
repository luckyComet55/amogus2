#ifndef AMOGUS2_MENU_H
#define AMOGUS2_MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "map.h"
#include "game.h"

using namespace sf;

void GameOptions(RenderWindow &window, Texture &character_t, Music &music) {
    int currentKey = 1, skin = 0;
    std::string skins[6] = {"Biker.png", "Biker2.png","alien.png","cyborg.png","ledy.png","punk.png"};

    Texture bg;
    bg.loadFromFile("menu.png");
    Sprite background(bg);
    background.setOrigin(bg.getSize().x/2, bg.getSize().y/2);

    Sprite character(character_t);
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
                if (currentKey == 1) {  //sound
//                    if (event.key.code == Keyboard::Left || event.key.code == Keyboard::D) {
//                        music.pause();
//                        if (music.getVolume() > 5) music.setVolume(music.getVolume() - 5);
//                        else music.setVolume(0);
//                        music.play();
//                    }
//                    if (event.key.code == Keyboard::Right || event.key.code == Keyboard::A) {
//                        music.pause();
//                        if (music.getVolume() < 95) music.setVolume(music.getVolume() + 5);
//                        else music.setVolume(100);
//                        music.play();
//                    }
//                    if (event.key.code == Keyboard::M) {
//                        if (music.getStatus() == music.Playing) music.pause();
//                        else if (music.getStatus() == music.Paused) music.play();
//                    }
                }
                if (currentKey == 2) {  // skin
                    if (event.key.code == Keyboard::Left || event.key.code == Keyboard::D) {
                        skin += 1;
                        if (skin > 5) skin = 0;
                    }
                    if (event.key.code == Keyboard::Right || event.key.code == Keyboard::A) {
                        skin -= 1;
                        if (skin < 0) skin = 5;
                    }
                    if (event.key.code == Keyboard::Enter) {
                        character_t.loadFromFile(skins[skin]);
                        Sprite character(character_t);
                    }
                }
                if (currentKey == 0 && event.key.code == Keyboard::Enter || event.key.code == Keyboard::Escape) return;
            }
        }

        View view( FloatRect(0, 0, 1920, 1000) );
        background.setPosition(view.getCenter());
        window.draw(background);
        window.draw(character);
        window.display();
    }
}

void Menu() {
    RenderWindow window(VideoMode(int(width), int(height)), "SheeshMenu");
    Texture bg, keys_t, character_t;
    bg.loadFromFile("bg.png");

    character_t.loadFromFile("Biker.png");

    Sprite background(bg);
    background.setOrigin(bg.getSize().x/2, bg.getSize().y/2);

    //keys.loadFromFile("keys.png");
    int currentKey = 1;

//    Music music;
//    music.openFromFile("Music.ogg");
//    music.setLoop(true);
//    music.play();

    Clock clock;
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time/800;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed //or Keyboard::isKeyPressed(Keyboard::Escape)
            )
                window.close();
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S) {
                    currentKey += 1;
                    if (currentKey > 4) currentKey = 0;
                }
                if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W) {
                    currentKey -= 1;
                    if (currentKey < 0) currentKey = 4;
                }
                if (event.key.code == Keyboard::Enter) {
                    if (currentKey == 1) RunGame(window, character_t);
                    //if (currentKey == 2) JoinGame();
                    //if (currentKey == 3) Registration();
                    //if (currentKey == 4) GameOptions(window, character_t, music);
                    if (currentKey == 0) window.close();
                }
            }
        }

        View view( FloatRect(0, 0, 1920, 1000) );
        background.setPosition(view.getCenter());
        window.draw(background);

        //keys.setPosition(view.getCenter());
        //window.draw(keys);

        window.display();
    }
}

#endif
