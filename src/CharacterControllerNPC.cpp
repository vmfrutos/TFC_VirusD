#include "CharacterControllerNPC.h"

CharacterControllerNPC::CharacterControllerNPC(String name, String mesh,SceneManager * sceneMgr, Vector3 & origin)
{


	_bodyName = name;
	_bodyMesh = mesh;
	setupBody(sceneMgr, origin);
	setupAnimations();
	_moving = false;
}

void CharacterControllerNPC::addTime(Real deltaTime)
{
	updateAnimations(deltaTime);
}

void CharacterControllerNPC::setupBody(SceneManager * sceneMgr, Vector3 & origin)
{
	_bodyNode = sceneMgr->getRootSceneNode()->createChildSceneNode(origin);
	_bodyEnt = sceneMgr->createEntity(_bodyName, _bodyMesh);
	_bodyNode->attachObject(_bodyEnt);
}

void CharacterControllerNPC::setupAnimations()
{
	_bodyEnt->getSkeleton()->setBlendMode(ANIMBLEND_CUMULATIVE);

	String animNames[] =
	{
			"Walk01"
	};

	for (int i = 0; i < NUM_ANIMS; i++)
	{
		_anims[i] = _bodyEnt->getAnimationState(animNames[i]);
		_anims[i]->setLoop(true);
		_fadingIn[i] = false;
		_fadingOut[i] = false;
	}

	setBaseAnimation(ANIM_NONE);
	//setTopAnimation(ANIM_NONE);

	//_anims[ANIM_HANDS_RELAXED]->setEnabled(true);
}

void CharacterControllerNPC::updateAnimations(Real deltaTime)
{
	Real baseAnimSpeed = 1;

	_timer += deltaTime;

	//TODO: Revisar los saltos
	/*
	if (_baseAnimID == ANIM_JUMP_START && _timer >= _anims[_baseAnimID]->getLength())
	{
		// takeoff animation finished, so time to leave the ground!
		setBaseAnimation(ANIM_JUMP_LOOP, true);
	}
	else if (_baseAnimID == ANIM_JUMP_END)
	{
		if (_timer >= _anims[_baseAnimID]->getLength())
		{
			if (_moving)
			{
				setBaseAnimation(ANIM_RUN_BASE, true);
				setTopAnimation(ANIM_RUN_TOP, true);
			}
			else
			{
				setBaseAnimation(ANIM_IDLE_BASE);
				setTopAnimation(ANIM_IDLE_TOP);
			}
		}
	}
	*/

	if (_baseAnimID != ANIM_NONE)
		_anims[_baseAnimID]->addTime(deltaTime * baseAnimSpeed);

	/*
	if (_topAnimID != ANIM_NONE)
		_anims[_topAnimID]->addTime(deltaTime * topAnimSpeed);
		*/


	fadeAnimations(deltaTime);

}

void CharacterControllerNPC::fadeAnimations(Real deltaTime)
{
	for (int i = 0; i < NUM_ANIMS; i++)
	{
		if (_fadingIn[i])
		{
			// Se incrementa el peso lentamente
			Real newWeight = _anims[i]->getWeight() + deltaTime * ANIM_FADE_SPEED;
			_anims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));

			//Si el peso es uno se desactiva el fadeingIn
			if (newWeight >= 1)
				_fadingIn[i] = false;
		}
		else if (_fadingOut[i])
		{
			// Se decrementa el peso lentamente
			Real newWeight = _anims[i]->getWeight() - deltaTime * ANIM_FADE_SPEED;
			_anims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));

			//Si el peso es cero se desactiva el fadeingOut
			if (newWeight <= 0)
			{
				_anims[i]->setEnabled(false);
				_fadingOut[i] = false;
			}
		}
	}
}

void CharacterControllerNPC::setBaseAnimation(AnimID id, bool reset)
{

	if (_baseAnimID >= 0 && _baseAnimID < NUM_ANIMS)
	{
		// Se establece el fadeout para la actual animación
		_fadingIn[_baseAnimID] = false;
		_fadingOut[_baseAnimID] = true;
	}

	_baseAnimID = id;

	if (id != ANIM_NONE)
	{
		// Para la nueva animación que s eva a poner se establece el fadein
		_anims[id]->setEnabled(true);
		_anims[id]->setWeight(0);
		_fadingOut[id] = false;
		_fadingIn[id] = true;

		if (reset)
			_anims[id]->setTimePosition(0);
	}
}

void CharacterControllerNPC::setTopAnimation(AnimID id, bool reset)
{

	if (_topAnimID >= 0 && _topAnimID < NUM_ANIMS)
	{
		/// Se establece el fadeout para la actual animación
		_fadingIn[_topAnimID] = false;
		_fadingOut[_topAnimID] = true;
	}

	_topAnimID = id;

	if (id != ANIM_NONE)
	{
		// Para la nueva animación que s eva a poner se establece el fadein
		_anims[id]->setEnabled(true);
		_anims[id]->setWeight(0);
		_fadingOut[id] = false;
		_fadingIn[id] = true;

		if (reset)
			_anims[id]->setTimePosition(0);
	}
}


void CharacterControllerNPC::animWalk01Start()
{
	//setTopAnimation(ANIM_WALK_TOP);
	setBaseAnimation(ANIM_WALK_01);
}

void CharacterControllerNPC::animWalk01End()
{
	//setTopAnimation(ANIM_NONE);
	setBaseAnimation(ANIM_NONE, true);
}


/*

void CharacterControllerNPC::animJumpStart()
{
	setBaseAnimation(ANIM_JUMP_START, true);
	setTopAnimation(ANIM_NONE);
	_timer = 0;
}

void CharacterControllerNPC::animJumpLoop()
{
	setBaseAnimation(ANIM_JUMP_LOOP, true);
}

void CharacterControllerNPC::animJumpEnd()
{
	setBaseAnimation(ANIM_JUMP_END, true);
	_timer = 0;
}
*/

SceneNode * CharacterControllerNPC::getBodySceneNode()
{
	return _bodyNode;
}

Vector3 CharacterControllerNPC::getPosition() const
{
	return _bodyNode->getPosition();
}

void CharacterControllerNPC::setIsMoving(bool isMoving)
{
	_moving = isMoving;
}

