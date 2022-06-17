#ifndef AMOGUS2_ANIMATIONS_H
#define AMOGUS2_ANIMATIONS_H
#include <SFML/Graphics.hpp>
using namespace sf;
class Animation {
public:
    std::vector<IntRect> frames, frames_flip;
    float currentFrame, speed;
    bool loop, flip, isPlaying;
    Sprite character;

    Animation() {
        currentFrame = 0;
        isPlaying = true;
        flip = false;
        loop = true;
    }

    void tick(float time) {
        if (!isPlaying) return;
        currentFrame += speed*time;
        if (currentFrame > frames.size()) {
            currentFrame -= frames.size();
            if (!loop) { isPlaying = false; return; }
        }
        int i = (int)currentFrame;
        character.setTextureRect(frames[i]);
        if (flip) character.setTextureRect(frames_flip[i]);
    }
};


class AnimationManager {
public:
    std::string currentAnim;
    std::map <std::string, Animation> animList;
    AnimationManager()
    {}

    ~AnimationManager() {
        animList.clear();
    }

    void create(std::string name, Texture &t, int x, int y, int w, int h, int count, float speed, int step = 0, bool Loop = true) {
        Animation a;
        a.speed=speed;
        a.loop = Loop;
        a.character.setTexture(t);
        a.character.setOrigin(0,(float)h);

        for (int i=0; i<count; ++i) {
            a.frames.emplace_back(IntRect(x+i*step,y,w,h));
            a.frames_flip.emplace_back(IntRect (x+i*step+w,y,-w,h));
        }
        animList[name] = a;
        currentAnim=name;
    }

    void draw(RenderWindow &window, int x=0, int y=0) {
        animList[currentAnim].character.setPosition(x, y);
        window.draw(animList[currentAnim].character);
    }

    void set(std::string name) {
        currentAnim=name;
        animList[currentAnim].flip = 0;
    }
    void flip(bool b = 1) {animList[currentAnim].flip = b;}
    void tick(float time) {animList[currentAnim].tick(time);}
    void pause() {animList[currentAnim].isPlaying = false;}
    void play() {animList[currentAnim].isPlaying = true;}
    void play(std::string name) {animList[name].isPlaying = true;}
    bool isPlaying() {return animList[currentAnim].isPlaying;}
    float getH() {return animList[currentAnim].frames[0].height;}
    float getW() {return animList[currentAnim].frames[0].width;}
    int getCurrFrame() {return animList[currentAnim].currentFrame;}
};


#endif
