#pragma once
#include "Label.h"

namespace demo{
    class Button;
    typedef std::shared_ptr<Button> ButtonPtr;

    class Button : public Label {
        public:
            ~Button();
            bool getDownState() const { return down; }
            void setDown(bool state);
            void draw() const;
        protected:
            Button(int x,int y,int w,int h,std::string txt);
        private:
            bool down = false;
    };
}
