#pragma once
#include "Sprite.h"

namespace demo{
    class Label;
    typedef std::shared_ptr<Label> LabelPtr;

    class Label : public Sprite, public std::enable_shared_from_this<Label>{
        public:
            virtual ~Label();
            void draw() const override;
            void setText(std::string);
            std::string getText() const;
        protected:
            Label(int x, int y, int w, int h, std::string text);
        private:
            SDL_FRect rect;
            std::string text;
            SDL_Texture* image;

    };
}