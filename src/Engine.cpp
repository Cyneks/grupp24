#include "Engine.h"
#include "Sprite.h"
#include "Constants.h"
#include <iostream>
namespace demo{

    Engine::Engine(){
        win = SDL_CreateWindow("Ett spel", cnts::gScreenWidth, cnts::gScreenHeight,0);
        ren = SDL_CreateRenderer(win, NULL);
        TTF_Init();
        font = TTF_OpenFont((cnts::gResPath + "fonts/INKFREE.TTF").c_str(), 24);
        if(!font){SDL_Log("Font failed");}
        srand(time(NULL));
    }

    Engine::~Engine(){
        TTF_Quit();
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
    }

    void Engine::add(SpritePtr spr){
        added.push_back(spr);
    }

    void Engine::remove(SpritePtr spr){
        removed.push_back(spr);
    }

    void Engine::run(){
        const int FPS = 60; // Frames Per Second
        const int TICKINTERVAL = 1000 / FPS; // In miliseconds

        bool running = true;
        while (running){
            Uint64 nextTick = SDL_GetTicks() + TICKINTERVAL; 
            SDL_Event event;
            while (SDL_PollEvent(&event)){
                switch (event.type){
                    case SDL_EVENT_QUIT:
                        running = false; break;
                    case SDL_EVENT_MOUSE_BUTTON_DOWN:
                        for(SpritePtr spr : sprites)
                            spr->onMouseDown(event);
                        break;
                    case SDL_EVENT_KEY_UP: {
                        for (SpritePtr spr : sprites) {
                            spr->onKeyUp();
                        }
                    }
                    case SDL_EVENT_KEY_DOWN:
                        if(event.key.key == SDLK_ESCAPE){
                            running = false; 
                            break;
                        }
                        for (SpritePtr spr : sprites) {
                            spr->onKeyDown(event);
                        }
                        break;
                // Fler händelser utelämnade för minska kodmängden i exemplet
                } // switch
            } // while event

            for(SpritePtr spr : sprites)
                spr->tick();

            for(SpritePtr spr : added)
                sprites.push_back(spr);
            added.clear();

            for(SpritePtr spr : removed){
                for(unsigned int pos = 0; pos < sprites.size(); pos++){
                    auto iter = sprites.begin() + pos;
                    if (spr == *iter) {
                        sprites.erase(iter);
                        break;
                    } // if
                } // for pos
            } // for spr
            removed.clear();

            for(SpritePtr sp1 : sprites){
                for(SpritePtr sp2 : sprites){
                    if (sp1 != sp2){
                        if(sp1->collidedWith(sp2)){
                            sp1->onCollisionWith(sp2);
                            sp2->onCollisionWith(sp1);
                        }
                        sp1->interactWith(sp2);
                    }
                }
            }

           SDL_SetRenderDrawColor(ren,255,255,255,255);
            SDL_RenderClear(ren);
            for(SpritePtr spr : sprites)
                spr->draw();
            SDL_RenderPresent(ren);

            Sint64 delay = nextTick - SDL_GetTicks();
            if (delay > 0)
                SDL_Delay(delay); 
        } // while running
    } // run

    Engine eng;
    
} // namspace gui