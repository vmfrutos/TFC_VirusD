#include "Hud.h"

Hud::Hud(){
	_hud = 0;

	initialize();


}
Hud::~Hud(){
	if (_hud) {
		_hud->hide();
		_hud->destroy();
		_hud = 0;
	}

}

void Hud::initialize(){

	CEGUI::Window* _sheet = CeguiManager::getSheet();

	// Se crea el layout
	_hud  = CEGUI::WindowManager::getSingleton().loadWindowLayout("hud.layout");



	// Se crean los ImageSet
	//CEGUI::ImagesetManager::getSingleton().createFromImageFile("ImageTimeBox", "TimeBox.png");
	CEGUI::ImagesetManager::getSingleton().createFromImageFile("ImageGasoil", "bidon.png");
	CEGUI::ImagesetManager::getSingleton().createFromImageFile("ImageLlave", "llave.png");
	CEGUI::ImagesetManager::getSingleton().createFromImageFile("ImageSemRojo", "rojo.png");
	CEGUI::ImagesetManager::getSingleton().createFromImageFile("ImageSemVerde", "verde.png");



	// Se establecen imagenes a los widgets
	//_hud->getChild("Hud/Fondo")->getChild("TimeBox")->setProperty("Image", "set:ImageTimeBox image:full_image");
	_hud->getChild("Hud/Fondo")->getChild("Gasoil")->setProperty("Image", "set:ImageGasoil image:full_image");
	_hud->getChild("Hud/Fondo")->getChild("llave")->setProperty("Image", "set:ImageLlave image:full_image");

	_hud->getChild("Hud/Fondo")->getChild("semaforo")->setProperty("Image", "set:ImageSemVerde image:full_image");





	_sheet->addChildWindow(_hud);

	_timeText =_hud->getChild("Hud/Fondo")->getChild("TimeText");
	_fpsText =_hud->getChild("Hud/Fondo")->getChild("fps");


	_accumulatedToRefreshFPS = 0;

	_timeToRefreshFPS = Properties::getSingletonPtr()->getPropertyFloat("fps.refresh");

	setVisibleFPS(false);

}

bool Hud::update(float delta, float fps){
	_contador.decrementar(delta);
	int currentSecond = (int)_contador.getSegundosTranscurridos();
	if (currentSecond <= 0){

		return true;
	}

	_timeText->setText(_contador.getContadorMinSecStr());

	// Los fps se actualizan cada X segundos (El valor es configurable)
	_accumulatedToRefreshFPS += delta;

	if (_accumulatedToRefreshFPS >=_timeToRefreshFPS){
		_fpsText->setText(Ogre::StringConverter::toString((int)fps));
		_accumulatedToRefreshFPS = 0;
	}

	return false;
}





void Hud::resetTime(float time){
	_contador.reset(time);
	_timeText->setText(_contador.getContadorMinSecStr());
}

void Hud::setVisibleFPS(bool visible){
	if (visible == false){
		_fpsText->hide();
	} else {
		_fpsText->show();
	}
}

void Hud::getKey(){
	_hud->getChild("Hud/Fondo")->getChild("llave")->setProperty("Alpha","1.0");
}

void Hud::getGasCan(){
	_hud->getChild("Hud/Fondo")->getChild("Gasoil")->setProperty("Alpha","1.0");
}

void
Hud::setSemaforoRojo(){
	_hud->getChild("Hud/Fondo")->getChild("semaforo")->setProperty("Image", "set:ImageSemRojo image:full_image");
}

void
Hud::setSemaforoVerde(){
	_hud->getChild("Hud/Fondo")->getChild("semaforo")->setProperty("Image", "set:ImageSemVerde image:full_image");
}




