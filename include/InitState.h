#ifndef InitState_H
#define InitState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include "ModalWindow.h"
#include "PlayState.h"
#include "GameSound.h"
#include "GameState.h"


class InitState : public Ogre::Singleton<InitState>, public GameState
{
public:

	InitState();
	virtual ~InitState();

	void enter ();
	void exit ();
	void pause ();
	void resume ();

	void keyPressed (const OIS::KeyEvent &e);
	void keyReleased (const OIS::KeyEvent &e);

	void mouseMoved (const OIS::MouseEvent &e);
	void mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
	void mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);

	bool frameStarted (const Ogre::FrameEvent& evt);
	bool frameEnded (const Ogre::FrameEvent& evt);
	bool frameRenderingQueued (const Ogre::FrameEvent& evt);

	// Heredados de Ogre::Singleton.
	static InitState& getSingleton ();
	static InitState* getSingletonPtr ();

protected:


	Modalwindow* _modalWindow;

	void updateText(float delta);
	void putLetter();
	string readTextFromFile();
	std::string replaceAll(std::string str, const std::string& from, const std::string& to);

	float timeToPutLetter; // Tiempo en segundos que tiene que pasar para poner la siguiente letra

	float acculmulatedTime;

	unsigned int lastShowLetter; // Ultima letra escrita

	string textToShow;
};

#endif
