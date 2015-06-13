#include  "CeguiManager.h"

CEGUI::Window* CeguiManager::_sheet;

void
CeguiManager::initialize(){
	  //CEGUI

	  CEGUI::OgreRenderer::bootstrapSystem();


	  CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	  CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	  CEGUI::Font::setDefaultResourceGroup("Fonts");
	  CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	  CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");

	  CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	  CEGUI::System::getSingleton().setDefaultFont("DejaVuSans-10");
	  CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook","MouseArrow");
	  _sheet = CEGUI::WindowManager::getSingletonPtr()->createWindow("DefaultWindow","Sheet");

	  CEGUI::System::getSingleton().setGUISheet(_sheet);
	  CEGUI::MouseCursor::getSingleton().hide();

}

void
CeguiManager::finalize(){
	if (_sheet) {
		_sheet->destroy();
	}
}

CEGUI::Window*
CeguiManager::getSheet(){
	if (_sheet) {
		return _sheet;
	}
	return 0;
}

CEGUI::MouseButton
CeguiManager::convertMouseButton(OIS::MouseButtonID id) {
	  CEGUI::MouseButton ceguiId;
	  switch(id)
	  {
	  case OIS::MB_Left:
		  ceguiId = CEGUI::LeftButton;
		  break;
	  case OIS::MB_Right:
		  ceguiId = CEGUI::RightButton;
		  break;
	  case OIS::MB_Middle:
		  ceguiId = CEGUI::MiddleButton;
		  break;
	  default:
		  ceguiId = CEGUI::LeftButton;
	  }
	  return ceguiId;
}



