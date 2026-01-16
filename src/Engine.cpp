#include <iostream>

#include "Engine.h"
#include "Sprite.h"
#include "Constants.h"

//spelmotorklass
namespace grupp24{

    Engine::Engine() : window(nullptr), renderer(nullptr), font(nullptr), running(true){
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == 0) {
            throw std::runtime_error("SDL_Init failed");
        }

        try {
            window = SDL_CreateWindow("Ett spel", cnts::gScreenWidth, cnts::gScreenHeight,0);
            if(!window) {
                throw std::runtime_error("Window creation failed");
            }

            renderer = SDL_CreateRenderer(window, NULL);
            if(!renderer){
                throw std::runtime_error("Renderer creation failed");
            }

            if(!TTF_Init()){
                throw std::runtime_error("TTF_Init failed");
            }

            font = TTF_OpenFont((cnts::gResPath + "fonts/INKFREE.TTF").c_str(), 36);
            if(!font){
                throw std::runtime_error("Font load failed");
            }
        } catch (const std::runtime_error&) {
            if (font) {TTF_CloseFont(font);}
            TTF_Quit();
            if (renderer) {SDL_DestroyRenderer(renderer);}
            if (window) {SDL_DestroyWindow(window);}
            SDL_Quit();
            throw;
        }
    }

    Engine::~Engine(){
        added.clear();
        removed.clear();
        sprites.clear();
        if (font) {TTF_CloseFont(font);}
        TTF_Quit();
        if (renderer) {SDL_DestroyRenderer(renderer);}
        if (window) {SDL_DestroyWindow(window);}
        SDL_Quit();
    }

    void Engine::add(SpritePtr sprite){
        added.push_back(sprite);
    }

    void Engine::remove(SpritePtr sprite){
        removed.push_back(sprite);
    }

    void Engine::clearSprites() {
        sprites.clear();
    }

    void Engine::playSFX(const std::string& name) {
        sound.play(name);
    }

    void Engine::stop() {
        running = false;
    }

    void Engine::setStartCallBack(std::function<void()> callBack){
        startCallBack = std::move(callBack);
    }

    void Engine::run(){
        const int FPS = 60;
        const int TICKINTERVAL = 1000 / FPS;

        if(startCallBack){
            startCallBack();
        }

        while (running){
            Uint64 nextTick = SDL_GetTicks() + TICKINTERVAL; 
            SDL_Event event;
            while (SDL_PollEvent(&event)){
                switch (event.type){
                    case SDL_EVENT_QUIT:{
                        running = false; 
                        break;
                    }
                    case SDL_EVENT_MOUSE_BUTTON_DOWN:{
                        for(SpritePtr sprite : sprites){
                            sprite->onMouseDown(event);
                        } 
                        break;
                    }
                    case SDL_EVENT_MOUSE_BUTTON_UP:{
                        for (SpritePtr sprite : sprites) {
                            sprite->onMouseUp(event);
                        }
                        break;
                    }
                    case SDL_EVENT_KEY_UP: {
                        for (SpritePtr sprite : sprites) {
                            sprite->onKeyUp();
                            if(sprite->getClearState()){
                                gameOver = sprite->getClearState();
                            }
                        }
                        if(event.key.key == SDLK_R && gameOver){
                            clearSprites();
                            if(startCallBack){
                                startCallBack();
                            }
                            gameOver = false;
                        }
                    }
                    case SDL_EVENT_KEY_DOWN:{
                        if(event.key.key == SDLK_ESCAPE){
                            running = false; 
                            break;
                        }
                        for (SpritePtr sprite : sprites) {
                            sprite->onKeyDown(event);
                        }
                        break;
                    }
                } 
            } 

            for(SpritePtr sprite : sprites){
                sprite->tick();
            }

            for(SpritePtr sprite : added){
                sprites.push_back(sprite);
            }
            added.clear();

            for(SpritePtr sprite : removed){
                for(unsigned int position = 0; position < sprites.size(); position++){
                    auto iterator = sprites.begin() + position;
                    if (sprite == *iterator) {
                        sprites.erase(iterator);
                        break;
                    }
                } 
            } 
            removed.clear();

            for(SpritePtr sprite : sprites){
                for(SpritePtr otherSprite : sprites){
                    if (sprite != otherSprite){
                        if(sprite->collidedWith(otherSprite)){
                            sprite->onCollisionWith(otherSprite);
                            otherSprite->onCollisionWith(sprite);
                        }
                        sprite->interactWith(otherSprite);
                    }
                }
            }

            SDL_SetRenderDrawColor(renderer,255,255,255,255);
            SDL_RenderClear(renderer);
            for(SpritePtr sprite : sprites){
                sprite->draw();
            }
            SDL_RenderPresent(renderer);

            Sint64 delay = nextTick - SDL_GetTicks();
            if (delay > 0){
                SDL_Delay(delay);
            } 
        } 
    }

    Engine engine;
    
}