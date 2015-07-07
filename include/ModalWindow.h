#ifndef __MODALWINDOW_H__
#define __MODALWINDOW_H__

#include <Ogre.h>
#include <CEGUI.h>
#include "CeguiManager.h"
using namespace std;

class Modalwindow{
	public:
	Modalwindow(const string& layout);
	~Modalwindow();


	void show();
	void hide();
	void setText(const string& text);


private:

	void initialize(const string& layout);

	CEGUI::Window* _window;




};
#endif
