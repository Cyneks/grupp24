#pragma once
#include "Sprite.h"

namespace demo{
    class Label;
    typedef std::shared_ptr<Label> LabelPtr;

    class Label : public Sprite {
        public:
            virtual ~Label();
            void draw() const override;
            void setText(std::string);
            std::string getText() const;
        protected:
            Label(int x, int y, int w, int h, std::string text);
            SDL_FRect& getMutableRect() { return rect; }
        private:
            SDL_FRect rect;
            std::string text;
            SDL_Texture* image;

    };
}