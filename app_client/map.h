#ifndef AMOGUS2_MAP_H
#define AMOGUS2_MAP_H
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace sf;

float width=1920, height=1000;

std::string TileMap[25] = {
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
        "                                                ",
        "                                                ",
        "                                                ",
        "                                                ",
        "                                                ",
        "                                                ",
        "                                                ",
        "BBBBBBBB      BBBBBBBBBBBBBBBB      BBBBBBBBBBBB",
        "                                                ",
        "                                                ",
        "                                                ",
        "                                                ",
        "       BBBBBBB      BBBBBBBBBBBBBBBBBB          ",
        "                                                ",
        "                                                ",
        "                                                ",
        "                                                ",
        "BBBBBBBBBBBBBBBBBBBBBB       BBBBBBBB    BBBBBBB",
        "                                                ",
        "                                                ",
        "                                                ",
        "                                                ",
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
        "000000000000000000000000000000000000000000000000"
};

#endif
