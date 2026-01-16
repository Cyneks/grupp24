#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <memory>
#include <functional>

#include "Constants.h"
#include "Sound.h"

namespace cnts = constants;
namespace grupp24{
    class Sprite;
    typedef std::shared_ptr<Sprite> SpritePtr;
 
    class Engine{
        public:
            Engine();
            ~Engine();
            SDL_Renderer* getRenderer() const { return renderer; }
            SDL_Window* getWindow() const { return window; }
            TTF_Font* getFont() const { return font; }
            void playSFX(const std::string& name);
            void add(SpritePtr sprite);
            void remove(SpritePtr sprite);
            void clearSprites();
            void run();
            void stop();
            void setStartCallBack(std::function<void()> startCallBack);
        private:
            SDL_Window* window;
            SDL_Renderer* renderer;
            TTF_Font* font;
            Sound sound;
            std::vector<SpritePtr> sprites, added, removed;
            std::function<void()> startCallBack;
            bool gameOver = false;
            bool running = true;
    };
    extern Engine engine;
}