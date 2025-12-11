#pragma once
#include <memory>
#include <SDL3/SDL.h>
namespace demo{
    class Sprite;
    typedef std::shared_ptr<Sprite> SpritePtr;

    class Sprite : public std::enable_shared_from_this<Sprite>{
        public:
            virtual ~Sprite();
            void draw() const; // Inte virtual
            const SDL_FRect& getRect() const { return rect; }
            virtual void onMouseDown(const SDL_Event& event){}
            // Fler händelsefunktioner utelämnade för att minska kodmängden i exemplet
            virtual void tick() = 0;
            void move(int dx, int dy) { rect.x += dx; rect.y += dy; }
            bool collidedWith(SpritePtr other) const;
            virtual void onCollisionWith(SpritePtr other) {}
            virtual void onKeyDown(const SDL_Event& event) {}

            Sprite(const Sprite& other) = delete;
            const Sprite& operator=(const Sprite& other) = delete;

        protected:
            Sprite() = default; // Default-konstruktor
            Sprite(std::string name, float x,float y);
        private:
            SDL_FRect rect;
            SDL_Texture* image; // Alla Sprites har en image
    };
}  