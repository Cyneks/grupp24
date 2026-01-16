#pragma once
#include "Label.h"

namespace grupp24{
    class Button;
    typedef std::shared_ptr<Button> ButtonPtr;

    class Button : public Label {
        public:
            ~Button();
            bool getPressedState() const { return pressedState; }
            void setPressedState(bool state);
            void draw() const;
        protected:
            Button(int x,int y,int w,int h,std::string txt);
        private:
            bool pressedState = false;
    };
}
