#ifndef GameSound_H
#define GameSound_H

#include "Ogre.h"
#include "SoundFXManager.h"
#include "GameManager.h"
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>

using namespace std;

class GameSound : public Ogre::Singleton<GameSound> {
public:

	GameSound();
	~GameSound();

	// Heredados de Ogre::Singleton.
	static GameSound& getSingleton ();
	static GameSound* getSingletonPtr ();
	void playSoundFX(int fx);
	void stopSoundFX(int fx);
	bool isPlayingSoundFX(int fx);
	TrackPtr loadTrack(const string& fileTrack);

	static const int FX_TYPEWRITER = 1;
	static const int FX_NEW_OBJECT = 2;
	static const int FX_ZOMBI_01 = 3;
	static const int FX_ZOMBI_02 = 4;
	static const int FX_ZOMBI_03 = 5;
	static const int FX_ZOMBI_04= 6;


	void initialize();

private:

	// Sonido
	TrackManager*  _pTrackManager;
	SoundFXManager*  _pSoundFXManager;




	SoundFXPtr _audioTypewriter;
	SoundFXPtr _audioNewObject;
	SoundFXPtr _audioZombie01;
	SoundFXPtr _audioZombie02;
	SoundFXPtr _audioZombie03;
	SoundFXPtr _audioZombie04;




};
#endif
