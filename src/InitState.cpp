#include "InitState.h"

template<> InitState* Ogre::Singleton<InitState>::msSingleton = 0;

InitState::InitState(){
	_modalWindow = NULL;

	timeToPutLetter = 0.1;
	acculmulatedTime = 0.0;
	lastShowLetter = 1;
	textToShow = readTextFromFile();
}
InitState::~InitState(){

}

void
InitState::enter ()
{
	acculmulatedTime = 0.0;
	lastShowLetter = 1;
	_modalWindow = new Modalwindow("modalWindowFull.layout");
	_modalWindow->show();


}

void
InitState::exit ()
{
	_modalWindow->setText("\n\n\n\n\nLoading...");
	_modalWindow->hide();
	delete _modalWindow;
	_modalWindow = 0;
}

void
InitState::pause ()
{
}

void
InitState::resume ()
{
}

bool
InitState::frameStarted
(const Ogre::FrameEvent& evt)
{
	updateText(evt.timeSinceLastFrame);
	return true;
}

bool
InitState::frameEnded
(const Ogre::FrameEvent& evt)
{

	return true;
}

bool
InitState::frameRenderingQueued
(const Ogre::FrameEvent& evt)
{
	return true;
}

void
InitState::keyPressed
(const OIS::KeyEvent &e) {

	// Se se pulsa cualquier tecla se cambia a PlayState
	changeState(PlayState::getSingletonPtr());
}

void
InitState::keyReleased
(const OIS::KeyEvent &e)
{
}

void
InitState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
InitState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
InitState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

InitState*
InitState::getSingletonPtr ()
{
	return msSingleton;
}

InitState&
InitState::getSingleton ()
{ 
	assert(msSingleton);
	return *msSingleton;
}

void
InitState::updateText(float delta){

	acculmulatedTime += delta;
	if (acculmulatedTime >= timeToPutLetter){

		// Si se ha completado el texto no se hace nada
		if (textToShow.size() >= lastShowLetter)
			putLetter();
		acculmulatedTime = 0;
	}

}

void
InitState::putLetter() {

	string letters = textToShow.substr(0,lastShowLetter);
	_modalWindow->setText(letters);
	lastShowLetter++;
	char c = textToShow[lastShowLetter];
	if (isalpha(c)){
		GameSound::getSingletonPtr()->playSoundFX(GameSound::FX_TYPEWRITER);
	}


}

string
InitState::readTextFromFile(){
	string line;
	string ret;
	ifstream myfile ("conf/textoInicio.txt");
	if (myfile.is_open()){
		while ( getline (myfile,line) ){
			ret+=line;

		}
		myfile.close();
	}
	ret = replaceAll(ret,"|","\n");
	return ret;
}

std::string
InitState::replaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

