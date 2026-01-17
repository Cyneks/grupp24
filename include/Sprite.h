#pragma once
#include <memory>
#include <SDL3/SDL.h>

namespace grupp24{
    class Sprite;
    typedef std::shared_ptr<Sprite> SpritePtr;

    class Sprite : public std::enable_shared_from_this<Sprite>{
        public:
            virtual ~Sprite();
            
            const SDL_FRect& getRectangle() const { return rectangle; }
            void move(int dx, int dy);
            const bool collidedWith(const SpritePtr other) const;

            virtual void tick() = 0;
            virtual const bool getClearState() const { return false; }
            virtual void draw() const;
            virtual void changeImage(std::string);
            virtual void interactWith(SpritePtr other) {}
            virtual void onCollisionWith(SpritePtr other) {}
            virtual void onKeyDown(const SDL_Event& event) {}
            virtual void onKeyUp() {}
            virtual void onMouseDown(const SDL_Event& event){}
            virtual void onMouseUp(const SDL_Event& event) {}

            Sprite(const Sprite& other) = delete;
            const Sprite& operator=(const Sprite& other) = delete;

        protected:
            Sprite() = default;
            Sprite(std::string name, float x,float y);
            SDL_FRect& getMutableRectangle() { return rectangle; }
            void setRectangle(int x, int y, int w, int h);
            SDL_Texture* getImage() {return image;}
            void setImage(SDL_Texture* newImage);
        private:
            SDL_FRect rectangle;
            SDL_Texture* image;
    };
}  