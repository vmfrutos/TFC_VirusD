#include "CreditsState.h"

template<> CreditsState* Ogre::Singleton<CreditsState>::msSingleton = 0;

CreditsState::CreditsState(){
	_modalWindow = NULL;

	timeToPutLetter = 0.1;
	acculmulatedTime = 0.0;
	lastShowLetter = 1;
	textToShow = readTextFromFile();
}
CreditsState::~CreditsState(){

}

void
CreditsState::enter ()
{
	acculmulatedTime = 0.0;
	lastShowLetter = 1;
	_modalWindow = new Modalwindow("modalWindowFull.layout");
	_modalWindow->show();


}

void
CreditsState::exit ()
{
	_modalWindow->hide();
	delete _modalWindow;
	_modalWindow = 0;
}

void
CreditsState::pause()
{
}

void
CreditsState::resume ()
{
}

bool
CreditsState::frameStarted
(const Ogre::FrameEvent& evt)
{
	updateText(evt.timeSinceLastFrame);
	return true;
}

bool
CreditsState::frameEnded
(const Ogre::FrameEvent& evt)
{

	return true;
}

bool
CreditsState::frameRenderingQueued
(const Ogre::FrameEvent& evt)
{
	return true;
}

void
CreditsState::keyPressed
(const OIS::KeyEvent &e) {

	// Se se pulsa cualquier tecla se cambia a PlayState
	changeState(IntroState::getSingletonPtr());
}

void
CreditsState::keyReleased
(const OIS::KeyEvent &e)
{
}

void
CreditsState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
CreditsState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
CreditsState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

CreditsState*
CreditsState::getSingletonPtr ()
{
	return msSingleton;
}

CreditsState&
CreditsState::getSingleton ()
{ 
	assert(msSingleton);
	return *msSingleton;
}

void
CreditsState::updateText(float delta){

	acculmulatedTime += delta;
	if (acculmulatedTime >= timeToPutLetter){

		// Si se ha completado el texto no se hace nada
		if (textToShow.size() >= lastShowLetter)
			putLetter();
		acculmulatedTime = 0;
	}

}

void
CreditsState::putLetter() {

	string letters = textToShow.substr(0,lastShowLetter);
	_modalWindow->setText(letters);
	lastShowLetter++;
	char c = textToShow[lastShowLetter];
	if (isalpha(c)){
		GameSound::getSingletonPtr()->playSoundFX(GameSound::FX_TYPEWRITER);
	}


}

string
CreditsState::readTextFromFile(){
	string line;
	string ret;
	ifstream myfile ("conf/creditos.txt");
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
CreditsState::replaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}


