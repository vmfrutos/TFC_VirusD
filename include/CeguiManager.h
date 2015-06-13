#ifndef __CEGUIMANAGER_H__
#define __CEGUIMANAGER_H__
#include <CEGUI.h>
#include <OIS/OIS.h>
#include <iostream>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>

class CeguiManager {
public:
	CeguiManager();
	~CeguiManager();

	static void initialize();
	static void finalize();
	static CEGUI::Window* getSheet();
	static CEGUI::MouseButton convertMouseButton(OIS::MouseButtonID id);

private:
	static CEGUI::Window* _sheet;



};
#endif
