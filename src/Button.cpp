#include "Button.h"
#include "Engine.h"
namespace demo{
    Button::Button(int x, int y, int w, int h, std::string txt) : Label(x, y, w, h, txt) {
        draw();
    }

    Button::~Button() {}

    void Button::setDown(bool state) {
        down = state;
    }

    void Button::draw() const{
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(eng.getRen(), &r,&g,&b,&a);
        
        if (down) {
            SDL_SetRenderDrawColor(eng.getRen(),255,0,0,255);
        } else {
            SDL_SetRenderDrawColor(eng.getRen(),255,192,203,255); // rosa
        
            SDL_RenderFillRect(eng.getRen(), &getRect());
        }
            
        SDL_SetRenderDrawColor(eng.getRen(), r,g,b,a);
        Label::draw();
    }
}