#include "musicPlayer.h"
#include <iostream>

MusicPlayer::MusicPlayer() : initialized(false), music(nullptr) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }

    if (Mix_Init(MIX_INIT_MP3) == 0) {
        std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Failed to open audio: " << Mix_GetError() << std::endl;
        Mix_Quit();
        SDL_Quit();
        return;
    }

    initialized = true;
}

MusicPlayer::~MusicPlayer() {
    if (music) {
        Mix_FreeMusic(music);
    }
    Mix_Quit();
    SDL_Quit();
}

bool MusicPlayer::playMusic(const std::string& filePath) {
    if (!initialized) {
        std::cerr << "MusicPlayer not initialized properly." << std::endl;
        return false;
    }

    music = Mix_LoadMUS(filePath.c_str());
    if (!music) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        return false;
    }

    Mix_PlayMusic(music, -1);  // -1 means loop indefinitely
    return true;
}
