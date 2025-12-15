#include "Engine.h"
#include <memory>
#include "Sprite.h"
#include <cstdlib>
#include <iostream>
using namespace demo;
using namespace std;

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

class Wall : public Sprite {
public:
    Wall() : Sprite("RedWall.png", 300, 200){}
    void tick() override {}
private:

};

class Boll : public Sprite {
public:
    Boll(int x):Sprite("football.png", x,400){}
    void tick() override {
        move(xSpeed, ySpeed);
        if (getRect().y < 0){
           eng.remove(shared_from_this());
        }
    }
    void onCollisionWith(SpritePtr other) override {
        if (dynamic_pointer_cast<Goal>(other) && !done){
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Grattis!", "Mål!", eng.getWin());
            eng.remove(shared_from_this());
            done = true;
        } // if
    }
	void onKeyDown(const SDL_Event& event) {
		const bool *key_states = SDL_GetKeyboardState(0);
		
		if(key_states[SDL_SCANCODE_W] && key_states[SDL_SCANCODE_D]){
            xSpeed = 5;
            ySpeed = -5;
		}
        if(key_states[SDL_SCANCODE_W] && key_states[SDL_SCANCODE_A]){
            xSpeed = -5;
            ySpeed = -5;
		}
        if(key_states[SDL_SCANCODE_S] && key_states[SDL_SCANCODE_D]){
            xSpeed = 5;
            ySpeed = 5;
		}
        if(key_states[SDL_SCANCODE_S] && key_states[SDL_SCANCODE_A]){
            xSpeed = -5;
            ySpeed = 5;
		}
        if(key_states[SDL_SCANCODE_W]){
            ySpeed = -5;
		}
        if(key_states[SDL_SCANCODE_A]){
            xSpeed = -5;
		}
        if(key_states[SDL_SCANCODE_D]){
            xSpeed = 5;
		}
        if(key_states[SDL_SCANCODE_S]){
            ySpeed = 5;
		}
	}
    void onKeyUp() {
        const bool *key_states = SDL_GetKeyboardState(0);
        if (!key_states[SDL_SCANCODE_A] || !key_states[SDL_SCANCODE_D]) {
            xSpeed = 0;
        }
        if (!key_states[SDL_SCANCODE_W] || !key_states[SDL_SCANCODE_S]) {
            ySpeed = 0;
        }
    }
private:
    bool done = false;
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
    //eng.add(goal);
    eng.run();
}
