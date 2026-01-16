#include "Button.h"
#include "Engine.h"

namespace grupp24{
    Button::Button(int x, int y, int w, int h, std::string text) : Label(x, y, w, h, text) {
        draw();
    }

    Button::~Button() {}

    void Button::setPressedState(bool state) {
        pressedState = state;
    }

    void Button::draw() const{
        Uint8 red, green, blue, alpha;
        SDL_GetRenderDrawColor(engine.getRenderer(), &red,&green,&blue,&alpha);
        
        if (pressedState) {
            SDL_SetRenderDrawColor(engine.getRenderer(),255,0,0,255);
        } else {
            SDL_SetRenderDrawColor(engine.getRenderer(),255,192,203,255); // rosa
        
            SDL_RenderFillRect(engine.getRenderer(), &getRectangle());
        }
            
        SDL_SetRenderDrawColor(engine.getRenderer(), red,green,blue,alpha);
        Label::draw();
    }
}