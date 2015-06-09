#ifndef __CHARECTERCONTROLLERPLAYER__H
#define __CHARECTERCONTROLLERPLAYER__H

#include <Ogre.h>


using namespace Ogre;
using namespace std;

class CharacterControllerPlayer
{
	private:
		static const int NUM_ANIMS = 2;
		static constexpr const float ANIM_FADE_SPEED = 7.5;

		// Tipo enumerado con los identificadores para las animaciones
		enum AnimID
		{
			ANIM_WALK_BASE,
			ANIM_RUN_BASE,
			//ANIM_WALK_TOP,

			/*
			ANIM_HANDS_CLOSED,
			ANIM_HANDS_RELAXED,

			ANIM_DRAW_SWORDS,

			ANIM_SLICE_VERTICAL,
			ANIM_SLICE_HORIZONTAL,

			ANIM_DANCE,

			ANIM_JUMP_START,
			ANIM_JUMP_LOOP,
			ANIM_JUMP_END,
			*/
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
		CharacterControllerPlayer(String name, String mesh,SceneManager * sceneMgr, Vector3 & origin);
		void addTime(Real deltaTime);

		SceneNode * getBodySceneNode();
		Vector3 getPosition() const;

		void animWalkStart();
		void animWalkEnd();

		void animRunStart();
		void animRunEnd();
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
