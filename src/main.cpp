#include <memory>
#include <cstdlib>
#include <random>
#include <iostream>

#include "Engine.h"
#include "Sprite.h"
#include "Label.h"
#include "Button.h"
#include "Sound.h"

using namespace grupp24;
using namespace constants;

class Player;
class Bullet;

//spelklass
class Wall : public Sprite {
public:
    Wall(int x, int y) : Sprite(wall_full_hp, x, y){}
    void tick() override {
        if(collided){
            if (health > 0) {
                engine.playSFX(wall_hit_sfx);
            }
            health--;
            collided = false;
        }
        switch(health){
            case 1:{
                changeImage(wall_1_hp);
                break;
            }
            case 2:{
                changeImage(wall_2_hp);
                break;
            }
            case 3:{
                changeImage(wall_3_hp);
                break;
            }
            case 4:{
                changeImage(wall_4_hp);
                break;
            }
        }
        if(health <= 0){
            engine.playSFX(wall_break_sfx);
            engine.remove(shared_from_this());
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

//spelklass
class Enemy : public Sprite {
public:
    Enemy(int x, int y) : Sprite(enemy, x, y){}
    void tick() override {
        move(xSpeed, ySpeed);
        xCollision = false;
        yCollision = false;
        if (getRectangle().y < 0 - getRectangle().h || getRectangle().y > gScreenHeight || getRectangle().x < 0  - getRectangle().w || getRectangle().x > gScreenWidth){
           engine.remove(shared_from_this());
        }
    }
    
    void interactWith(SpritePtr other) override {
        if(dynamic_pointer_cast<Player>(other)){
            if(!xCollision){
                if(getRectangle().x < other->getRectangle().x){ xSpeed = 1; }
                if(getRectangle().x > other->getRectangle().x){ xSpeed = -1; }
                if(getRectangle().x == other->getRectangle().x){ xSpeed = 0; }
            }

            if(!yCollision){
                if(getRectangle().y < other->getRectangle().y){ ySpeed = 1; }
                if(getRectangle().y > other->getRectangle().y){ ySpeed = -1; }
                if(getRectangle().y == other->getRectangle().y){ ySpeed = 0; }
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
    int xSpeed = 1;
    int ySpeed = 1;
    bool xCollision, yCollision;
};

//spelklass
class WinText : public Label {
public:
    WinText(int x, int y, int w, int h) : Label (x, y, w, h, "You win! Press R to play again!"){};

    void tick() override {};
};

//spelklass
class LoseText : public Label {
public:
    LoseText(int x, int y, int w, int h) : Label (x, y, w, h, "You lose! Press R to play again!"){};

    void tick() override {};

    const bool getClearState() const override {
        return true;
    }
};

//spelklass
class PointCounter : public Label {
public:
    PointCounter(int x, int y, int w, int h) : Label(x,y,w,h,"Points: 0"){}
    void tick() override {
        if (points >= 10 && !cleared) {
            engine.playSFX(player_victory_sfx);

            engine.add(LabelPtr(new WinText(300, 350, 0, 0)));
            cleared = true;
        }
    }

    void addPoints(){
        setText("Points: " + std::to_string(++points));
    }

    void setClearState(bool state){
        cleared = state;
    }

    const bool getClearState() const override {
        return cleared;
    }
private:
    int points = 0;
    bool cleared = false;
};

//spelklass
class Bullet : public Sprite {
public:
    Bullet(int x, int y, int xs, int ys, std::shared_ptr<PointCounter> pc):Sprite(bullet, x,y), xSpeed(xs), ySpeed(ys), pointCounter(pc){}

    void tick() override {
        move(xSpeed, ySpeed);
        
        if (getRectangle().y < 0 - getRectangle().h || getRectangle().y > gScreenHeight || getRectangle().x < 0  - getRectangle().w || getRectangle().x > gScreenWidth){
           engine.remove(shared_from_this());
        }
    }

    void onCollisionWith(SpritePtr other) override {
        if(dynamic_pointer_cast<Enemy>(other)){
            if(!hit){
                engine.playSFX(enemy_death_sfx);
                hit = true;
                pointCounter->addPoints();
                engine.remove(other);
                engine.remove(shared_from_this());
            }
        }
        
        if (dynamic_pointer_cast<Wall>(other)){
            engine.remove(shared_from_this());
        }
    }

private:
    int xSpeed;
    int ySpeed;
    std::shared_ptr<PointCounter> pointCounter;
    bool hit = false;
};

//spelklass
class EnemySpawner : public Sprite {
public:
    EnemySpawner(LabelPtr pc) : pointCounter(dynamic_pointer_cast<PointCounter>(pc)),
    generator(randomDevice()), side(0,1), smallX(0,1), smallY(0,1){}
    void tick() override {
        spawnRate++;
        if(!pointCounter->getClearState()){
            if(spawnRate >= 60){
                if(side(generator)){
                    int x = gScreenWidth/2;
                    int y = smallY(generator);
                    engine.add(SpritePtr(new Enemy(x,yPosition[y])));
                    spawnRate = 0;
                } else {
                    int x = smallX(generator);
                    int y = gScreenHeight/2 - 69;
                    engine.add(SpritePtr(new Enemy(xPosition[x],y)));
                    spawnRate = 0;
                }
            } //spawnrate if 
        }
    }
private:
    std::shared_ptr<PointCounter> pointCounter;
    int spawnRate = 30;
    int xPosition[2] = {0, gScreenWidth - 69};
    int yPosition[2] = {0, gScreenHeight - 69};

    std::random_device randomDevice;
    std::mt19937 generator;
    std::uniform_int_distribution<int> side;
    std::uniform_int_distribution<int> smallX;
    std::uniform_int_distribution<int> smallY;
};

//spelklass
class Player : public Sprite {
public:
    Player(int x, int y, LabelPtr pc):Sprite(player, x, y), pointCounter(dynamic_pointer_cast<PointCounter>(pc)) {}
    void tick() override {
        move(xSpeed, ySpeed);

        if (attackTimer < 35) {
            attackTimer++;
        }

        if (pointCounter->getClearState()) {
            engine.remove(shared_from_this());
        }
    }

    void onCollisionWith(SpritePtr other) override {
        if(dynamic_pointer_cast<Enemy>(other)){
            engine.playSFX(player_death_sfx);
            engine.add(LabelPtr(new LoseText(300, 350, 0, 0)));
            pointCounter->setClearState(true);
            engine.remove(shared_from_this());
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
                      
        }
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
                engine.playSFX(attack_sfx);
                engine.add(SpritePtr(new Bullet(getRectangle().x, getRectangle().y, 0, -10,pointCounter)));

                attackTimer = 0;
            } else if(key_states[SDL_SCANCODE_RIGHT]){
                engine.playSFX(attack_sfx);
                engine.add(SpritePtr(new Bullet(getRectangle().x, getRectangle().y, 10, 0,pointCounter)));
                
                attackTimer = 0;
            } else if(key_states[SDL_SCANCODE_DOWN]){
                engine.playSFX(attack_sfx);
                engine.add(SpritePtr(new Bullet(getRectangle().x, getRectangle().y, 0, 10,pointCounter)));
                
                attackTimer = 0;
            } else if(key_states[SDL_SCANCODE_LEFT]){
                engine.playSFX(attack_sfx);
                engine.add(SpritePtr(new Bullet(getRectangle().x, getRectangle().y, -10, 0,pointCounter))); 
                
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
    std::shared_ptr<PointCounter> pointCounter;
};

//spelklass
class ExitButton : public Button {
public:
    ExitButton(int x, int y, int w, int h, std::string text): Button(x, y, w, h, text) {}

    void tick() override {}

    void onMouseDown(const SDL_Event& event){
        SDL_FPoint point = {event.button.x, event.button.y};
        if (SDL_PointInRectFloat(&point, &getRectangle())){
            setPressedState(true);
        }
    }

    void onMouseUp(const SDL_Event& event) override {
        SDL_FPoint point = {event.button.x, event.button.y};
        if (getPressedState() && SDL_PointInRectFloat(&point, &getRectangle())) {
            engine.stop();
        }

        setPressedState(false);
    }
};

//spelklass
class StartButton : public Button {
public:
    StartButton(int x, int y, int w, int h, std::string text, ButtonPtr exit): Button(x, y, w, h, text), exitButton(exit) {}

    void tick() override {}

    void onMouseDown(const SDL_Event& event){
        SDL_FPoint point = {event.button.x, event.button.y};
        if (SDL_PointInRectFloat(&point, &getRectangle())){
            setPressedState(true);
        }
    }

    void onMouseUp(const SDL_Event& event) override {
        SDL_FPoint point = {event.button.x, event.button.y};
        if (getPressedState() && SDL_PointInRectFloat(&point, &getRectangle())) {
            LabelPtr label = LabelPtr(new PointCounter(100,100,0,0));
            SpritePtr player = SpritePtr(new Player(gScreenWidth/2, gScreenHeight/2, label));

            engine.add(label);
            engine.add(player);
            engine.add(SpritePtr(new Wall(300,200)));
            engine.add(SpritePtr(new Wall(300,450)));
            engine.add(SpritePtr(new Wall(700,200)));
            engine.add(SpritePtr(new Wall(700,450)));
            for(int i = 0; i < 16; i++){
                if(i != 7 && i != 8){
                    engine.add(SpritePtr(new Wall(i*69, 0)));
                    engine.add(SpritePtr(new Wall(i*69, gScreenHeight - 69)));
                }
            }
            for(int i = 1; i < 10; i++){
                if(i != 4 && i != 5){
                    engine.add(SpritePtr(new Wall(0, i*69)));
                    engine.add(SpritePtr(new Wall(gScreenWidth - 69, i*69)));
                }
            }
            engine.add(SpritePtr(new EnemySpawner(label)));
            engine.playSFX(start_game_sfx);
            engine.remove(exitButton);
            engine.remove(shared_from_this());
        }

        setPressedState(false);
    }
private:
    ButtonPtr exitButton;
};

//spelklass
class Background : public Sprite {
public:
    Background(int x, int y) : Sprite(grass_bg, x, y) {}

    void tick() override {}
};

//game function where gameprogrammers write all of their starting code for the game
//although class itself is more leaned into engine and the structure should not be changed
class Game{
public:
    void start(){
        SpritePtr background = SpritePtr(new Background(0, 0));
        ButtonPtr exitButton = ButtonPtr(new ExitButton(490, 350, 10, 10, "Exit"));
        ButtonPtr startButton = ButtonPtr(new StartButton(480, 250, 10, 10, "Start", exitButton));

        engine.add(background);
        engine.add(exitButton);
        engine.add(startButton);
    }
};

//this is more closely attahced to the game engine and not the gameprogrammer
//aka main function is a engine class and should not be touched by the gameprogrammer
int main(){
    Game game;    
    engine.setStartCallBack([&]() {game.start();});
    engine.run();
}
