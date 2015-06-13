#ifndef IntroState_H
#define IntroState_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"
#include "PlayState.h"


class IntroState : public Ogre::Singleton<IntroState>, public GameState
{
public:
	IntroState();
	virtual ~IntroState();

	void enter ();
	void exit ();
	void pause ();
	void resume ();
	string getName ();

	void keyPressed (const OIS::KeyEvent &e);
	void keyReleased (const OIS::KeyEvent &e);

	void mouseMoved (const OIS::MouseEvent &e);
	void mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
	void mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);

	bool frameStarted (const Ogre::FrameEvent& evt);
	bool frameEnded (const Ogre::FrameEvent& evt);
	bool frameRenderingQueued (const Ogre::FrameEvent& evt);

	// Heredados de Ogre::Singleton.
	static IntroState& getSingleton ();
	static IntroState* getSingletonPtr ();

protected:

	bool _exitGame;
	CEGUI::Window* _window;

private:
	void initialize();
	void finalize();
	bool clickPlay(const CEGUI::EventArgs &e);
	bool clickRecords(const CEGUI::EventArgs &e);
	bool clickCredits(const CEGUI::EventArgs &e);
	bool clickExit(const CEGUI::EventArgs &e);

	bool mouseOutButton(const CEGUI::EventArgs &e);
	bool mouseInButton(const CEGUI::EventArgs &e);

	// Sonido
	TrackPtr _audioIntro;
};

#endif
