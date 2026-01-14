#include "Engine.h"
#include "Sound.h"
#include <iostream>

namespace demo {
    Sound::Sound() {}

    Sound::~Sound() {
        for (auto& s : activeSounds) {
            SDL_DestroyAudioStream(s->stream);
            SDL_free(s->buffer);
        }
        activeSounds.clear();
    }

    void Sound::play(const std::string& name) {
        SDL_AudioSpec spec{};
        Uint8* buffer = nullptr;
        Uint32 length = 0;

        if (!SDL_LoadWAV(name.c_str(), &spec, &buffer, &length)) {
            SDL_Log("Failed to load WAV: %s", SDL_GetError());
            return;
        }

        SDL_AudioStream* stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
        if (!stream) {
            SDL_Log("Failed to open audio stream: %s", SDL_GetError());
            SDL_free(buffer);
            return;
        }

        SDL_ResumeAudioStreamDevice(stream);
        SDL_PutAudioStreamData(stream, buffer, length);

        auto sfx = std::make_unique<SoundInstance>();
        sfx->stream = stream;
        sfx->buffer = buffer;
        sfx->length = length;

        activeSounds.push_back(std::move(sfx));
        
        activeSounds.erase(
            std::remove_if(activeSounds.begin(), activeSounds.end(),
                [](const std::unique_ptr<SoundInstance>& s) {
                    if (SDL_GetAudioStreamAvailable(s->stream) == 0) {
                        SDL_DestroyAudioStream(s->stream);
                        SDL_free(s->buffer);
                        return true;
                    }
                    return false;
                }),
            activeSounds.end()
        );
    }
}

