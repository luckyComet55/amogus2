cmake_minimum_required(VERSION 3.21)
project(amogus2)

set(CMAKE_CXX_STANDARD 17)

add_executable(amogus2 main.cpp player.h animations.h map.h entity.h game.h HealthBar.h bullet.h menu.h)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics network audio REQUIRED)

include_directories(C:/SFML/include)
target_link_libraries(amogus2 sfml-system sfml-window sfml-graphics sfml-network sfml-audio)