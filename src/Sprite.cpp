#include <SDL3_image/SDL_image.h>
#include <iostream>

#include "Sprite.h"
#include "Engine.h"

//spelmotorklass
namespace grupp24{
    Sprite::Sprite(std::string name,float x,float y){
        image = IMG_LoadTexture(engine.getRenderer(), (name).c_str());
        if (!image){
            std::cerr << "No such file: " << name << std::endl;
            exit(EXIT_FAILURE);
        }
        rectangle = {x,y,static_cast<float>(image->w), static_cast<float>(image->h)};
    }

    void Sprite::changeImage(std::string name) {
        if (image) {
            SDL_DestroyTexture(image);
            image = nullptr;
        }
        
        image = IMG_LoadTexture(engine.getRenderer(), (name).c_str());
        if(!image){
            std::cerr << "No such file: " << name << std::endl;
            exit(EXIT_FAILURE);
        }
        
        rectangle = {getRectangle().x, getRectangle().y,static_cast<float>(image->w), static_cast<float>(image->h)};
    }

    Sprite::~Sprite(){
        if(image){
            SDL_DestroyTexture(image);
        }
    }

    void Sprite::draw() const{
        SDL_RenderTexture(engine.getRenderer(), image, NULL, &getRectangle());
    }

    void Sprite::move(int dx, int dy) {
        rectangle.x += dx;
        rectangle.y += dy; 
    }

    void Sprite::setRectangle(int x, int y, int w, int h) {
        rectangle.x = x;
        rectangle.y = y;
        rectangle.w = w;
        rectangle.h = h;
    }

    const bool Sprite::collidedWith(SpritePtr other) const{
        return SDL_HasRectIntersectionFloat(&rectangle, &other->rectangle);
    }

    void Sprite::setImage(SDL_Texture* newImage){
        image = newImage;
    }
}