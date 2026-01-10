#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include <memory>

namespace demo {
    class Sound {
    public:
        Sound();
        ~Sound();

        void play(const std::string& name);
    
    private:
        struct SoundInstance {
            SDL_AudioStream* stream;
            Uint8* buffer;
            Uint32 length;
        };
    
        std::vector<std::unique_ptr<SoundInstance>> activeSounds;
    };
}

