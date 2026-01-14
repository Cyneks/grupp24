#include "Sprite.h"
#include "Engine.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>
using namespace std;

namespace demo{
    Sprite::Sprite(string name,float x,float y){
        //only allow for png pictures? Can add ".png" then.
        image = IMG_LoadTexture(eng.getRen(), (name).c_str());
        if (!image){
            cerr << "No such file: " << name << endl;
            exit(EXIT_FAILURE);
        }
        rect = {x,y,static_cast<float>(image->w),static_cast<float>(image->h)};
    }

    void Sprite::changeImage(std::string pic) {
        if (image) {
            SDL_DestroyTexture(image);
            image = nullptr;
        }
        
        image = IMG_LoadTexture(eng.getRen(), (pic).c_str());
        if(!image){
            cerr << "No such file: " << pic << endl;
            exit(EXIT_FAILURE);
        }
        
        rect = {getRect().x, getRect().y, static_cast<float>(image->w), static_cast<float>(image->h)};
    }

    Sprite::~Sprite(){
        SDL_DestroyTexture(image);
    }

    void Sprite::draw() const{
        SDL_RenderTexture(eng.getRen(), image, NULL, &rect);
    }

    void Sprite::move(int dx, int dy) {
        rect.x += dx; 
        rect.y += dy; 
    }

    void Sprite::setRect(int x, int y, int w, int h) {
        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;
    }

    bool Sprite::collidedWith(SpritePtr other) const{
        return SDL_HasRectIntersectionFloat(&rect, &other->rect);
    }
}