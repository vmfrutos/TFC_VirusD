#include "IntroState.h"


template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

IntroState::IntroState(){
	_exitGame = false;
	_window = 0;
}

IntroState::~IntroState(){

}

void
IntroState::enter ()
{

	_exitGame = false;

	initialize();

	CEGUI::MouseCursor::getSingleton().show();

	// Se carga la pista de audio
	_audioIntro = GameSound::getSingletonPtr()->loadTrack("horror-ambiance.wav");

	// Se pone en modo loop
	_audioIntro->play(true);

	_window->show();

}

void
IntroState::exit()
{
	CEGUI::MouseCursor::getSingleton().hide();
	_window->hide();

	// Se detiene la pista de audio
	_audioIntro->stop();
	finalize();

}

void
IntroState::pause ()
{
	CEGUI::MouseCursor::getSingleton().hide();
	_window->hide();
}

void
IntroState::resume ()
{
	CEGUI::MouseCursor::getSingleton().show();
	_window->show();
}

bool
IntroState::frameStarted
(const Ogre::FrameEvent& evt) 
{
	return true;
}

bool
IntroState::frameEnded
(const Ogre::FrameEvent& evt)
{
	if (_exitGame)
		return false;

	return true;
}

bool
IntroState::frameRenderingQueued
(const Ogre::FrameEvent& evt)
{
	return true;
}

void
IntroState::keyPressed
(const OIS::KeyEvent &e)
{
	CEGUI::System::getSingleton().injectKeyDown(e.key);
	CEGUI::System::getSingleton().injectChar(e.text);
}

void
IntroState::keyReleased
(const OIS::KeyEvent &e )
{
	CEGUI::System::getSingleton().injectKeyUp(e.key);
	if (e.key == OIS::KC_ESCAPE) {
		_exitGame = true;
	}
}

void
IntroState::mouseMoved
(const OIS::MouseEvent &evt)
{
	CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
}

void
IntroState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonDown(CeguiManager::convertMouseButton(id));
}

void
IntroState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(CeguiManager::convertMouseButton(id));
}

IntroState*
IntroState::getSingletonPtr ()
{
	return msSingleton;
}

IntroState&
IntroState::getSingleton ()
{ 
	assert(msSingleton);
	return *msSingleton;
}

string IntroState::getName (){
	return "IntroState";

}
void IntroState::finalize(){
	if (_window) {
		_window->destroy();
		_window = 0;
	}
}
void IntroState::initialize(){

	CEGUI::Window* _sheet = CeguiManager::getSheet();

	//Se crea el layout
	_window  = CEGUI::WindowManager::getSingleton().loadWindowLayout("intro.layout");

	CEGUI::ImagesetManager::getSingleton().createFromImageFile("IntroBackGroud", "intro.png");

	// Se estable la imagen
	_window->getChild("Intro/Fondo")->setProperty("Image", "set:IntroBackGroud image:full_image");

	_window->getChild("Intro/Fondo")->getChild("Play")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&IntroState::clickPlay,this));
	_window->getChild("Intro/Fondo")->getChild("Records")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&IntroState::clickRecords,this));
	_window->getChild("Intro/Fondo")->getChild("Credits")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&IntroState::clickCredits,this));
	_window->getChild("Intro/Fondo")->getChild("Exit")->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&IntroState::clickExit,this));

	_window->getChild("Intro/Fondo")->getChild("Play")->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea,CEGUI::Event::Subscriber(&IntroState::mouseOutButton,this));
	_window->getChild("Intro/Fondo")->getChild("Records")->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea,CEGUI::Event::Subscriber(&IntroState::mouseOutButton,this));
	_window->getChild("Intro/Fondo")->getChild("Credits")->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea,CEGUI::Event::Subscriber(&IntroState::mouseOutButton,this));
	_window->getChild("Intro/Fondo")->getChild("Exit")->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea,CEGUI::Event::Subscriber(&IntroState::mouseOutButton,this));

	_window->getChild("Intro/Fondo")->getChild("Play")->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea,CEGUI::Event::Subscriber(&IntroState::mouseInButton,this));
	_window->getChild("Intro/Fondo")->getChild("Records")->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea,CEGUI::Event::Subscriber(&IntroState::mouseInButton,this));
	_window->getChild("Intro/Fondo")->getChild("Credits")->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea,CEGUI::Event::Subscriber(&IntroState::mouseInButton,this));
	_window->getChild("Intro/Fondo")->getChild("Exit")->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea,CEGUI::Event::Subscriber(&IntroState::mouseInButton,this));

	_sheet->addChildWindow(_window);


}

bool
IntroState::clickPlay(const CEGUI::EventArgs &e){


	changeState(PlayState::getSingletonPtr());
	return true;
}

bool
IntroState::clickRecords(const CEGUI::EventArgs &e){
	//pushState(RecordsState::getSingletonPtr());
	return true;
}

bool
IntroState::clickCredits(const CEGUI::EventArgs &e){
	//pushState(CreditsState::getSingletonPtr());
	return true;
}

bool
IntroState::clickExit(const CEGUI::EventArgs &e){
	_exitGame = true;
	return true;
}

bool
IntroState::mouseInButton(const CEGUI::EventArgs &e){

	const CEGUI::MouseEventArgs& we = static_cast<const CEGUI::MouseEventArgs&>(e);
	CEGUI::String senderID = we.window->getName();

	_window->getChild("Intro/Fondo")->getChild(senderID)->setAlpha(1.0);
	return true;
}

/**
 * Metodo que se ejecuta cuando el raton se quita de encima de un botón
 */
bool
IntroState::mouseOutButton(const CEGUI::EventArgs &e){

	const CEGUI::MouseEventArgs& we = static_cast<const CEGUI::MouseEventArgs&>(e);
	CEGUI::String senderID = we.window->getName();

	_window->getChild("Intro/Fondo")->getChild(senderID)->setAlpha(0.4);
	return true;
}
