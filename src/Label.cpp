#include "Engine.h"
#include "Label.h"

namespace demo{

    Label::Label(int x, int y, int w, int h, std::string newText) : rect(x,y,w,h), text(newText){}

    Label::~Label(){
        SDL_DestroyTexture(image);
    }

    void Label::draw() const{
        SDL_RenderTexture(eng.getRen(), image, NULL, &rect);
    }
    
    void Label::setText(std::string newText){
        Label::text = newText;
        SDL_DestroyTexture(image);
        SDL_Surface* surface = TTF_RenderText_Solid(eng.getFont(),text.c_str(),0,{255,0,0});
        image = SDL_CreateTextureFromSurface(eng.getRen(), surface);
        SDL_DestroySurface(surface);
    }

    std::string Label::getText() const { return text; }
}