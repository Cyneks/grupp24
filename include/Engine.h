#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <memory>
#include "Constants.h"
#include "Sound.h"
namespace cnts = constants;
namespace demo{
    class Sprite;
    typedef std::shared_ptr<Sprite> SpritePtr;
 
    class Engine{
        public:
            Engine();
            ~Engine();
            SDL_Renderer* getRen() const { return ren; }
            SDL_Window* getWin() const { return win; }
            TTF_Font* getFont() const { return font; }
            void playSFX(const std::string& name);
            void add(SpritePtr spr);
            void remove(SpritePtr spr);
            void clearSprites();
            void run();
            void stop();
        private:
            SDL_Window* win;
            SDL_Renderer* ren;
            TTF_Font* font;
            Sound sound;
            std::vector<SpritePtr> sprites, added, removed;
            bool running = true;

    };
    extern Engine eng;
}