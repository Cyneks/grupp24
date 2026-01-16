#include <iostream>

#include "Engine.h"
#include "Sound.h"

//spelmotorklass
namespace grupp24 {
    Sound::Sound() {}

    Sound::~Sound() {
        for (auto& sound : activeSounds) {
            SDL_DestroyAudioStream(sound->stream);
            SDL_free(sound->buffer);
        }
        activeSounds.clear();
    }

    void Sound::play(const std::string& name) {
        SDL_AudioSpec specifier{};
        Uint8* buffer = nullptr;
        Uint32 length = 0;

        if (!SDL_LoadWAV(name.c_str(), &specifier, &buffer, &length)) {
            SDL_Log("Failed to load WAV: %s", SDL_GetError());
            return;
        }

        SDL_AudioStream* stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &specifier, nullptr, nullptr);
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
                [](const std::unique_ptr<SoundInstance>& sound) {
                    if (SDL_GetAudioStreamAvailable(sound->stream) == 0) {
                        SDL_DestroyAudioStream(sound->stream);
                        SDL_free(sound->buffer);
                        return true;
                    }
                    return false;
                }),
            activeSounds.end()
        );
    }
}

