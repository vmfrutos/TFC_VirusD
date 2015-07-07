#include "ModalWindow.h"

Modalwindow::Modalwindow(const string& layout){

	initialize(layout);

}
Modalwindow::~Modalwindow(){

	if (_window) {
		_window->destroy();
		_window = 0;

	}

}



void Modalwindow::initialize(const string& layout){


	CEGUI::Window* _sheet = CeguiManager::getSheet();
	_window  = CEGUI::WindowManager::getSingleton().loadWindowLayout(layout);
	// Se crean los ImageSet
	CEGUI::ImagesetManager::getSingleton().createFromImageFile("ModalWindowBackGroud", "WindowBackGround.png");
	// Se establecen imagenes a los widgets
	_window->getChild("ModalWindow/Fondo")->setProperty("Image", "set:ModalWindowBackGroud image:full_image");
	_sheet->addChildWindow(_window);
	_window->hide();

}

void Modalwindow::show(){
	_window->show();
}

void Modalwindow::hide(){
	_window->hide();
}

void Modalwindow::setText(const std::string& text){
	_window->getChild("ModalWindow/Fondo")->getChild("Text")->setText(text);

}





