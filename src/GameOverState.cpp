#include "GameOverState.h"

template<> GameOverState* Ogre::Singleton<GameOverState>::msSingleton = 0;

GameOverState::GameOverState(){
	_modalWindow = NULL;
}
GameOverState::~GameOverState(){

}

void
GameOverState::enter ()
{
	_modalWindow = new Modalwindow("modalWindow.layout");
	_modalWindow->show();
	_modalWindow->setText("Game Over");
}

void
GameOverState::exit ()
{
	_modalWindow->hide();
	delete _modalWindow;
	_modalWindow = 0;
}

void
GameOverState::pause ()
{
}

void
GameOverState::resume ()
{
}

bool
GameOverState::frameStarted
(const Ogre::FrameEvent& evt)
{
	return true;
}

bool
GameOverState::frameEnded
(const Ogre::FrameEvent& evt)
{

	return true;
}

bool
GameOverState::frameRenderingQueued
(const Ogre::FrameEvent& evt)
{
	return true;
}

void
GameOverState::keyPressed
(const OIS::KeyEvent &e) {

}

void
GameOverState::keyReleased
(const OIS::KeyEvent &e)
{
	if (e.key == OIS::KC_RETURN){
		// Se se pulsa enter tecla se cambia a IntroState
		changeState(IntroState::getSingletonPtr());
	}
}

void
GameOverState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
GameOverState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
GameOverState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

GameOverState*
GameOverState::getSingletonPtr ()
{
	return msSingleton;
}

GameOverState&
GameOverState::getSingleton ()
{ 
	assert(msSingleton);
	return *msSingleton;
}
