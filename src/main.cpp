#include "Engine.h"
#include <memory>
#include "Sprite.h"
#include "Label.h"
#include <cstdlib>
#include <random>
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
    Enemy(int x, int y) : Sprite("GreenEnemy.png", x, y){}
    void tick() override {
        move(xSpeed, ySpeed);
        xCollision = false;
        yCollision = false;
    }
    
    void interactWith(SpritePtr other) override {
        if(dynamic_pointer_cast<Boll>(other)){
            //spaceship operator?
            if(!xCollision){
                if(getRect().x < other->getRect().x){ xSpeed = 1; }
                if(getRect().x > other->getRect().x){ xSpeed = -1; }
                if(getRect().x == other->getRect().x){ xSpeed = 0; }
            }

            if(!yCollision){
                if(getRect().y < other->getRect().y){ ySpeed = 1; }
                if(getRect().y > other->getRect().y){ ySpeed = -1; }
                if(getRect().y == other->getRect().y){ ySpeed = 0; }
            }
        }
    }

    void onCollisionWith(SpritePtr other) override {
        if(dynamic_pointer_cast<Boll>(other)){
            xSpeed = 0;
            ySpeed = 0;
            xCollision = true;
            yCollision = true;
        }
        
        if (dynamic_pointer_cast<Wall>(other)){            
            move(-xSpeed, -ySpeed);

            move(xSpeed, 0);
            if(this->collidedWith(other)){
                move(-xSpeed, 0);
                xSpeed = 0;
                xCollision = true;
            }

            move(0, ySpeed);
            if(this->collidedWith(other)){
                move(0, -ySpeed);
                ySpeed = 0;
                yCollision = true;
            }
        }
    }
private:
    //maybe should already be declared in a moveable sprite class?
    int xSpeed = 1;
    int ySpeed = 1;
    bool xCollision, yCollision;
};

class PointCounter : public Label {
    public:
        PointCounter(int x, int y) : Label(x,y,"Points: 0"){}
        void tick() override {}
        void addPoints(){
            setText("Points: " + std::to_string(++points));
        }
    private:
        int points = 0;
};

class Bullet : public Sprite {
public:
    Bullet(int x, int y, int xs, int ys, shared_ptr<PointCounter> pc):Sprite("bullet.png", x,y), xSpeed(xs), ySpeed(ys), pointCounter(pc){}

    void tick() override {
        move(xSpeed, ySpeed);
        
        if (getRect().y < 0 - getRect().h || getRect().y > cnts::gScreenHeight || getRect().x < 0  - getRect().w || getRect().x > cnts::gScreenWidth){
           eng.remove(shared_from_this());
        }
    }

    void onCollisionWith(SpritePtr other) override {
        if(dynamic_pointer_cast<Enemy>(other)){
            if(!hit){
                hit = true;
                pointCounter->addPoints();
                eng.remove(other);
                eng.remove(shared_from_this());
            }
        }
        
        if (dynamic_pointer_cast<Wall>(other)){
            eng.remove(shared_from_this());
        }
    }

private:
    int xSpeed;
    int ySpeed;
    shared_ptr<PointCounter> pointCounter;
    bool hit = false;
};

//create random spawn coordinates for enemies
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> side(0,1);
uniform_int_distribution<int> smallX(0, 1);
uniform_int_distribution<int> smallY(0, 1);
uniform_int_distribution<int> largeX(0, cnts::gScreenWidth);
uniform_int_distribution<int> largeY(0, cnts::gScreenHeight);

class EnemySpawner : public Sprite {
public:
    void tick() override {
        spawnRate++;
        if(spawnRate >= 60){
            if(side(gen)){
                int x = largeX(gen);
                int y = smallY(gen);
                eng.add(SpritePtr(new Enemy(x,yPos[y])));
                spawnRate = 0;
            } else {
                int x = smallX(gen);
                int y = largeY(gen);
                eng.add(SpritePtr(new Enemy(xPos[x],y)));
                spawnRate = 0;
            }
        } //spawnrate if 
    }
private:
    int spawnRate = 30;
    int xPos[2] = {0, cnts::gScreenWidth};
    int yPos[2] = {0, cnts::gScreenHeight};
};

//probably move to cpp and h files
class Boll : public Sprite {
public:
    Boll(int x, LabelPtr label):Sprite("football.png", x,400), pointCounter(dynamic_pointer_cast<PointCounter>(label)) {}
    void tick() override {
        move(xSpeed, ySpeed);

        if (attackTimer < 35) {
            attackTimer++;
        }

        /*if (getRect().y < 0){
           eng.remove(shared_from_this());
        }*/
    }

    void onCollisionWith(SpritePtr other) override {
        if(dynamic_pointer_cast<Enemy>(other)){
            eng.remove(shared_from_this());
        }
        
        if (dynamic_pointer_cast<Wall>(other)){
            move(-xSpeed, -ySpeed);

            move(xSpeed, 0);
            if(this->collidedWith(other)){
                move(-xSpeed, 0);
                xSpeed = 0;
            }

            move(0, ySpeed);
            if(this->collidedWith(other)){
                move(0, -ySpeed);
                ySpeed = 0;
            }
                      
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

        /*
        Attacks
        Move to its own class so that movement/attack inputs don't disturb eachother?
        Not very noticeably on high (slow) attacktimers
        */
        if (attackTimer == 35) {
            if(key_states[SDL_SCANCODE_UP]){
                SpritePtr spr = SpritePtr(new Bullet(getRect().x, getRect().y, 0, -10,pointCounter));
                eng.add(spr);

                attackTimer = 0;
            } else if(key_states[SDL_SCANCODE_RIGHT]){
                SpritePtr spr = SpritePtr(new Bullet(getRect().x, getRect().y, 10, 0,pointCounter));
                eng.add(spr);
                
                attackTimer = 0;
            } else if(key_states[SDL_SCANCODE_DOWN]){
                SpritePtr spr = SpritePtr(new Bullet(getRect().x, getRect().y, 0, 10,pointCounter));
                eng.add(spr);
                
                attackTimer = 0;
            } else if(key_states[SDL_SCANCODE_LEFT]){
                SpritePtr spr = SpritePtr(new Bullet(getRect().x, getRect().y, -10, 0,pointCounter));
                eng.add(spr);
                
                attackTimer = 0;
            }
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
    int attackTimer = 35;
    shared_ptr<PointCounter> pointCounter;
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
    LabelPtr label = LabelPtr(new PointCounter(10,10));
    SpritePtr ball = SpritePtr(new Boll(300, label));
    //SpritePtr goal = SpritePtr(new Goal);

    eng.add(label);
    eng.add(ball);
    eng.add(SpritePtr(new Wall()));
    eng.add(SpritePtr(new EnemySpawner()));
    //eng.add(goal);
    eng.run();
}
