#ifndef __CHARECTERCONTROLLERNPC__H
#define __CHARECTERCONTROLLERNPC__H

#include <Ogre.h>


using namespace Ogre;
using namespace std;

class CharacterControllerNPC
{
	private:
		static const int NUM_ANIMS = 1;
		static constexpr const float ANIM_FADE_SPEED = 7.5;

		// Tipo enumerado con los identificadores para las animaciones
		enum AnimID
		{
			ANIM_WALK_01,
			ANIM_NONE
		};

		Camera * _camera;
		SceneNode * _bodyNode;
		SceneNode * _cameraPivot;
		SceneNode * _cameraGoal;
		SceneNode * _cameraNode;
		Real _pivotPitch;
		String _bodyName;
		String _bodyMesh;



		Entity * _bodyEnt;
		AnimationState * _anims[NUM_ANIMS];
		AnimID _baseAnimID;
		AnimID _topAnimID;
		bool _fadingIn[NUM_ANIMS];
		bool _fadingOut[NUM_ANIMS];
		Real _timer; // Timer para saber cuanto tiempo ha estado una animacion
		bool _moving; // used to decide what animation we want to use after we've landed


	public:
		CharacterControllerNPC(String name, String mesh,SceneManager * sceneMgr, Vector3 & origin);
		void addTime(Real deltaTime);

		SceneNode * getBodySceneNode();
		Vector3 getPosition() const;

		void animWalk01Start();
		void animWalk01End();

		//void animRunStart();
		//void animRunEnd();
		/*
		void animJumpStart();
		void animJumpLoop();
		void animJumpEnd();
		*/
		void setIsMoving(bool isMoving);


	private:
		void setupBody(SceneManager * sceneMgr, Vector3 & origin);
		void setupAnimations();
		void updateAnimations(Real deltaTime);
		void fadeAnimations(Real deltaTime);
		void setBaseAnimation(AnimID id, bool reset = false);
		void setTopAnimation(AnimID id, bool reset = false);
};
#endif
