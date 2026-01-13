#include "Engine.h"
#include "Label.h"

namespace demo{
    Label::Label(int x, int y, int w, int h, std::string newText) : text(newText), image(nullptr){
        setRect(x, y, w, h);
        setText(text);
    }

    Label::~Label(){
        if(image){
            SDL_DestroyTexture(image);
        }
    }

    void Label::draw() const {
        SDL_RenderTexture(eng.getRen(), image, NULL, &getRect());
    }
    
    void Label::setText(std::string newText){
        Label::text = newText;
        if(image){
            SDL_DestroyTexture(image);
        }
        SDL_Surface* surface = TTF_RenderText_Solid(eng.getFont(),text.c_str(),text.length(),{255,0,0, 255});
        if(!surface){
            SDL_Log("TTF_RenderText_Solid failed:");
            return;
        }
        image = SDL_CreateTextureFromSurface(eng.getRen(), surface);
        getMutableRect().w = surface->w;
        getMutableRect().h = surface->h;
        SDL_DestroySurface(surface);
    }

    std::string Label::getText() const { return text; }
}