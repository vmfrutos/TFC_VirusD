#ifndef __CHARACTERCONTROLLERMANAGER_H__
#define __CHARACTERCONTROLLERMANAGER_H__

//#include "CharacterControllerPhysics.h"
#include "CharacterControllerPlayer.h"
#include "MyKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletCollision/BroadphaseCollision/btOverlappingPairCache.h"

#include <OIS/OIS.h>

using namespace Ogre;

class btGhostObject;
class btPairCachingGhostObject;
class btOverlappingPairCache;

class Hero
{
private:
	static const int WALK_SPEED = 5;
	static const int RUN_SPEED = 15;
	static const int TURN_SPEED = 500;
	static const int CAM_HEIGHT = 0.5;

	MyKinematicCharacterController* mCCPhysics;
	CharacterControllerPlayer * mCCPlayer;

	SceneManager * mSceneManager;

	Camera * mCamera;
	SceneNode * mBodyNode;
	SceneNode * mCameraPivot;
	SceneNode * mCameraGoal;
	SceneNode * mCameraNode;
	Real mPivotPitch;
	Vector3 mWalkDirection;


	bool mIsFalling;
	bool mJumped;
	bool mRun;

	Vector3 mGoalDirection;
	Vector3 mKeyDirection;


public:
	Hero(String name, String mesh, SceneManager * scnMgr, Camera * cam, btPairCachingGhostObject * ghostObject,
			btConvexShape * convexShape, btScalar stepHeight, btCollisionWorld * collisionWorld,
			Vector3 & origin, int upAxis = 1
	);


	Hero(SceneManager * scnMgr, CharacterControllerPlayer * ccPlayer,
			MyKinematicCharacterController * ccPhysics
	);

	CharacterControllerPlayer * getCCPlayer();
	MyKinematicCharacterController * getCCPhysics();
	void injectKeyDown(const OIS::KeyEvent & evt);
	void injectKeyUp(const OIS::KeyEvent & evt);
	void injectMouseMove(const OIS::MouseEvent & evt);
	void updateCharacter(Real dt);


private:
	void setupCamera(Camera * cam);
	void updateCamera(Real deltaTime);
	void updateCameraGoal(Real deltaYaw, Real deltaPitch, Real deltaZoom);
	Quaternion updateOrientation(Real deltaTime);
};
#endif
