#include "Engine.h"
#include <memory>
#include "Sprite.h"
#include <cstdlib>
#include <iostream>
using namespace demo;
using namespace std;

class Boll;

class Goal : public Sprite{
public:
    Goal():Sprite("goal.png",10,0){}
    void tick() override {
        if (static_cast<double>(rand()) / RAND_MAX < 0.01 || getRect().x > cnts::gScreenWidth - getRect().w || getRect().x < 0)
            direction = -direction;
        move(5*direction,0);
    }
private:
    int direction = 1;
};

//probably move to cpp and h files
class Wall : public Sprite {
public:
    Wall() : Sprite("RedWall.png", 300, 200){}
    void tick() override {}
private:
};

//probably move to cpp and h files
class Enemy : public Sprite {
public:
    Enemy() : Sprite("GreenEnemy.png", 100, 100){}
    void tick() override {
        move(xSpeed, ySpeed);
    }
    
    void interactWith(SpritePtr other) override {
        if(dynamic_pointer_cast<Boll>(other)){
            //spaceship operator?
            if(getRect().x < other->getRect().x){ xSpeed = 1; }
            if(getRect().x > other->getRect().x){ xSpeed = -1; }
            if(getRect().x == other->getRect().x){ xSpeed = 0; }

            if(getRect().y < other->getRect().y){ ySpeed = 1; }
            if(getRect().y > other->getRect().y){ ySpeed = -1; }
            if(getRect().y == other->getRect().y){ ySpeed = 0; }
        }
    }

    void onCollisionWith(SpritePtr other) override {
        if (dynamic_pointer_cast<Wall>(other)){
            move(xSpeed * -1, ySpeed * -1);

            xSpeed = 0;
            ySpeed = 0; 
                      
        }
    }
private:
    //maybe should already be declared in a moveable sprite class?
    int xSpeed = 1;
    int ySpeed = 1;
};

//probably move to cpp and h files
class Boll : public Sprite {
public:
    Boll(int x):Sprite("football.png", x,400){}
    void tick() override {
        move(xSpeed, ySpeed);
        /*if (getRect().y < 0){
           eng.remove(shared_from_this());
        }*/
    }

    void onCollisionWith(SpritePtr other) override {
        if(dynamic_pointer_cast<Enemy>(other)){
            eng.remove(shared_from_this());
        }
        
        if (dynamic_pointer_cast<Wall>(other)){
            move(xSpeed * -1, ySpeed * -1);

            xSpeed = 0;
            ySpeed = 0; 
                      
        } //wall collision
    }

	void onKeyDown(const SDL_Event& event) {
		const bool *key_states = SDL_GetKeyboardState(0);

        if(key_states[SDL_SCANCODE_W]){
            ySpeed = -5;
        }
        if(key_states[SDL_SCANCODE_W] && key_states[SDL_SCANCODE_D]){
            xSpeed = 5;
            ySpeed = -5;
        }
        if(key_states[SDL_SCANCODE_D]){
            xSpeed = 5;
        }
        if(key_states[SDL_SCANCODE_S] && key_states[SDL_SCANCODE_D]){
            xSpeed = 5;
            ySpeed = 5;
        }
        if(key_states[SDL_SCANCODE_S]){
            ySpeed = 5;
        }
        if(key_states[SDL_SCANCODE_S] && key_states[SDL_SCANCODE_A]){
            xSpeed = -5;
            ySpeed = 5;
        }
        if(key_states[SDL_SCANCODE_A]){
            xSpeed = -5;
        }
        if(key_states[SDL_SCANCODE_W] && key_states[SDL_SCANCODE_A]){
            xSpeed = -5;
            ySpeed = -5;
        }
	}
    void onKeyUp() {
        const bool *key_states = SDL_GetKeyboardState(0);
        if (!key_states[SDL_SCANCODE_A] && !key_states[SDL_SCANCODE_D]) {
            xSpeed = 0;
        }
        if (!key_states[SDL_SCANCODE_W] && !key_states[SDL_SCANCODE_S]) {
            ySpeed = 0;
        }
    }
private:
    int xSpeed = 0;
    int ySpeed = 0;
};

/*
class Kicker : public Sprite{
public:
    void tick() override {}
    void onMouseDown(const SDL_Event& event) override{
        SpritePtr spr = SpritePtr(new Boll(event.button.x));
        eng.add(spr);
    }
};
*/

int main(){
    SpritePtr ball = SpritePtr(new Boll(300));
    //SpritePtr goal = SpritePtr(new Goal);
    
    eng.add(ball);
    eng.add(SpritePtr(new Wall()));
    eng.add(SpritePtr(new Enemy()));
    //eng.add(goal);
    eng.run();
}
