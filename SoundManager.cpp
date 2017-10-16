#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager(void)
{
	//initialize 
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0){
		std::cout << "Error\n"<<std::endl;
	}
	else if (Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024 ) < 0 ){
		std::cout << "Error\n"<<std::endl;
    }
    else{
	    ballSound = Mix_LoadWAV("./Resources/ball.wav");
	    paddleSound = Mix_LoadWAV("./Resources/racket.wav");
	    music = Mix_LoadMUS( "./Resources/Music.mp3" );
	    successSound = Mix_LoadWAV( "./Resources/success.wav"); 
	    wooshSound = Mix_LoadWAV("./Resources/woosh.wav");
    }
}
void SoundManager::ball(void){
	Mix_PlayChannel( 1, ballSound, 0);
}
void SoundManager::woosh(void){
	Mix_PlayChannel( 1, wooshSound, 0);
}
void SoundManager::racket(void){
	Mix_PlayChannel( -1, paddleSound, 0);
}
void SoundManager::success(void){
	Mix_PlayChannel( 1, successSound, 0);
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