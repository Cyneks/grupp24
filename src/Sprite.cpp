#include "Sprite.h"
#include "Engine.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>
using namespace std;
namespace demo{
    Sprite::Sprite(string name,float x,float y){
        //only allow for png pictures? Can add ".png" then.
        image = IMG_LoadTexture(eng.getRen(), (cnts::gResPath + "/images/" + name).c_str());
        if (!image){
            cerr << "No such file: " << name << endl;
            exit(EXIT_FAILURE);
        }
        rect = {x,y,static_cast<float>(image->w),static_cast<float>(image->h)};
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

    bool Sprite::collidedWith(SpritePtr other) const{
        return SDL_HasRectIntersectionFloat(&rect, &other->rect);
    }

 }