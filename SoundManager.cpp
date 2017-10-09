#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager(void)
{
	std::cout << "initializing\n\n"<<std::endl;
	//initialize 
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0){
		std::cout << "Error\n\n"<<std::endl;
	}
	else if (Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024 ) < 0 ){
		std::cout << "Error\n\n"<<std::endl;
    }
    else{
	    ballSound = Mix_LoadWAV("./ball.wav");
	    paddleSound = Mix_LoadWAV("./racket.wav");
	    music = Mix_LoadMUS( "./Music.mp3" ); 
	    wooshSound = Mix_LoadWAV("./whoosh.wav");
    }
}
void SoundManager::ball(void){
	Mix_PlayChannel( 1, ballSound, 0);
}
void SoundManager::woosh(void){
	Mix_PlayChannel( 1, wooshSound, 0);
}
void SoundManager::racket(void){
	Mix_PlayChannel( 1, paddleSound, 0);
}
void SoundManager::playMusic(void){
    Mix_PlayMusic( music, -1 );
}
void SoundManager::pauseMusic(void){
	Mix_PauseMusic();
}

void SoundManager::resumeMusic(void){
	Mix_ResumeMusic();
}