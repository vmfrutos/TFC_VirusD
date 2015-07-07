#define UNUSED_VARIABLE(x) (void)x

#include "GameManager.h"
#include "IntroState.h"
#include "PlayState.h"
#include "PauseState.h"
#include "InitState.h"
#include "GameOverState.h"
#include "CompletedState.h"
#include "GameSound.h"
#include "CreditsState.h"

#include <iostream>

using namespace std;

int main () {

	// Se inicializa la clase Properties que tiene los parámetros de configuración
	new Properties;
#ifdef LINUX
	if (!Properties::getSingletonPtr()->load("conf/config.properties")) {
		return false;
	}
#endif
#ifdef WIN32
	if (!Properties::getSingletonPtr()->load("conf\\config.properties")) {
		return false;
	}
#endif


	GameManager* game = new GameManager();
	IntroState* introState = new IntroState();
	PlayState* playState = new PlayState();
	PauseState* pauseState = new PauseState();
	InitState* initState = new InitState();
	GameOverState* gameOverState = new GameOverState();
	CompletedState* completedState = new CompletedState();
	CreditsState* creditsState = new CreditsState();

	GameSound* gameSound = new GameSound();

	UNUSED_VARIABLE(introState);
	UNUSED_VARIABLE(playState);
	UNUSED_VARIABLE(pauseState);
	UNUSED_VARIABLE(initState);
	UNUSED_VARIABLE(gameSound);
	UNUSED_VARIABLE(gameOverState);
	UNUSED_VARIABLE(completedState);
	UNUSED_VARIABLE(creditsState);

	try
	{
		// Inicializa el juego y transición al primer estado.
		game->start(IntroState::getSingletonPtr());
	}
	catch (Ogre::Exception& e)
	{
		std::cerr << "Excepción detectada: " << e.getFullDescription();
	}

	delete game;

	return 0;
}
