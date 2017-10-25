#ifndef __SOUNDMANAGER_H_
#define __SOUNDMANAGER_H_

#include <SDL.h>
#include <SDL_mixer.h>

class SoundManager
{
public:
    SoundManager(void);
    void ball(void);
    void racket(void);
    void woosh(void);
    void success(void);
    void playMusic(void);
    void pauseMusic(void);
    void resumeMusic(void);
protected:
	Mix_Chunk *ballSound; 
	Mix_Chunk *paddleSound;
	Mix_Chunk *wooshSound;
    Mix_Chunk *successSound;
	Mix_Music *music;
};

#endif