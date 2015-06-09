#ifndef __ENEMY_H__
#define __ENEMY_H__


#include <Ogre.h>
#include "PhysicsWorld.h"
#include "CharacterControllerNPC.h"
#include "MyKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletCollision/BroadphaseCollision/btOverlappingPairCache.h"
#include "PlayState.h"
#include <stdlib.h>
#include <time.h>

using namespace Ogre;
using namespace std;
class Enemy{
public:
	Enemy(String name, String mesh,SceneManager* sceneMgr, btPairCachingGhostObject * ghostObject, btConvexShape * convexShape, btScalar stepHeight, btCollisionWorld * collisionWorld,Vector3 & origin, int upAxis = 1);

	CharacterControllerNPC * getCCNPC();
	MyKinematicCharacterController * getCCPhysics();
	void updateCharacter(Real dt);

private:
	static const int WALK_SPEED = 1.0;
	static const int RUN_SPEED = 15;
	static const int TURN_SPEED = 500;
	static const int CAM_HEIGHT = 0.5;

	enum Direction
	{
		X_POSITIVE,
		X_NEGATIVE,
		Z_POSITIVE,
		Z_NEGATIVE
	};

	MyKinematicCharacterController* _CCPhysics;
	CharacterControllerNPC * _CCNPC;
	SceneNode * _bodyNode;
	SceneManager* _sceneManager;
	//Real _pivotPitch;
	Vector3 _walkDirection;
	Vector3 _keyDirection;
	Vector3 _goalDirection;

	bool mIsFalling;
	bool mJumped;
	bool mRun;

	void walk(Direction direction);
	void stop();
	void moveRandom();
	void IA();
};
#endif
