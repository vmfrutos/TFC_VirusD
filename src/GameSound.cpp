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

	_audioTypewriter = _pSoundFXManager->load("typewriter.wav");
	_audioNewObject = _pSoundFXManager->load("newObject.wav");
	_audioZombie01 = _pSoundFXManager->load("zombie_01.wav");
	_audioZombie02 = _pSoundFXManager->load("zombie_02.wav");
	_audioZombie03 = _pSoundFXManager->load("zombie_03.wav");
	_audioZombie04 = _pSoundFXManager->load("zombie_04.wav");
}

void
GameSound::playSoundFX(int fx) {

	if (FX_TYPEWRITER == fx) {
		_audioTypewriter->play(FX_TYPEWRITER,0);
	} else if (FX_NEW_OBJECT == fx) {
		_audioNewObject->play(FX_NEW_OBJECT,0);
	} else if (FX_ZOMBI_01 == fx) {
		_audioZombie01->play(FX_ZOMBI_01,0);
	} else if (FX_ZOMBI_02 == fx) {
		_audioZombie02->play(FX_ZOMBI_02,0);
	} else if (FX_ZOMBI_03 == fx) {
		_audioZombie03->play(FX_ZOMBI_03,0);
	} else if (FX_ZOMBI_04 == fx) {
		_audioZombie04->play(FX_ZOMBI_04,0);
	}
}

bool
GameSound::isPlayingSoundFX(int fx){
	bool ret = false;
	if (FX_TYPEWRITER == fx) {
		ret = _audioTypewriter->isPlaying(FX_TYPEWRITER);
	} else if (FX_NEW_OBJECT == fx) {
		ret = _audioNewObject->isPlaying(FX_NEW_OBJECT);
	} else if (FX_ZOMBI_01 == fx) {
		ret = _audioZombie01->isPlaying(FX_ZOMBI_01);
	} else if (FX_ZOMBI_02 == fx) {
		ret = _audioZombie02->isPlaying(FX_ZOMBI_02);
	} else if (FX_ZOMBI_03 == fx) {
		ret = _audioZombie03->isPlaying(FX_ZOMBI_03);
	} else if (FX_ZOMBI_04 == fx) {
		ret = _audioZombie04->isPlaying(FX_ZOMBI_04);
	}
	return ret;
}

void GameSound::stopSoundFX(int fx){

	if (FX_TYPEWRITER == fx) {
		_audioTypewriter->stop(FX_TYPEWRITER);
	} else if (FX_NEW_OBJECT == fx) {
		_audioNewObject->stop(FX_NEW_OBJECT);
	} else if (FX_ZOMBI_01 == fx) {
		_audioZombie01->stop(FX_ZOMBI_01);
	} else if (FX_ZOMBI_02 == fx) {
		_audioZombie02->stop(FX_ZOMBI_02);
	} else if (FX_ZOMBI_03 == fx) {
		_audioZombie03->stop(FX_ZOMBI_03);
	} else if (FX_ZOMBI_04 == fx) {
		_audioZombie04->stop(FX_ZOMBI_04);
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


