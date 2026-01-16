#pragma once
#include "Sprite.h"

namespace grupp24{
    class Label;
    typedef std::shared_ptr<Label> LabelPtr;

    class Label : public Sprite {
        public:
            virtual ~Label();
            void setText(std::string text);
            const std::string& getText() const;
        protected:
            Label(int x, int y, int w, int h, std::string text);
        private:
            std::string text;
    };
}