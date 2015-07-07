#ifndef GameOverState_H
#define GameOverState_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameOverState.h"
#include "ModalWindow.h"
#include "IntroState.h"

class GameOverState : public Ogre::Singleton<GameOverState>, public GameState
{
public:
	GameOverState();
	virtual ~GameOverState();

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
	static GameOverState& getSingleton ();
	static GameOverState* getSingletonPtr ();

protected:
	Modalwindow* _modalWindow;
};

#endif
