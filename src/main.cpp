/*********************************************************************
 * Módulo 1. Curso de Experto en Desarrollo de Videojuegos
 * Autor: David Vallejo Fernández    David.Vallejo@uclm.es
 *
 * Código modificado a partir de Managing Game States with OGRE
 * http://www.ogre3d.org/tikiwiki/Managing+Game+States+with+OGRE
 * Inspirado en Managing Game States in C++
 * http://gamedevgeek.com/tutorials/managing-game-states-in-c/
 *
 * You can redistribute and/or modify this file under the terms of the
 * GNU General Public License ad published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * and later version. See <http://www.gnu.org/licenses/>.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  
 *********************************************************************/

#define UNUSED_VARIABLE(x) (void)x

#include "GameManager.h"
#include "IntroState.h"
#include "PlayState.h"
#include "PauseState.h"
#include "GameSound.h"

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

	GameSound* gameSound = new GameSound();

	UNUSED_VARIABLE(introState);
	UNUSED_VARIABLE(playState);
	UNUSED_VARIABLE(pauseState);
	UNUSED_VARIABLE(gameSound);

	try
	{
		// Inicializa el juego y transición al primer estado.
		game->start(IntroState::getSingletonPtr());
		//game->start(PlayState::getSingletonPtr());
	}
	catch (Ogre::Exception& e)
	{
		std::cerr << "Excepción detectada: " << e.getFullDescription();
	}

	delete game;

	return 0;
}
