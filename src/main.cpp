#include "Engine.h"
#include <memory>
#include "Sprite.h"
#include "Label.h"
#include "Button.h"
#include "Sound.h"
#include <cstdlib>
#include <random>
#include <iostream>
using namespace demo;
using namespace std;

class Player;
class Bullet;

//probably move to cpp and h files
class Wall : public Sprite {
public:
    Wall(int x, int y) : Sprite(cnts::red_wall_1, x, y){}
    void tick() override {
        if(collided){
            if (health > 0) {
                eng.playSFX(cnts::wall_hit);
            }

            health--;
            collided = false;
        }
        switch(health){
            case 1:{
                changeImage(cnts::red_wall_5);
                break;
            }
            case 2:{
                changeImage(cnts::red_wall_4);
                break;
            }
            case 3:{
                changeImage(cnts::red_wall_3);
                break;
            }
            case 4:{
                changeImage(cnts::red_wall_2);
                break;
            }
        }
        if(health <= 0){
            eng.playSFX(cnts::wall_break);
            eng.remove(shared_from_this());
        }
    }
    void onCollisionWith(SpritePtr other) override {
        if(dynamic_pointer_cast<Bullet>(other)){
            collided = true;
        }
    }
private:
    bool collided = false;
    int health = 5;
};

//probably move to cpp and h files
class Enemy : public Sprite {
public:
    Enemy(int x, int y) : Sprite(cnts::green_enemy, x, y){}
    void tick() override {
        move(xSpeed, ySpeed);
        xCollision = false;
        yCollision = false;
    }
    
    void interactWith(SpritePtr other) override {
        if(dynamic_pointer_cast<Player>(other)){
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
        if(dynamic_pointer_cast<Player>(other)){
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

class WinText : public Label {
    public:
        WinText(int x, int y, int w, int h) : Label (x, y, w, h, "You win! Press R to play again!"){};

        void tick() override {};
};

class LoseText : public Label {
    public:
        LoseText(int x, int y, int w, int h) : Label (x, y, w, h, "You lose! Press R to play again!"){};

        void tick() override {};
};

class PointCounter : public Label {
    public:
        PointCounter(int x, int y, int w, int h) : Label(x,y,w,h,"Points: 0"){}
        void tick() override {
            if (points >= 10 && !cleared) {
                eng.playSFX(cnts::win);

                //Orka hitta mitten av skärmen lol
                eng.add(LabelPtr(new WinText(400, 350, 0, 0)));
                cleared = true;
            }
        }

        void addPoints(){
            setText("Points: " + std::to_string(++points));
        }

        const bool getClearState() override {
            return cleared;
        }
    private:
        int points = 0;
        bool cleared = false;
};

class Bullet : public Sprite {
public:
    Bullet(int x, int y, int xs, int ys, shared_ptr<PointCounter> pc):Sprite(cnts::bullet, x,y), xSpeed(xs), ySpeed(ys), pointCounter(pc){}

    void tick() override {
        move(xSpeed, ySpeed);
        
        if (getRect().y < 0 - getRect().h || getRect().y > cnts::gScreenHeight || getRect().x < 0  - getRect().w || getRect().x > cnts::gScreenWidth){
           eng.remove(shared_from_this());
        }
    }

    void onCollisionWith(SpritePtr other) override {
        if(dynamic_pointer_cast<Enemy>(other)){
            if(!hit){
                eng.playSFX(cnts::monster_death);
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
//uniform_int_distribution<int> largeX(0, cnts::gScreenWidth);
//uniform_int_distribution<int> largeY(0, cnts::gScreenHeight);

class EnemySpawner : public Sprite {
public:
    void tick() override {
        spawnRate++;
        if(spawnRate >= 60){
            if(side(gen)){
                int x = cnts::gScreenWidth/2;
                int y = smallY(gen);
                eng.add(SpritePtr(new Enemy(x,yPos[y])));
                spawnRate = 0;
            } else {
                int x = smallX(gen);
                int y = cnts::gScreenHeight/2 - 69;
                eng.add(SpritePtr(new Enemy(xPos[x],y)));
                spawnRate = 0;
            }
        } //spawnrate if 
    }
private:
    int spawnRate = 30;
    int xPos[2] = {0, cnts::gScreenWidth - 69};
    int yPos[2] = {0, cnts::gScreenHeight - 69};
};

//probably move to cpp and h files
class Player : public Sprite {
public:
    Player(int x, int y, LabelPtr label):Sprite(cnts::player, x, y), pointCounter(dynamic_pointer_cast<PointCounter>(label)) {}
    void tick() override {
        move(xSpeed, ySpeed);

        if (attackTimer < 35) {
            attackTimer++;
        }

        if (pointCounter->getClearState()) {
            eng.remove(shared_from_this());
        }
    }

    void onCollisionWith(SpritePtr other) override {
        if(dynamic_pointer_cast<Enemy>(other)){
            eng.playSFX(cnts::player_death);
            eng.add(LabelPtr(new LoseText(400, 350, 0, 0)));
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

        if (attackTimer == 35) {
            if(key_states[SDL_SCANCODE_UP]){
                eng.playSFX(cnts::gunshot);
                SpritePtr spr = SpritePtr(new Bullet(getRect().x, getRect().y, 0, -10,pointCounter));
                eng.add(spr);

                attackTimer = 0;
            } else if(key_states[SDL_SCANCODE_RIGHT]){
                eng.playSFX(cnts::gunshot);
                SpritePtr spr = SpritePtr(new Bullet(getRect().x, getRect().y, 10, 0,pointCounter));
                eng.add(spr);
                
                attackTimer = 0;
            } else if(key_states[SDL_SCANCODE_DOWN]){
                eng.playSFX(cnts::gunshot);
                SpritePtr spr = SpritePtr(new Bullet(getRect().x, getRect().y, 0, 10,pointCounter));
                eng.add(spr);
                
                attackTimer = 0;
            } else if(key_states[SDL_SCANCODE_LEFT]){
                eng.playSFX(cnts::gunshot);
                SpritePtr spr = SpritePtr(new Bullet(getRect().x, getRect().y, -10, 0,pointCounter));
                eng.add(spr);
                
                attackTimer = 0;
            }
        } // onKeyDown   
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

class ExitButton : public Button {
    public:
        ExitButton(int x, int y, int w, int h, std::string txt): Button(x, y, w, h, txt) {}

        void tick() override {}

        void onMouseDown(const SDL_Event& event){
            SDL_FPoint point = {event.button.x, event.button.y};
            if (SDL_PointInRectFloat(&point, &getRect())){
                setDown(true);
            }
        }

        void onMouseUp(const SDL_Event& event) override {
            SDL_FPoint point = {event.button.x, event.button.y};
            if (getDownState() && SDL_PointInRectFloat(&point, &getRect())) {
                eng.stop();
            }

            setDown(false);
        }
};

class StartButton : public Button {
    public:
        StartButton(int x, int y, int w, int h, std::string txt, ButtonPtr exit): Button(x, y, w, h, txt), exitButton(exit) {}

        void tick() override {}

        void onMouseDown(const SDL_Event& event){
            SDL_FPoint point = {event.button.x, event.button.y};
            if (SDL_PointInRectFloat(&point, &getRect())){
                setDown(true);
            }
        }

        void onMouseUp(const SDL_Event& event) override {
            SDL_FPoint point = {event.button.x, event.button.y};
            if (getDownState() && SDL_PointInRectFloat(&point, &getRect())) {
                LabelPtr label = LabelPtr(new PointCounter(100,100,0,0));
                SpritePtr player = SpritePtr(new Player(cnts::gScreenWidth/2, cnts::gScreenHeight/2, label));

                eng.add(label);
                eng.add(player);
                eng.add(SpritePtr(new Wall(300,200)));
                eng.add(SpritePtr(new Wall(300,450)));
                eng.add(SpritePtr(new Wall(700,200)));
                eng.add(SpritePtr(new Wall(700,450)));
                for(int i = 0; i < 16; i++){
                    if(i != 7 && i != 8){
                        eng.add(SpritePtr(new Wall(i*69, 0)));
                        eng.add(SpritePtr(new Wall(i*69, cnts::gScreenHeight - 69)));
                    }
                }
                for(int i = 1; i < 10; i++){
                    if(i != 4 && i != 5){
                        eng.add(SpritePtr(new Wall(0, i*69)));
                        eng.add(SpritePtr(new Wall(cnts::gScreenWidth - 69, i*69)));
                    }
                }
                eng.add(SpritePtr(new EnemySpawner()));
                eng.remove(exitButton);
                eng.remove(shared_from_this());
            }

            setDown(false);
        }
    private:
        ButtonPtr exitButton;
};

class Background : public Sprite {
    public:
        Background(int x, int y) : Sprite(cnts::grass, x, y) {}

        void tick() override {}
};

//game function where gameprogrammers write all of their starting code for the game
//although class itself is more leaned into engine and the structure should not be changed
class Game{
public:
    void start(){
        SpritePtr background = SpritePtr(new Background(0, 0));
        ButtonPtr exitButton = ButtonPtr(new ExitButton(200, 100, 10, 10, "Exit"));
        ButtonPtr startButton = ButtonPtr(new StartButton(100, 100, 10, 10, "Start", exitButton));

        eng.add(background);
        eng.add(exitButton);
        eng.add(startButton);
    }
};

//this is more closely attahced to the game engine and not the gameprogrammer
//aka main function is a engine class and should not be touched by the gameprogrammer
int main(){
    Game g;    
    eng.setStartCallBack([&]() {g.start();});
    eng.run();
}
