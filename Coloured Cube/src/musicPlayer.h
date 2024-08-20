#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>

class MusicPlayer {
public:
    // Constructor to initialize SDL and SDL_mixer
    MusicPlayer();

    // Destructor to clean up SDL_mixer
    ~MusicPlayer();

    // Load and play the specified MP3 file in a loop
    bool playMusic(const std::string& filePath);

private:
    bool initialized;
    Mix_Music* music;
};

#endif // MUSICPLAYER_H
