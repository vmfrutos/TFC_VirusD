#include "CompletedState.h"

template<> CompletedState* Ogre::Singleton<CompletedState>::msSingleton = 0;

CompletedState::CompletedState(){
	_modalWindow = NULL;


}
CompletedState::~CompletedState(){

}

void
CompletedState::enter ()
{

	_modalWindow = new Modalwindow("modalWindow.layout");
	_modalWindow->show();
	_modalWindow->setText("Juego Completado.");

}

void
CompletedState::exit ()
{
	_modalWindow->hide();
	delete _modalWindow;
	_modalWindow = 0;
}

void
CompletedState::pause ()
{
}

void
CompletedState::resume ()
{
}

bool
CompletedState::frameStarted
(const Ogre::FrameEvent& evt)
{
	return true;
}

bool
CompletedState::frameEnded
(const Ogre::FrameEvent& evt)
{

	return true;
}

bool
CompletedState::frameRenderingQueued
(const Ogre::FrameEvent& evt)
{
	return true;
}

void
CompletedState::keyPressed
(const OIS::KeyEvent &e) {

	// Se se pulsa cualquier tecla se cambia a PlayState
	changeState(IntroState::getSingletonPtr());
}

void
CompletedState::keyReleased
(const OIS::KeyEvent &e)
{
}

void
CompletedState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
CompletedState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
CompletedState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

CompletedState*
CompletedState::getSingletonPtr ()
{
	return msSingleton;
}

CompletedState&
CompletedState::getSingleton ()
{ 
	assert(msSingleton);
	return *msSingleton;
}

