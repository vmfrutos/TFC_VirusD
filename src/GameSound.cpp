#include "GameSound.h"

template<> GameSound* Ogre::Singleton<GameSound>::msSingleton = 0;

GameSound::GameSound() {
	_pTrackManager = 0;
	_pSoundFXManager = 0;
}
GameSound::~GameSound (){

}

void GameSound::initialize() {
	// Se inicializan los gestores de sonido (track y FX)
	_pTrackManager = new TrackManager;
	_pSoundFXManager = new SoundFXManager;

	//_audioDead = _pSoundFXManager->load("dead.wav");
	//_audioNewLevel = _pSoundFXManager->load("newLevel.wav");
	//_audioGameOver = _pSoundFXManager->load("gameOver.wav");
	//_audioShoot = _pSoundFXManager->load("gunshot.wav");
	//_audioElevator = _pSoundFXManager->load("motor.wav");
	//_audioAlarm = _pSoundFXManager->load("alarme.wav");
}

void
GameSound::playSoundFX(int fx) {

	if (FX_NEW_LEVEL == fx) {
		_audioNewLevel->play(FX_NEW_LEVEL,0);
	} else if (FX_DEAD == fx) {
		_audioDead->play(FX_DEAD,0);
	} else if (FX_GAME_OVER == fx) {
		_audioGameOver->play(FX_GAME_OVER,0);
	} else if (FX_SHOOT == fx) {
		_audioShoot->play(FX_SHOOT,0);
	} else if (FX_ELEVATOR == fx) {
		_audioElevator->play(FX_ELEVATOR,1);
	} else if (FX_ALARM == fx) {
		_audioAlarm->play(FX_ALARM,1);
	}
}

bool
GameSound::isPlayingSoundFX(int fx){
	bool ret = false;
	if (FX_NEW_LEVEL == fx) {
		ret = _audioNewLevel->isPlaying(FX_NEW_LEVEL);
	} else if (FX_DEAD == fx) {
		ret = _audioDead->isPlaying(FX_DEAD);
	} else if (FX_GAME_OVER == fx) {
		ret = _audioGameOver->isPlaying(FX_GAME_OVER);
	} else if (FX_SHOOT == fx) {
		ret = _audioShoot->isPlaying(FX_SHOOT);
	} else if (FX_ELEVATOR == fx) {
		ret = _audioElevator->isPlaying(FX_ELEVATOR);
	} else if (FX_ALARM == fx) {
		ret = _audioAlarm->isPlaying(FX_ALARM);
	}

	return ret;
}

void GameSound::stopSoundFX(int fx){

	if (FX_NEW_LEVEL == fx) {
		_audioNewLevel->stop(FX_NEW_LEVEL);
	} else if (FX_DEAD == fx) {
		_audioDead->stop(FX_DEAD);
	} else if (FX_GAME_OVER == fx) {
		_audioGameOver->stop(FX_GAME_OVER);
	} else if (FX_SHOOT == fx) {
		_audioShoot->stop(FX_SHOOT);
	} else if (FX_ELEVATOR == fx) {
		_audioElevator->stop(FX_ELEVATOR);
	} else if (FX_ALARM == fx) {
		_audioAlarm->stop(FX_ALARM);
	}
}

GameSound&
GameSound::getSingleton (){
	assert(msSingleton);
	return *msSingleton;
}

GameSound*
GameSound::getSingletonPtr (){
	return msSingleton;
}


TrackPtr
GameSound::loadTrack(const string& fileTrack){
	return _pTrackManager->getSingletonPtr()->load(fileTrack);
}


