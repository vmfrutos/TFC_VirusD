#ifndef __HUD_H__
#define __HUD_H__

#include "Contador.h"
#include "Properties.h"
#include "CeguiManager.h"
#include "Properties.h"
#include <CEGUI.h>

class Hud {
public:
	Hud();
	virtual ~Hud();

	bool update(float delta, float fps);
	void resetTime(float time);

	void setVisibleFPS(bool visible);
	bool isVisibleFPS();

	void getKey();
	void getGasCan();

	void setSemaforoRojo();
	void setSemaforoVerde();

private:

	CEGUI::Window* _hud;
	CEGUI::Window* _timeText;
	CEGUI::Window* _fpsText;

	Contador _contador;

	float _accumulatedToRefreshFPS;
	float _timeToRefreshFPS;


	void initialize();

};
#endif
