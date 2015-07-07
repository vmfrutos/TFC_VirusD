#ifndef CompletedState_H
#define CompletedState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include "ModalWindow.h"
#include "PlayState.h"
#include "GameSound.h"
#include "GameState.h"


class CompletedState : public Ogre::Singleton<CompletedState>, public GameState
{
public:

	CompletedState();
	virtual ~CompletedState();

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
	static CompletedState& getSingleton ();
	static CompletedState* getSingletonPtr ();

protected:


	Modalwindow* _modalWindow;
};

#endif
