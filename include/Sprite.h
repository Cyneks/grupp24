#pragma once
#include <memory>
#include <SDL3/SDL.h>

namespace demo{
    class Sprite;
    typedef std::shared_ptr<Sprite> SpritePtr;

    class Sprite : public std::enable_shared_from_this<Sprite>{
        public:
            virtual ~Sprite();
            virtual void draw() const;
            const SDL_FRect& getRect() const { return rect; }
            // move to a moveable sprite class?
            virtual void tick() = 0;
            virtual void changeImage(std::string);

            //move to a moveable sprite class?
            void move(int dx, int dy);
            bool collidedWith(SpritePtr other) const;

            //move to a moveable sprite class?
            virtual void interactWith(SpritePtr other) {}
            virtual void onCollisionWith(SpritePtr other) {}
            virtual void onKeyDown(const SDL_Event& event) {}
            virtual void onKeyUp() {}
            virtual void onMouseDown(const SDL_Event& event){}
            virtual void onMouseUp(const SDL_Event& event) {};

            Sprite(const Sprite& other) = delete;
            const Sprite& operator=(const Sprite& other) = delete;

        protected:
            Sprite() = default; // Default-konstruktor
            Sprite(std::string name, float x,float y);
            SDL_FRect& getMutableRect() { return rect; }
            void setRect(int x, int y, int w, int h);
        private:
            SDL_FRect rect;
            SDL_Texture* image; // Alla Sprites har en image
    };
}  