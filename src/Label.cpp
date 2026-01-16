#include "Engine.h"
#include "Label.h"

namespace grupp24{
    Label::Label(int x, int y, int w, int h, std::string newText) : text(newText){
        setRectangle(x, y, w, h);
        setText(text);
    }

    Label::~Label(){
        if(getImage()){
            SDL_DestroyTexture(getImage());
        }
    }
    
    void Label::setText(std::string newText){
        text = newText;
        if(getImage()){
            SDL_DestroyTexture(getImage());
        }
        SDL_Surface* surface = TTF_RenderText_Solid(engine.getFont(),text.c_str(),text.length(),{255,0,0, 255});
        if(!surface){
            SDL_Log("TTF_RenderText_Solid failed:");
            return;
        }
        setImage(SDL_CreateTextureFromSurface(engine.getRenderer(), surface));
        getMutableRectangle().w = surface->w;
        getMutableRectangle().h = surface->h;
        SDL_DestroySurface(surface);
    }

    const std::string& Label::getText() const { return text; }
}